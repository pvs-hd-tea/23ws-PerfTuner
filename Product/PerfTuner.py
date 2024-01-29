from pathlib import Path

from transformByLLM import transformByLLM
from findSnippetList import findSnippetList
from transformBySnippet import transformBySnippet
#from transformByGoogle import transformByGoogle
from test import test

class PerfTuner:
    
    def __init__(self, subpath, runs_directLLM=3, runs_useSnippet=2, runs_buildSnippet=7, runs_Google=5, runs_useUserSnippet=5, runs_buildUserSnippet=5):
        
        # input files, output file, library file
        self.script_dir = Path(__file__).resolve().parent
        files_path = self.script_dir / subpath
        
        self.function_filepath = files_path / "function.cc"
        self.function_opt_filepath = files_path / "function_opt.cc"
        self.main_filepath = files_path / "main.cc"
        self.library_filepath = self.script_dir / "Snippets" / "library.cc"
        self.output_filepath = files_path / "output_cc"
        self.output_avx_filepath = files_path / "output_avx"
        
        # run limits
        self.runs_directLLM = runs_directLLM
        
        self.runs_useSnippet = runs_useSnippet
        self.runs_buildSnippet = runs_buildSnippet

        self.runs_Google = runs_Google

        self.runs_useUserSnippet = runs_useUserSnippet
        self.runs_buildUserSnippet = runs_buildUserSnippet
        
    
    def do(self):
        
        error = -3
        last_error_change = "-"

        # 1. run (direct LLM)
        for i in range (0, self.runs_directLLM):
            print("# A transformation by direct LLM try has been started:")
            print("- it's the following try of this approach: " + str(i))
            print("")
            
            # transform by using ChatGPT directly
            output = transformByLLM(self.function_filepath)
            with open(self.function_opt_filepath, "w") as file_out:
                file_out.write(output)

            # test the result and finish if successful
            print("# The result is being tested:")
            print("")
            test_result = test(self.main_filepath, self.function_filepath, self.output_filepath, self.function_opt_filepath, self.output_avx_filepath)
            if(test_result==0):
                print("SUCCESS: The working optimized function can be found in " + str(self.function_opt_filepath))
                return [0, 1, i, "-"] # [success, 1. run, ith build]
            else:
                print("THE TRANSFORMATION HAS FAILED.")
                print("")
                if(test_result>error):
                    error = test_result
                    last_error_change = 1

        # construct the voted snippet list
        SnippetList = -1
        while SnippetList == -1:
            SnippetList = findSnippetList(self.function_filepath, self.library_filepath)   
        
        # 2. run (use library)
        for i in range (0, self.runs_useSnippet):

            snippet_filepath = self.script_dir / "Snippets" / SnippetList[i][0]
            snippet_opt_filepath = self.script_dir / "Snippets" / SnippetList[i][1]
            
            for j in range (0, self.runs_buildSnippet):
                print("# A transformation by snippet try has been started:")
                print("- using the snippets:" + str(SnippetList[i]))
                print("- it's the following try using these snippets: " + str(j))
                print("")
                
                # transform by using ChatGPT with a Snippet
                output = transformBySnippet(str(self.function_filepath), snippet_filepath, snippet_opt_filepath)
                with open(self.function_opt_filepath, "w") as file_out:
                    file_out.write(output)

                # test the result and finish if successful
                print("# The result is being tested:")
                print("")
                test_result = test(self.main_filepath, self.function_filepath, self.output_filepath, self.function_opt_filepath, self.output_avx_filepath)
                if(test_result==0):
                    print("SUCCESS: The working optimized function can be found in " + str(self.function_opt_filepath))
                    return [0, 2, i, j]
                else:
                    print("THE TRANSFORMATION HAS FAILED.")
                    print("")
                    if(test_result>error):
                        error = test_result
                        last_error_change = 2

            

        # 3. run (use Google)
        #for i in range (0, self.runs_Google):
        #    
        #    # transform by using ChatGPT directly
        #    transformByGoogle(self.function)

            # test the result and finish if successful
        #    if (test(self.function_opt, self.main)):
        #        return [0, 3, i]
            
        # 4. run (use user snippet)
        #for i in range (0, self.runs_useUserSnippet): 
        #    for j in range (0, self.runs_buildUserSnippet):
                
                # transform by using ChatGPT with a Snippet
        #        transformBySnippet(self.function, userSnippet)

                # test the result and finish if successful
        #        if (test(self.function_opt, self.main)):
        #            return [0, 4, i, j]
                
        return [-1, last_error_change, error, "-"]