from pathlib import Path
from openai import OpenAI

def betterCode(file_in_name, file_out_name):

    # Structuring files
    script_dir = Path(__file__).resolve().parent
    file_path = script_dir / file_in_name
    file_in = open(file_path,"r")
    
    file_path = script_dir / "example.cpp"
    code_in = open(file_path,"r")
    
    file_path = script_dir / file_out_name
    file_out = open(file_path,"w")

    promt = code_in.read() + file_in.read()

    client = OpenAI()

    response = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=[
        {"role": "system", "content": "You will be given a C++ file and an algorithm to use AVX2 on one of its subtasks. \
        Optimize the function by making use of AVX intrinsics such that the time complexity is reduced. \
        Do not assume, that the arrays are aligned. \
        Do not assume, that the size of the arrays is a multiple of 8. \
        Use the function _mm256_dp_ps(). \
        Add _opt to the name of the function. \
        Only give the optimized function. Do not any comment in the result, just provide code. \
        Also, provide main function that could execute the vectorized program along with suitable header files and same input data as in C++ code."},
        {"role": "user", "content": promt}
    ])
    
    generated_text = response.choices[0].message.content
    file_out.write(generated_text)

    file_out.close()
    file_in.close()