import subprocess
import time
import warnings
from pathlib import Path

from openai import OpenAI

warnings.filterwarnings("ignore")

cpp_code = "example.cpp"
avx_code = "exampleOpt.cpp"
main_cpp = "main_c++.cpp"
main_avx = "main_avx.cpp"

script_dir = Path(__file__).resolve().parent
file_path_cpp = script_dir / cpp_code
file_in_cpp = open(file_path_cpp, "r")

file_path_avx = script_dir / avx_code
file_out_avx = open(file_path_avx, "w")

file_path_main_cpp = script_dir / main_cpp
file_out_main_cpp = open(file_path_main_cpp, "w")

file_path_main_avx = script_dir / main_avx
file_out_main_avx = open(file_path_main_avx, "w")

cpp_code_prompt = file_in_cpp.read()


# Function to compile C++ code
def compile_cpp(main_code):
    # Compile using a C++ compiler (adjust compiler and flags as needed)
    result = subprocess.run(f"g++ -mavx2 {main_code} -o createFile", shell=True)
    return result


# Create an OpenAI client
client = OpenAI()

# Initial conversation
conversation = [
    {"role": "user", "content": "Provide a main function to execute the given function along with header files and input data. \
                                Only give the optimized function. Do not add any comment in the result, just provide code."},

    {"role": "user", "content": cpp_code_prompt}
]

# Maximum number of prompts
max_prompts = 5

# Loop to interact with the model
for _ in range(max_prompts):
    # Make the API call for AVX code
    completion_avx = client.chat.completions.create(
        model="gpt-3.5-turbo",
        temperature=1.0,  # NEW param added
        messages=conversation
    )

    # Extract the model's reply for main code
    main_cpp = completion_avx.choices[0].message.content
    # print("CPP Code:\n", main_cpp)

    # Write CPP and main code to file

    with open(file_out_main_cpp.name, 'w') as file:
        file.write(main_cpp)
        file.flush()
    # file_out_main_cpp.write(main_cpp)

    conversation2 = [
        {"role": "system", "content": "You will be given a c++ function. \
             Optimize the function by making use of AVX intrinsics such that the time complexity is reduced. \
             Do not assume, that the arrays are aligned. \
             Do not assume, that the size of the arrays is a multiple of 8. \
             Use the function _mm256_dp_ps(). \
             Add _opt to the name of the function. \
             Only give the optimized function. Do not any comment in the result, just provide code. \
             Also, provide main function that could execute the vectorized program along with suitable header files and same input data as in C++ code."},
        {"role": "user", "content": main_cpp}
    ]

    # Generate main file for original C++ code
    completion_main_cpp = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=conversation2
    )

    main_avx = completion_main_cpp.choices[0].message.content
    # print("AVX Code:\n", main_avx)

    with open(file_out_main_avx.name, 'w') as file:
        file.write(main_avx)
        file.flush()
    # print("Generated Main Code for Original C++:\n", mainavx)

    # Compile C++ code using main file
    start_time = time.time()
    compile_result_cpp = compile_cpp(file_path_main_cpp.__str__())
    end_time = time.time()
    cpp_execution_time = end_time - start_time

    start_time = time.time()
    compile_result_avx = compile_cpp(file_path_main_avx.__str__())
    end_time = time.time()
    avx_execution_time = end_time - start_time

    # Check if compilation was successful
    if compile_result_cpp.returncode == 0 and compile_result_avx.returncode == 0:
        print("Compilation successful.")
        # Check computational speed
        print(f"AVX Execution Time: {avx_execution_time} seconds")
        print(f"C++ Execution Time: {cpp_execution_time} seconds")

        # Add another prompt if AVX program is slower than C++
        if avx_execution_time >= cpp_execution_time:
            next_prompt = input("The AVX program is slower than C++. Please provide more details: ")
            conversation2[-1]["content"] = next_prompt
        else:
            print("AVX program is faster than C++. Exiting.")
            break
    else:
        print("Compilation failed. Please provide more details.")
        next_prompt = input("Additional details: ")
        conversation2[-1]["content"] = next_prompt
