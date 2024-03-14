from pathlib import Path
import multiprocessing
import math
import numpy as np
import os
import subprocess
from findSnippetList import findSnippetList
from findSnippetListByTournament import findSnippetListByTournament
from TaskCode import TaskCode


class PerfTuner:
    
    def __init__(self, subpath, runs_useSnippet=2, runs_buildSnippet=7, snippetListMethod ="tournament", defaultSnippet = "snippet10"):    
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
        self.runs_useSnippet = int(runs_useSnippet)
        self.runs_buildSnippet = int(runs_buildSnippet)

        # options
        self.snippetListMethod = snippetListMethod
        self.defaultSnippet = defaultSnippet
        
    
    def do(self):
        
        # construct the snippet list
        SnippetList = -1
        if self.snippetListMethod == "voting":
            while SnippetList == -1:
                SnippetList = findSnippetList(self.function_filepath, self.library_filepath)  
        elif self.snippetListMethod == "tournament":
            while SnippetList == -1:
                SnippetList = findSnippetListByTournament(self.function_filepath, self.library_filepath)
        elif self.snippetListMethod == "default":
            print("# The follwing default snippet is used: " + str(self.defaultSnippet))
            print("")
            SnippetList = [[self.defaultSnippet + ".cc", self.defaultSnippet + "_opt.cc"]]
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
        print("# A transformation by snippet try has been started")
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

        for job in Jobs:
            job.start()
                
        for job in Jobs: 
            job.join()

        # output
        print("# Statistics of the transformation:")
        print("")
        best_results = []
        numberInList = -1
        best_snippet = ""
        buildTrial = -1
        best_time = np.infty
        runtime_cc_compared = -1
        transformationQuality = [0] * self.runs_useSnippet
        
        counter = 0
        success = [0] * self.runs_useSnippet
        for index in range(len(jobsStatusArray)):

                counter+=1

                i = math.floor(index/self.runs_buildSnippet)
                j = index - i*self.runs_buildSnippet
                status = jobsStatusArray[index][0]
                runtime_cc = jobsStatusArray[index][1]
                runtime_avx = jobsStatusArray[index][2]
                snippet = SnippetList[i][1]

                transformationQuality[i] += status
                
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
                    success[i] += 1
                    if(runtime_avx < best_time):
                        best_time = runtime_avx
                        runtime_cc_compared = runtime_cc
                        best_snippet = snippet
                        numberInList = i
                        buildTrial = j
                    print("- SUCCESS: A working optimized function can be found in " + str(self.function_opt_filepath) + " / " + str(i) + " / " + str(j) + ".cc")
                    print("- runtime_cc: " + str(runtime_cc) + ", runtime_avx: " + str(runtime_avx))
                    print("")

                best_results.append(best_result_local)

        factor = self.runs_buildSnippet

        transformationQualityAverage = [-1] * self.runs_useSnippet
        for i in range(self.runs_useSnippet):
            transformationQualityAverage[i] = transformationQuality[i]/factor

        successRate = [-1] * self.runs_useSnippet
        for i in range(self.runs_useSnippet):
            successRate[i] = success[i]/factor

        
        print("=> transformation quality average: " + str(transformationQualityAverage) + ", success rate: " + str(successRate)) 
        print("") 
        
        
        if (best_snippet != ""):
            print("=> The best transformation can be found in the file function_opt.cc")
            print("")
            if os.name == "nt":
                null_output = "NUL"
                command = "copy " + str(self.function_opt_filepath) + "\\" + str(numberInList) + "\\" + str(buildTrial) + f".cc function_opt.cc > {null_output} 2>&1"
            else:
                null_output = "/dev/null"
                command = "cp " + str(self.function_opt_filepath) + "/" + str(numberInList) + "/" + str(buildTrial) + f".cc function_opt.cc > {null_output} 2>&1" 
            result = subprocess.run(command, shell=True)
        
        return[max(best_results), numberInList, best_snippet, buildTrial, best_time, runtime_cc_compared, best_results, transformationQualityAverage, successRate]
