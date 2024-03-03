import subprocess
import warnings
import time

def test(main_file, function_file, output_file, function_opt, output_file_avx):
    warnings.filterwarnings("ignore")
    
    # Specify the file names (done by the arguments of the function)
    #main_file = "main.cc"
    #function_file = "function.cc"
    #output_file = "output_cc"
    #function_opt = "function_opt.cc"
    #output_file_avx = "output_avx"

    # Run the g++ command to compile the C++ code
    #print("# g++ main.cc function.cc:")
    command = f"g++ {main_file} {function_file} -o {output_file}"
    result = subprocess.run(command, shell=True)

    # Check if the compilation was successful
    if result.returncode == 0:
        #print("- compilation successful")
        print("")
    else:
        #print(result.stderr)
        #print("")
        return -1  

    # Run the g++ command to compile the AVX code
    #print("# g++ main.cc function_opt.cc:")
    command = f"g++ -mavx2 {main_file} {function_opt} -o {output_file_avx}"
    result = subprocess.run(command, shell=True)

    # Check if the compilation was successful
    if result.returncode == 0:
        #print("- compilation successful")
        print("")
    else:
        #print(result.stderr)
        #print("")
        return -3

    # Measure the runtime of the C++ code
    start_time_cc = time.time()
    run_command_cc = f".\\{output_file}"
    result_run_cc = subprocess.run(run_command_cc, shell=True, stdout=subprocess.PIPE, text=True)
    end_time_cc = time.time()
    runtime_cc = end_time_cc - start_time_cc
    
    # Measure the runtime of the AVX code
    start_time_avx = time.time()
    run_command_avx = f".\\{output_file_avx}"
    result_run_avx = subprocess.run(run_command_avx, shell=True, stdout=subprocess.PIPE, text=True)
    end_time_avx = time.time()
    runtime_avx = end_time_avx - start_time_avx
   
    #print("# Compare the output")
    #print("")
    # Check if the outputs are the same
    if result_run_cc.stdout == result_run_avx.stdout:
        #print("- Outputs are the same.")
        print("")
    else:
        #print("- Outputs are different.")
        #print("")
        return -2

    #print("# Measure the runtime")
    #print("")
    #print(f"- Runtime for C++ code: {runtime_cc} seconds")
    #print("")
    #print(f"- Runtime for AVX code: {runtime_avx} seconds")
    #print("")
    if(runtime_cc <= runtime_avx):
        #print("- C++ code has a lower runtime.")
        #print("")
        return -1
    else: 
        #print("- AVX code has a lower runtime.")
        print("")

    # TODO: Result may have to be checked at main.cc, need to remove this part if so.
    
    return 0