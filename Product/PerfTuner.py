from pathlib import Path
import multiprocessing
import math
import numpy as np
import os

from transformByLLM import transformByLLM
from findSnippetList import findSnippetList
from findSnippetListByTournament import findSnippetListByTournament
from transformBySnippet import transformBySnippet
from test import test
from TaskCode import TaskCode
from Controll import Controll

class PerfTuner:
    
    def __init__(self, subpath, runs_directLLM=0, runs_useSnippet=2, runs_buildSnippet=7, runs_Google=5, runs_useUserSnippet=5, runs_buildUserSnippet=5, snippetListMethod ="tournament"):
        
        # input files, output file, library file
        self.script_dir = Path(__file__).resolve().parent
        files_path = self.script_dir / subpath
        
        self.function_filepath = files_path / "function.cc"
        self.function_opt_filepath = files_path / "function_opt" 
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

        # options
        self.snippetListMethod = snippetListMethod
        
    
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

        # construct the snippet list
        SnippetList = -1
        if self.snippetListMethod == "voting":
            while SnippetList == -1:
                SnippetList = findSnippetList(self.function_filepath, self.library_filepath)  
        elif self.snippetListMethod == "tournament":
            while SnippetList == -1:
                SnippetList = findSnippetListByTournament(self.function_filepath, self.library_filepath)
        else:
            print("no valid snippet list construction method given")
                
        # multiprocessing setup
        Jobs = []

        def create_shared_array(size):
            return multiprocessing.Array('d', size)
        
        jobsStatusArray = [create_shared_array(3) for _ in range(self.runs_useSnippet*self.runs_buildSnippet)] # shared memory: status array

        
        for i in range(len(jobsStatusArray)):
            jobsStatusArray[i][0] = -99
        
        # voted snippets
        print("# A transformation by snippet try has been started:")
        print("")

        for i in range (0, self.runs_useSnippet):
            for j in range (0, self.runs_buildSnippet):
                job = multiprocessing.Process(target=TaskCode, args=(i, j, 
                                                                    self.script_dir / "Snippets" / SnippetList[i][0], 
                                                                    self.script_dir / "Snippets" / SnippetList[i][1], 
                                                                    self.function_opt_filepath / str(i) / (str(j) + ".cc"),
                                                                    self.main_filepath, 
                                                                    self.function_filepath, 
                                                                    self.output_filepath / str(i) / str(j),
                                                                    self.output_avx_filepath / str(i) / str(j),
                                                                    jobsStatusArray, self.runs_buildSnippet))
                Jobs.append(job)

        controller = multiprocessing.Process(target= Controll, args=(Jobs, jobsStatusArray))
        
        controller.start()
        for job in Jobs:
            job.start()
                
        for job in Jobs: 
            job.join()
        controller.kill()
        controller.join()

        # output
        print("# Statistics of the transformation:")
        print("")
        best_results = []
        numberInList = -1
        best_snippet = ""
        buildTrial = -1
        best_time = np.infty
        runtime_cc_compared = -1
        transformationQuality = 0
        
        counter = 0
        numberInactive = 0
        success = 0
        for index in range(len(jobsStatusArray)):

                counter+=1

                i = math.floor(index/self.runs_buildSnippet)
                j = index - i*self.runs_buildSnippet
                status = jobsStatusArray[index][0]
                runtime_cc = jobsStatusArray[index][1]
                runtime_avx = jobsStatusArray[index][2]
                snippet = SnippetList[i][1]

                if status == -99:
                    numberInactive += 1
                else:
                    transformationQuality += status
                
                print(str(counter) + ". Process: " + snippet + ", number in the list: " + str(i) + ", trial " + str(j))
                
                best_result_local = -4
                if status == -99:
                    print("- The process wasn't active")
                    print("")
                elif status == -4:
                    print("- FAIL: function.cc didn't compile")
                    print("")
                elif status == -3:
                    best_result_local = -3
                    print("- FAIL: function_opt.cc didn't compile")
                    print("")
                elif status == -2:
                    best_result_local = -2
                    print("- FAIL: The results aren't the same")
                    print("")
                elif status == -1:
                    best_result_local = -1
                    print("- FAIL: function_opt.cc was slower")
                    print("- runtime_cc: " + str(runtime_cc) + " runtime_avx: " + str(runtime_avx))
                    print("")
                elif status == 0:
                    best_result_local = 0
                    success += 1
                    if(runtime_avx < best_time):
                        best_time = runtime_avx
                        runtime_cc_compared = runtime_cc
                        best_snippet
                        numberInList = i
                        buildTrial = j
                    print("- SUCCESS: A working optimized function can be found in " + str(self.function_opt_filepath) + " / " + str(i) + " / " + str(j) + ".cc")
                    print("- runtime_cc: " + str(runtime_cc) + ", runtime_avx: " + str(runtime_avx))
                    print("")

                best_results.append(best_result_local)

        factor=(self.runs_useSnippet*self.runs_buildSnippet)-numberInactive
        if factor!= 0:
            transformationQualityAverage = transformationQuality/factor
        else:
            transformationQualityAverage = 99
        
        factor = (self.runs_useSnippet*self.runs_buildSnippet)-numberInactive
        if factor!= 0:
            successRate = success/factor
        else: successRate = 99
        
        print("=> transformation quality average: " + str(transformationQualityAverage) + ", success rate: " + str(successRate)) 
        print("") 
        
        return[max(best_results), numberInList, best_snippet, buildTrial, best_time, runtime_cc_compared, best_results, transformationQualityAverage, successRate]
