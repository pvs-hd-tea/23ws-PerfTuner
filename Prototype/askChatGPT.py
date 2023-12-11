from pathlib import Path
from openai import OpenAI

def askChatGPT(files_in_name, file_out_name, message):

    script_dir = Path(__file__).resolve().parent
    
    prompt = ""
    for file_in_name in files_in_name:
        file_path = script_dir / file_in_name
        file_in = open(file_path,"r")
        prompt += file_in.read()

    client = OpenAI()

    response = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=[
        message,
        {"role": "user", "content": prompt}
    ])
    
    generated_text = response.choices[0].message.content

    # construct file if not already existing
    file_path = script_dir / file_out_name
    with open(file_path, "w") as file_out:
        file_out.write(generated_text)

    file_in.close()