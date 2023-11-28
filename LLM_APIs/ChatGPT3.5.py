# Test how to call the API of ChatGPT 3.5

from dotenv import load_dotenv
from pathlib import Path
from openai import OpenAI

load_dotenv()

input = "example2.cpp"
output = "exampleOpt.cpp"

script_dir = Path(__file__).resolve().parent
file_path = script_dir / input
file_in = open(file_path,"r")
file_path = script_dir / output
file_out = open(file_path,"w")

promt = file_in.read()

client = OpenAI()

response = client.chat.completions.create(
    model="gpt-3.5-turbo",
    messages=[
    {"role": "system", "content": "You will be given a c++ function. \
     Optimize the function by making use of AVX intrinsics. \
     Do not assume, that the arrays are aligned. \
     Do not assume, that the size of the arrays is a multiple of 8. \
     Use the function _mm256_dp_ps(). \
     Add _opt to the name of the function. \
     Only give the optimized function. Do not comment the result."},
    {"role": "user", "content": promt}
  ]
)
generated_text = response.choices[0].message.content
file_out.write(generated_text)
print(generated_text)

file_out.close()
file_in.close()

