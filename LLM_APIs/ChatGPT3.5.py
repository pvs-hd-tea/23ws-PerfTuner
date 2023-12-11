# Test how to call the API of ChatGPT 3.5

from dotenv import load_dotenv
from pathlib import Path
from openai import OpenAI

load_dotenv()

input = "example.cpp"
input1 = "snippet1.cc"
input2 = "snippet2.cc"
output = "exampleOpt.cpp"

script_dir = Path(__file__).resolve().parent
file_path = script_dir / input
file_in = open(file_path,"r")
promt = file_in.read()
file_in.close()
file_path = script_dir / input1
file_in = open(file_path,"r")
snippet1 = file_in.read()
file_in.close()
file_path = script_dir / input2
file_in = open(file_path,"r")
snippet2 = file_in.read()
file_in.close()

environment = "double A[n][n+1], n is a multiple of B, B is arbitrary"


file_path = script_dir / output
file_out = open(file_path,"w")


client = OpenAI()

response = client.chat.completions.create(
    model="gpt-3.5-turbo",
    temperature=1.0,
    messages=[
    {"role": "system", "content": "You will be given a c++ function after the key word 'function:'\
     You will be given some extra information about the \
     environment of the function after the key word 'environment:'.\
     You will also be given two c++ snippets (after the keywords 'snippet1:' and 'snippet2:'). \
     Both snippets do the same, however the second snippet uses AVX intrinsics. \
     Use this analogy to optimize the function. \
     Loading into the AVX registers from a n-dimensional matrix is only possible \
     if the loop variable only appears in the last field.\
     Add _opt to the name of the function. \
     Explain what you are doing and give the optimized function."},
    {"role": "user", "content": "function: " + promt},
    {"role": "user", "content": "environment: " + environment},
    {"role": "user", "content": "snippet1: " + snippet1},
    {"role": "user", "content": "snippet2: " + snippet2}
  ]
)
generated_text = response.choices[0].message.content
file_out.write(generated_text)
print(generated_text)

file_out.close()

