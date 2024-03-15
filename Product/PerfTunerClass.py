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
    
    ### set options here ###
    def __init__(self, subpath, runs_useSnippet=2, runs_buildSnippet=7, snippetListMethod ="tournament", defaultSnippet = "snippet10"): # default settings are set    
        
        # input, output, library files
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

        # option settings to find the to be used snippet
        self.snippetListMethod = snippetListMethod # voting, tournament or default
        self.defaultSnippet = defaultSnippet # snippet 10 (LU decomposition) is set by default
        
    
    ### method that does the transformation ###
    def do(self):
        
        ## construct the snippet list respectively which method is set ##
        SnippetList = -1
        if self.snippetListMethod == "voting":
            while SnippetList == -1: # just try again if it didn't work
                SnippetList = findSnippetList(self.function_filepath, self.library_filepath)  
        
        elif self.snippetListMethod == "tournament":
            while SnippetList == -1:
                SnippetList = findSnippetListByTournament(self.function_filepath, self.library_filepath)
        
        elif self.snippetListMethod == "default":
            print("# The following default snippet is used: " + str(self.defaultSnippet))
            print("")
            SnippetList = [[self.defaultSnippet + ".cc", self.defaultSnippet + "_opt.cc"]]
        
        else:
            print("no valid snippet list construction method given")


        ## multiprocessing setup ##
        Jobs = []

        def create_shared_array(size):
            return multiprocessing.Array('d', size)
        
        jobsStatusArray = [create_shared_array(3) for _ in range(self.runs_useSnippet*self.runs_buildSnippet)] # shared memory status array: [status, runtime_cc, runtime_avx] for each process

        
        for i in range(len(jobsStatusArray)):
            jobsStatusArray[i][0] = -99 # to check later if the process was even active
        
        
        ## instantiate the processes ##
        print("# A transformation by snippet try has been started")
        print("")

        for i in range (0, self.runs_useSnippet): # amount of snippets of SnippetList that should be used
            for j in range (0, self.runs_buildSnippet): # and how often they should be tried
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

        
        ## start the processes ##
        for job in Jobs:
            job.start()

        ## wait for the processes to finish ##       
        for job in Jobs: 
            job.join()

        
        ## output ##
        print("# Statistics of the transformation:")
        print("")
        
        # concerning all processes
        best_results = [] # best status (meaning biggest number) of each transforming process
        success = [0] * self.runs_useSnippet # amount of successful transformations per snippet used
        transformationQuality = [0] * self.runs_useSnippet # sum of statuses per snippet used
        
        # concerning the best transforming process
        numberInList = -1 # rank in SnippetList 
        best_snippet = "" # used snippet 
        buildTrial = -1 # build trial with that snippet 
        runtime_cc_compared = -1 # the runtime of function_cc he was compared against
        
        # general variables needed
        best_time = np.infty
        
        # print the results of each process and collect information
        for index in range(len(jobsStatusArray)):

            # determine the process
            i = math.floor(index/self.runs_buildSnippet) # rank of the snippet used in the snippet list
            j = index - i*self.runs_buildSnippet # build trial with that snippet
            snippet = SnippetList[i][1] 

            # get its information
            status = jobsStatusArray[index][0]
            best_results.append(status)
            transformationQuality[i] += status
            runtime_cc = jobsStatusArray[index][1]
            runtime_avx = jobsStatusArray[index][2]
                    
            # print the information of the process  
            print(str(index+1) + ". Process: " + snippet + ", number in the list: " + str(i) + ", trial " + str(j))  
            
            if status == -99:
                print("- The process wasn't active")
                print("")
            
            elif status == -4:
                print("- FAIL: function.cc didn't compile")
                print("")
            
            elif status == -3:
                print("- FAIL: function_opt.cc didn't compile")
                print("")    
            
            elif status == -2:
                print("- FAIL: The results aren't the same")
                print("")
            
            elif status == -1:
                print("- FAIL: function_opt.cc was slower")
                print("- runtime_cc: " + str(runtime_cc) + " runtime_avx: " + str(runtime_avx))
                print("")
            
            elif status == 0:
                print("- SUCCESS: A working optimized function can be found in " + str(self.function_opt_filepath) + " / " + str(i) + " / " + str(j) + ".cc")
                print("- runtime_cc: " + str(runtime_cc) + ", runtime_avx: " + str(runtime_avx))
                print("")
                success[i] += 1
                
                # check if this process is better than the other ones checked before
                if(runtime_avx < best_time): 
                    best_time = runtime_avx
                    runtime_cc_compared = runtime_cc
                    best_snippet = snippet
                    numberInList = i
                    buildTrial = j
        

        # calculate transformationQualityAverage and successRate
        divisor = self.runs_buildSnippet

        transformationQualityAverage = [-1] * self.runs_useSnippet
        for i in range(self.runs_useSnippet):
            transformationQualityAverage[i] = transformationQuality[i]/divisor

        successRate = [-1] * self.runs_useSnippet
        for i in range(self.runs_useSnippet):
            successRate[i] = success[i]/divisor

        print("=> transformation quality average: " + str(transformationQualityAverage) + ", success rate: " + str(successRate)) 
        print("") 
        
        
        # copy the best function_opt in a for the user easily accessible position
        if (best_snippet != ""):
            print("=> The best transformation can be found in the file function_opt.cc")
            print("")
            
            # Windows
            if os.name == "nt":
                null_output = "NUL"
                command = "copy " + str(self.function_opt_filepath) + "\\" + str(numberInList) + "\\" + str(buildTrial) + f".cc function_opt.cc > {null_output} 2>&1"
            
            # Linux
            else:
                null_output = "/dev/null"
                command = "cp " + str(self.function_opt_filepath) + "/" + str(numberInList) + "/" + str(buildTrial) + f".cc function_opt.cc > {null_output} 2>&1" 
            result = subprocess.run(command, shell=True)
        
        
        # return a statistics array with the information of the transformation
        return[max(best_results), numberInList, best_snippet, buildTrial, best_time, runtime_cc_compared, best_results, transformationQualityAverage, successRate]
