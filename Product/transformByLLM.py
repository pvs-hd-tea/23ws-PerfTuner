import warnings
from pathlib import Path
from openai import OpenAI

def transformByLLM(function_filepath):
    warnings.filterwarnings("ignore")

    with open(function_filepath, "r") as file:
        function = file.read()
    print("- The function is:")
    print(function)
    print("")

    # Create an OpenAI client
    client = OpenAI()

    # Initial conversation
    conversation = [
        {"role": "system", "content": "You will be given a c++ function. \
             Optimize the function by making use of AVX intrinsics such that the time complexity is reduced. \
             Do not assume, that the arrays are aligned. \
             Do not assume, that the size of the arrays is a multiple of 8. \
             Use the function _mm256_dp_ps(). \
             Add _opt to the name of the function. \
             Only give the optimized function. Do not any comment in the result, just provide code."},
        {"role": "user", "content": function}
    ]


    # Make the API call for AVX code
    completion_avx = client.chat.completions.create(
        model="gpt-3.5-turbo",
        temperature=1.0,  # NEW param added
        messages=conversation
    )

    # Extract the model's reply for main code
    output = completion_avx.choices[0].message.content

    print("- The function_opt is:")
    print(output)
    print("")
    
    return output