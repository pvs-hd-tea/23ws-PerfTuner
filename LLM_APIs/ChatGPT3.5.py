# Test how to call the API of ChatGPT 3.5

from dotenv import load_dotenv
from pathlib import Path
from openai import OpenAI

load_dotenv()

input = "example1.cpp"
input1 = "snippet1.cc"
input2 = "snippet1_opt.cc"
output = "exampleOpt.cpp"

script_dir = Path(__file__).resolve().parent
file_path = script_dir / input
file_in = open(file_path,"r")
function = file_in.read()
file_in.close()
file_path = script_dir / input1
file_in = open(file_path,"r")
snippet = file_in.read()
file_in.close()
file_path = script_dir / input2
file_in = open(file_path,"r")
snippet_opt = file_in.read()
file_in.close()


environment = "double A[n][n+1], n is a multiple of B, B is chosen by the user"
#"double A[n][n+1], n is a multiple of B, B is chosen by the user"
#"const int M = 8; n is multiple of M."


file_path = script_dir / output
file_out = open(file_path,"w")


client = OpenAI()




response = client.chat.completions.create(
    model="gpt-3.5-turbo",
    temperature=1.0,
    messages=[
    {"role": "user", "content": "You will be given a c++ snippet. Optimize the snippet using AVX intrinsics."},
    {"role": "user", "content": "snippet: " + snippet},
    {"role": "assistant", "content": snippet_opt},
    {"role": "user", "content": "Explain your work."},
  ]
)

generated_text1 = response.choices[0].message.content
print(generated_text1)

response = client.chat.completions.create(
    model="gpt-3.5-turbo",
    temperature=1.0,
    messages=[
    {"role": "system", "content": "You will be given a c++ function after the key word 'function:'\
     Make a list of the main subtasks of the function."},
    {"role": "user", "content": "function: " + function},
  ]
)
generated_text2 = response.choices[0].message.content
print(generated_text2)

response = client.chat.completions.create(
    model="gpt-3.5-turbo",
    temperature=1.0,
    messages=[
    {"role": "user", "content": "Which subtask of the following list can be optimized by using AVX intrinsics?"},
    {"role": "user", "content": "function: " + generated_text1}
  ]
)

generated_text3 = response.choices[0].message.content
print(generated_text3)


response = client.chat.completions.create(
    model="gpt-3.5-turbo",
    temperature=1.0,
    messages=[
    {"role": "user", "content": "You will be given a c++ function after the key word 'function:'"},
    {"role": "user", "content": "function: " + function},
    {"role": "user", "content": "Make a list of the main subtasks of the function."},
    {"role": "assistant", "content": generated_text2},
    {"role": "user", "content": "Which subtask of the following list can be optimized by using AVX intrinsics?"},
    {"role": "assistant", "content": generated_text3},
    {"role": "user", "content": "You will be given a c++ snippet. Optimize the snippet using AVX intrinsics."},
    {"role": "user", "content": "snippet: " + snippet},
    {"role": "assistant", "content": snippet_opt},
    {"role": "user", "content": "Explain your work."},
    {"role": "assistant", "content": generated_text1},
    {"role": "user", "content": "Optimize the relevant subtasks of the original function \
     using your explanation of your work with the snippet. \
      Add '_opt' to the name of the function."},
    #{"role": "user", "content": "Be aware that loading into the AVX registers from a 2-dimensional matrix \
    #is only possible if the loop variable only appears in the last field."},
    #{"role": "user", "content": "You will be given some extra information about the global \
    #  environment of the function after the key word 'environment:'. Do not include this information in the function."},
    #{"role": "user", "content": "environment: " + environment}


  ]
)

generated_text4 = response.choices[0].message.content
file_out.write(generated_text4)
print(generated_text4)

file_out.close()

