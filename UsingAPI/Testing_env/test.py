import subprocess
import warnings
import time

warnings.filterwarnings("ignore")
# Specify the file names
main_file = "main.cc"
function_file = "function.cc"
output_file = "output_cc"
function_opt= "function_opt.cc"
output_file_avx = "output_avx"

# Run the g++ command to compile the C++ code
command = f"g++ {main_file} {function_file} -o {output_file}"
result = subprocess.run(command, shell=True)

# Check if the compilation was successful
if result.returncode == 0:
    print("C++ function: TRUE")
else:
    print("C++ function:FALSE \nPlease check your C++ code.")
    print(result.stderr)
    exit(1)  # Exit if compilation fails

# Run the g++ command to compile the AVX code
command = f"g++ -march=haswell {main_file} {function_opt} -o {output_file_avx}"
result = subprocess.run(command, shell=True)

# Check if the compilation was successful
if result.returncode == 0:
    print("AVX function: TRUE")
else:
    print("AVX function: FALSE \nPlease check your AVX code.")
    print(result.stderr)
    exit(1)  # Exit if compilation fails

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


#TODO: Result may have to be checked at main.cc, need to remove this part if so.

# Check if the outputs are the same
if result_run_cc.stdout == result_run_avx.stdout:
    print("Outputs are the same.")
else:
    print("Outputs are different.")

# Compare the runtimes
print(f"Runtime for C++ code: {runtime_cc} seconds")
print(f"Runtime for AVX code: {runtime_avx} seconds")

# Compare the runtimes
if runtime_cc < runtime_avx:
    print("C++ code has a lower runtime.")
elif runtime_cc > runtime_avx:
    print("AVX code has a lower runtime.")
else:
    print("Both programs have the same runtime.")