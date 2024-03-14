from dotenv import load_dotenv
from pathlib import Path
from openai import OpenAI

# the following function uses a snippet to guide chatGPT in transforming the given function
def transformBySnippet(function_filepath, snippet_filepath, snippet_opt_filepath):
    # open the needed files:
    with open(function_filepath, "r") as file:
        function = file.read()
    with open(snippet_filepath, "r") as file:
        snippet = file.read()
    with open(snippet_opt_filepath, "r") as file:
        snippet_opt = file.read()

    # enable conection to chatGPT           
    load_dotenv()  # load token from .env
    client = OpenAI()
    
    # train chatGPT with snippet and manual transformation
    response = client.chat.completions.create(
        model="gpt-3.5-turbo",#-4",
        temperature=1.0,
        messages=[
        {"role": "user", "content": "You will be given a c++ snippet. Optimize the snippet using AVX intrinsics."},
        {"role": "user", "content": "snippet: " + snippet},
        {"role": "assistant", "content": snippet_opt},
        {"role": "user", "content": "Explain your work."},
        ]
    )
    generated_text1 = response.choices[0].message.content
    
    # create list of subtasks in the function
    response = client.chat.completions.create(
        model="gpt-3.5-turbo",#-4",
        temperature=1.0,
        messages=[
        {"role": "system", "content": "You will be given a c++ function after the key word 'function:'\
        Make a list of the main subtasks of the function."},
        {"role": "user", "content": "function: " + function},
        ]
    )
    generated_text2 = response.choices[0].message.content

    # determine which subtaks can be optimized
    response = client.chat.completions.create(
        model="gpt-3.5-turbo",#-4",
        temperature=1.0,
        messages=[
        {"role": "user", "content": "Which subtask of the following list can be optimized by using AVX intrinsics?"},
        {"role": "user", "content": "function: " + generated_text1}
        ]
    )
    generated_text3 = response.choices[0].message.content
    
    # create the optimized function with the previously obtained information
    response = client.chat.completions.create(
        model="gpt-3.5-turbo",#-4",
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
        using your explanation of your work with the snippet. Do not change the name of the function. \
         Mark the start of the code with: ```cpp and the end with ```"},
        ]
    )
    generated_text4 = response.choices[0].message.content

    # extract the code from the answer by chatGPT
    try:
        output1 = generated_text4.split("```cpp")

        if (len(output1) > 1): 
            output2 = output1[1].split("```")
            return output2[0]
        else:
            return output1[0]
     
    except:
        print("The code could not be extracted from the answer from chatGPT.")
        return("")

   