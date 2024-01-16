from transformByLLM import transformByLLM
from findSnippetList import findSnippetList
from transformBySnippet import transformBySnippet
from transformByGoogle import transformByGoogle
from test import test

class PerfTuner:
    
    def __init__(self, function, function_opt, main, runs_directLLM=5, runs_useSnippet=5, runs_buildSnippet=5, runs_Google=5, runs_useUserSnippet=5, runs_buildUserSnippet=5, library = "Snippets/library.cc"):
        
        # input files, output file, library fily
        self.function = function
        self.function_opt = function_opt
        self.main = main
        self.library = library
        
        # run limits
        self.runs_directLLM = runs_directLLM
        
        self.runs_useSnippet = runs_useSnippet
        self.runs_buildSnippet = runs_buildSnippet

        self.runs_Google = runs_Google

        self.runs_useUserSnippet = runs_useUserSnippet
        self.runs_buildUserSnippet = runs_buildUserSnippet
    
    def do(self):

        # 1. run (direct LLM)
        for i in range (0, self.runs_directLLM):
            
            # transform by using ChatGPT directly
            transformByLLM(self.function)

            # test the result and finish if successful
            if (test(self.function_opt, self.main)):
                return [0, 1, i] # [success, 1. run, ith build]

        # construct the voted snippet list
        SnippetList = findSnippetList(self.function, self.library)   
        
        # 2. run (use library)
        for i in range (0, self.runs_useSnippet): 
            for j in range (0, self.runs_buildSnippet):
                
                # transform by using ChatGPT with a Snippet
                transformBySnippet(self.function, SnippetList[i])

                # test the result and finish if successful
                if (test(self.function_opt, self.main)):
                    return [0, 2, i, j]
            

        # 3. run (use Google)
        for i in range (0, self.runs_Google):
            
            # transform by using ChatGPT directly
            transformByGoogle(self.function)

            # test the result and finish if successful
            if (test(self.function_opt, self.main)):
                return [0, 3, i]
            
        # 4. run (use user snippet)
        for i in range (0, self.runs_useUserSnippet): 
            for j in range (0, self.runs_buildUserSnippet):
                
                # transform by using ChatGPT with a Snippet
                transformBySnippet(self.function, userSnippet)

                # test the result and finish if successful
                if (test(self.function_opt, self.main)):
                    return [0, 4, i, j]
                
        return 1