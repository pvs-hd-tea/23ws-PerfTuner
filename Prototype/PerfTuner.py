# missing:
# 1. add the database
# 2. testing: compile, correctness and speed

# problems:
# 1. does not give nothing if requested (maybe better write a python program that gives a number?)
# 2. Does not onyl return C++-code if asked for better code
# 3. runtime

# ides:
# 1. multiple SubTasksLists and then taking the shortest to reduce runtime

from pathlib import Path
import shutil

from askChatGPT import askChatGPT
from Functionality import file_not_empty
from Functionality import test

class PerfTuner:
    
    def __init__(self, file_input_name, file_output_name, build_trials=3):
        self.file_input_name = file_input_name
        self.file_output_name = file_output_name
        self.file_SubTasks_name = "SubTasks.cpp"
        self.build_trials = build_trials

    
    def do(self):
      
        # find subtasks
        askChatGPT([self.file_input_name], self.file_SubTasks_name, 
                        message={"role": "system", "content": "You will be given C++-code. \
                                Which subtasks are tackeled? Give me a list!"})
        
        # do a copy of the subtasks to use as stack
        script_dir = Path(__file__).resolve().parent
        file_path_src = script_dir / self.file_SubTasks_name
        file_path_dest = script_dir / "SubTasksStack.cpp"
        shutil.copy2(file_path_src, file_path_dest)
        
        SubTask_counter = 0
        potential = []
        while(file_not_empty("SubTasksStack.cpp")):
            
                SubTask_counter += 1
                print(SubTask_counter)
                SubTask_counter_string = str(SubTask_counter)

                # find improvable subtasks/use of AVX2
                askChatGPT(["SubTasksStack.cpp"], SubTask_counter_string + "_improvableSubTask.cpp", 
                        message={"role": "system", "content": "You will be given a list of subtasks from a C++-code. \
                                Tell me if the first subtask can be parallelized? \
                                If that cannot be done, give me nothing."})
                
                askChatGPT([SubTask_counter_string + "_improvableSubTask.cpp"], SubTask_counter_string + "_UseOfAVX2.cpp", 
                        message={"role": "system", "content": "You will be given a subtask that can be parallized or nothing. \
                                If you reveived nothing, give me nothing. \
                                If you did receive something, tell me if the given subtask can be parallelized using AVX2? \
                                If that cannot be done, give me nothing."})
                
                # shorten list of subtasks
                askChatGPT(["SubTasksStack.cpp"], "SubTasksStack.cpp", 
                        message={"role": "system", "content": "You will be given a list of subtasks from a C++-code. \
                                Shorten this list by the first subtask and give me the new list.\
                                If that is also the only subtask, give me nothing."})
                
                # check if we can use AVX and if it's worth to proceed
                if(file_not_empty(SubTask_counter_string + "_UseOfAVX2.cpp")):
                        potential.append(True)
                else:
                        potential.append(False)
                        
                        script_dir = Path(__file__).resolve().parent
                        # construct file if not already existing
                        file_path = script_dir / SubTask_counter_string + "_betterCode.cpp"
                        open(file_path, "w") # save better code as an empty file
                                
                        break
                
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
                        

        
                        
                

        
        

       
        



    


        




