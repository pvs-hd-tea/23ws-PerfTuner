import subprocess
import warnings
import time
import os


def test(main_file, function_file, output_file, function_opt, output_file_avx):

    # make files for the executables if not already existing
    output_file.parent.mkdir(parents=True, exist_ok=True)
    output_file_avx.parent.mkdir(parents=True, exist_ok=True)

    # filter out warnings
    warnings.filterwarnings("ignore")
    null_output = "NUL" if os.name == "nt" else "/dev/null"

    # run the g++ command to compile the C++ code
    command = f"g++ {main_file} {function_file} -o {output_file} > {null_output} 2>&1"
    result = subprocess.run(command, shell=True)

    # check if the compilation was successful
    if result.returncode != 0:
        return [-4, -99, -99] 

    # run the g++ command to compile the AVX code
    command = f"g++ -march=haswell {main_file} {function_opt} -o {output_file_avx} > {null_output} 2>&1"
    result = subprocess.run(command, shell=True)

    # check if the compilation was successful
    if result.returncode != 0:
        return [-3, -99, -99]

    # measure the runtime of the C++ code
    start_time_cc = time.time()
    run_command_cc = f"{output_file}" if os.name == "nt" else f"\\{output_file}" # different for Windows
    result_run_cc = subprocess.run(run_command_cc, shell=True, stdout=subprocess.PIPE, text=True)
    end_time_cc = time.time()
    runtime_cc = end_time_cc - start_time_cc
    
    # measure the runtime of the AVX code
    start_time_avx = time.time()
    run_command_avx = f"{output_file_avx}" if os.name == "nt" else f"\\{output_file_avx}" # different for Windows
    result_run_avx = subprocess.run(run_command_avx, shell=True, stdout=subprocess.PIPE, text=True)
    end_time_avx = time.time()
    runtime_avx = end_time_avx - start_time_avx
   
    # check if the outputs are the same
    if result_run_cc.stdout != result_run_avx.stdout:
        return [-2, -99, -99]

    # compare the runtime
    if(runtime_cc < runtime_avx):
        return [-1, runtime_cc, runtime_avx] 
    else:
        return [0, runtime_cc, runtime_avx] 
    
