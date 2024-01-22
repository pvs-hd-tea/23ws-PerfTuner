def transformBySnippet(function, snippet, snippet_opt):
    from dotenv import load_dotenv
    from pathlib import Path
    from openai import OpenAI

    load_dotenv()
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

    response = client.chat.completions.create(
        model="gpt-3.5-turbo",
        temperature=1.0,
        messages=[
        {"role": "user", "content": "Which subtask of the following list can be optimized by using AVX intrinsics?"},
        {"role": "user", "content": "function: " + generated_text1}
        ]
    )

    generated_text3 = response.choices[0].message.content
    
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
        using your explanation of your work with the snippet."},
        ]
    )

    generated_text4 = response.choices[0].message.content
    
    return generated_text4