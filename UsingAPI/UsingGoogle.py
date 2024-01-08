import subprocess
import time
from pathlib import Path
from googlesearch import search

from openai import OpenAI

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

# Create an OpenAI client
client = OpenAI()

# Initial conversation
conversation = [
    {"role": "user", "content": "Provide a main function to execute the given function along with header files and input data. "
                                "Only give the optimized function. Do not add any comment in the result, just provide code."},
    {"role": "user", "content": cpp_code_prompt}
]

# Function to interact with the model recursively
def interact_with_model(conversation, max_depth=5):
    if max_depth <= 0:
        print("Maximum depth reached. Exiting.")
        return

    # Make the API call for AVX code
    completion_avx = client.chat.completions.create(
        model="gpt-3.5-turbo",
        temperature=1.0,
        messages=conversation
    )

    # Extract the model's reply for main code
    main_cpp = completion_avx.choices[0].message.content

    # Write CPP and main code to file
    with open(file_out_main_cpp.name, 'w') as file:
        file.write(main_cpp)
        file.flush()

    conversation2 = [
        {"role": "system", "content": "You will be given a c++ function. "
                                      "Optimize the function by making use of AVX intrinsics such that the time complexity is reduced. "
                                      "Do not assume, that the arrays are aligned. "
                                      "Do not assume, that the size of the arrays is a multiple of 8. "
                                      "Use the function _mm256_dp_ps(). "
                                      "Add _opt to the name of the function. "
                                      "Only give the optimized function. Do not any comment in the result, just provide code. "
                                      "Also, provide main function that could execute the vectorized program along with suitable header files and same input data as in C++ code."},
        {"role": "user", "content": main_cpp}
    ]

    # Generate main file for original C++ code
    completion_main_cpp = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=conversation2
    )

    main_avx = completion_main_cpp.choices[0].message.content

    with open(file_out_main_avx.name, 'w') as file:
        file.write(main_avx)
        file.flush()

    # Display the code to the user
    print("Optimized AVX Code:")
    print(main_avx)

    # Add a Google search for the user-provided input
    user_input = conversation2[-1]["content"]
    print(f"\nPerforming a Google search for: {user_input}")
    for j, result in enumerate(search(user_input, num=5, stop=5, pause=2)):
        print(f"Result {j + 1}: {result}")

    # Prompt the user for the next input
    next_prompt = input("\nEnter the next prompt for the chat with GPT-3.5-turbo (type 'Success' to exit): ")

    if next_prompt.lower() == 'success':
        print("Exiting as per user's request.")
    else:
        # Add the new prompt to the conversation
        conversation2[-1]["content"] = next_prompt

        # Recursive call with reduced depth
        interact_with_model(conversation2, max_depth - 1)

# Start the recursive interaction
interact_with_model(conversation)
