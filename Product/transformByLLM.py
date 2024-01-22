import warnings
from pathlib import Path
from openai import OpenAI

def transformByLLM():
    warnings.filterwarnings("ignore")

    main_cpp = "main_c++.cpp"
    output_avx = "output_avx.cpp"

    script_dir = Path(__file__).resolve().parent

    file_path_main_avx = script_dir / output_avx
    file_out_main_avx = open(file_path_main_avx, "w")


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
        {"role": "user", "content": main_cpp}
    ]


    # Make the API call for AVX code
    completion_avx = client.chat.completions.create(
        model="gpt-3.5-turbo",
        temperature=1.0,  # NEW param added
        messages=conversation
    )

    # Extract the model's reply for main code
    output_avx = completion_avx.choices[0].message.content

    # Write AVX code to file
    with open(file_out_main_avx.name, 'w') as file:
        file.write(output_avx)
        file.flush()

    return -1