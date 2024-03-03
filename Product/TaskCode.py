from transformBySnippet import transformBySnippet
from test import test

def TaskCode(i,j, snippet_filepath, snippet_opt_filepath, function_opt_filepath, main_filepath, function_filepath, output_filepath, output_avx_filepath, jobsStatusArray, runs_useSnippet):
            
                    
            # transform by using ChatGPT with a Snippet
            output = transformBySnippet(str(function_filepath), snippet_filepath, snippet_opt_filepath)
            function_opt_filepath.parent.mkdir(parents=True, exist_ok=True)
            with open(function_opt_filepath, "w") as file_out:
                file_out.write(output)

            # test the result and finish if successful
            output_filepath.parent.mkdir(parents=True, exist_ok=True)
            output_avx_filepath.parent.mkdir(parents=True, exist_ok=True)
            test_result = test(main_filepath, function_filepath, output_filepath, function_opt_filepath, output_avx_filepath)
            if(test_result==0):
                jobsStatusArray[i*runs_useSnippet+j] = 1
            else:
                jobsStatusArray[i*runs_useSnippet+j] = -1

