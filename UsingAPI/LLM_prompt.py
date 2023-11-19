from datetime import time
from pathlib import Path
import openai
import subprocess

cpp_code = "example.cpp"
avx_code = "exampleOpt.cpp"

script_dir = Path(__file__).resolve().parent
file_path = script_dir / cpp_code
file_in = open(file_path,"r")
file_path = script_dir / avx_code
file_out = open(file_path,"w")


# Function to compile C++ code
def compile_cpp(code):
    # Save code to a temporary file
    with open("temp.cpp", "w") as file:
        file.write(code)

    # Compile using a C++ compiler (adjust compiler and flags as needed)
    result = subprocess.run(["g++", "temp.cpp", "-o", "temp"], capture_output=True, text=True)

    return result

# Function to check computational speed
def check_speed(avx_code, cpp_code):
    # Measure the execution time of the AVX code
    avx_execution_time = measure_execution_time(avx_code)

    # Measure the execution time of the compiled C++ code
    cpp_execution_time = measure_execution_time("./temp")

    return avx_execution_time, cpp_execution_time

# Function to measure execution time
def measure_execution_time(command):
    start_time = time.time()
    subprocess.run(command, shell=True)
    end_time = time.time()

    return end_time - start_time

# Create an OpenAI client
client = openai.OpenAI()

# Initial conversation
conversation = [
    {"role": "system", "content": "You are a code vectorizing machine. You can vectorize c++ code into AVX2 code to create optimized programs."},
    {"role": "user", "content": "Convert program from c++ to AVX2 based optimized code."}
]

# Maximum number of prompts
max_prompts = 5

# Loop to interact with the model
for _ in range(max_prompts):
    # Make the API call
    completion = client.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=conversation
    )

    # Extract the model's reply
    avx_code = completion.choices[0].message['content']
    print("AVX Code:\n", avx_code)

    # Compile C++ code
    compile_result = compile_cpp(avx_code)

    # Check if compilation was successful
    if compile_result.returncode == 0:
        print("Compilation successful.")
        # Check computational speed
        avx_execution_time, cpp_execution_time = check_speed(avx_code, "./temp")
        print(f"AVX Execution Time: {avx_execution_time} seconds")
        print(f"C++ Execution Time: {cpp_execution_time} seconds")

        # Add another prompt if AVX program is slower than C++
        if avx_execution_time >= cpp_execution_time:
            next_prompt = input("The AVX program is slower than C++. Please provide more details: ")
            conversation.append({"role": "user", "content": next_prompt})
        else:
            print("AVX program is faster than C++. Exiting.")
            break
    else:
        print("Compilation failed. Please provide more details.")
        next_prompt = input("Additional details: ")
        conversation.append({"role": "user", "content": next_prompt})
