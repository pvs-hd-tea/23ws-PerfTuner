# Structure (iteratively advancement): 
# find the right snippet (multiple runs, voting) [add the database]
# optimize
# testing: compile, correctness and speed -> user gives main function(s), one for every subtask

# problems:
# Does not onyl return C++-code if asked for better code
# runtime -> less ChatGPT

# too much yes (solution: multiple runs and all need to be yes)
# too general explanation (-> too long)
# needs to give the list in the right format (solution: multiple trials)
# found subtasks that are not really subtasks (solution: multiple runs and shortest list)

# ideas:
# 1. multiple SubTaskArrays and then taking the shortest to reduce runtime
# 2. maximum amount of tokens for checking potential: Yes or No + explanation
# 3. merging as a later step
# 4. user gives snippets 
# 5. optimize, then optimize result
# 6. maybe voting strategy, instead of first suitable result

from pathlib import Path
import shutil
import sys
import numpy as np

from askChatGPT import askChatGPT
from Functionality import test
from Functionality import split

class PerfTuner:
    
    def __init__(self, file_input_name, file_output_name, shorterListTrials = 20, build_trials=3):
        self.file_input_name = file_input_name
        self.file_output_name = file_output_name
        self.build_trials = build_trials
        self.shorterListTrials = shorterListTrials

    
    def do(self):

        # find subtasks
        SubTaskArrayLength = np.infty
        runs = 0
        while(True): # try again if failed + find shorter list for shorter total runtime
                print(SubTaskArrayLength)
                try:
                        SubTaskString = askChatGPT([self.file_input_name], None, 
                                message={"role": "system", "content": "You will be given C++-code. \
                                        Which subtasks are tackled? Give me a list! "})
                        
                        SubTaskArray_temp = split(SubTaskString)

                        if(len(SubTaskArray_temp) < SubTaskArrayLength):
                                SubTaskArray = SubTaskArray_temp
                                SubTaskArrayLength = len(SubTaskArray)
                        
                        runs += 1
                        if(runs >= self.shorterListTrials):
                               break
                
                except Exception as e:
                        print(f"error: {e}")
        
        SubTaskNumber = len(SubTaskArray)
        
        potential = [False for i in range(SubTaskNumber)]
        explanation = ["" for i in range(SubTaskNumber)]
        
        for i in range(SubTaskNumber):
            
                # find improvable subtasks/use of AVX2
                answer = askChatGPT(None, None, 
                        message={"role": "system", "content": "You have the following subtask from a C++-code:" 
                                + SubTaskArray[i] + 
                                "Tell me if the subtask can be parallelized? \
                                Answer with Yes or No and explain afterwards"})
                
                if "Yes" in answer:
                       potential[i] = True
                       explanation[i] = answer

        # output
        for i in range(SubTaskNumber):
                print("SubTask", i, ":", SubTaskArray[i])
                print("potential : ", potential[i])
                print()
                print("explanation : ", explanation[i])
                print("-------------------------------------------------------------------------------------------------------------------------")
                
                                
        

        sys.exit()
                
        # multiple improvement trials:
        for i in range(self.build_trials):
                # build algorithm to construct better code
                askChatGPT([SubTask_counter_string + "_UseOfAVX2.cpp"], SubTask_counter_string + "_improvementsInSubTask.cpp", 
                        message={"role": "system", "content": "You will be given a subtask that can be parallelized using AVX2. \
                                Tell me how to improve the subtask using AVX2."})

                # do algorithm to construct better code
                askChatGPT([SubTask_counter_string + "_improvementsInSubTask.cpp"], SubTask_counter_string + "_betterCode.cpp", 
                        message={"role": "system", "content": "You will be given a C++ file and an algorithm to use AVX2 on one of its subtasks. \
                                Optimize the function by making use of AVX intrinsics such that the time complexity is reduced. \
                                Do not assume, that the arrays are aligned. \
                                Do not assume, that the size of the arrays is a multiple of 8. \
                                Use the function _mm256_dp_ps(). \
                                Add _opt to the name of the function. \
                                Only give the optimized function. Do not any comment in the result, just provide code. \
                                Also, provide main function that could execute the vectorized program along with suitable header files and same input data as in C++ code."})
                        
                # break if working code is already obtained before the maximum number of build trials
                if(test(SubTask_counter_string + "_betterCode.cpp")):
                        break
                # empty file as better code if the solution is not working
                else:
                        script_dir = Path(__file__).resolve().parent
                        file_path = script_dir / SubTask_counter_string + "_betterCode.cpp"
                        with open(file_path, "w") as file_out:
                                file_out.write() # save better code as an empty file

        # output
        print("parallelization potential:")
        print(potential) 

        input_files_merging = []
        input_files_merging(self.file_input_name)
        input_files_merging(self.file_SubTasks_name)
        for i in range(1, SubTask_counter + 1): 
                input_files_merging.append(str(i) + "_betterCode.cpp")
        
        askChatGPT(input_files_merging, self.file_output_name, 
                                message={"role": "system", "content": "You will be given a C++ file, a list of its subtasks and an improved code using AVX2 for each subtask if possible. \
                                        Merge the improved code into the orginal file. \
                                        Do not assume, that the arrays are aligned. \
                                        Do not assume, that the size of the arrays is a multiple of 8. \
                                        Add _opt to the name of the function. \
                                        Only give the optimized function. Do not any comment in the result, just provide code. \
                                        Also, provide main function that could execute the vectorized program along with suitable header files and same input data as in C++ code."})
                        

        
                        
                

        
        

       
        



    


        




