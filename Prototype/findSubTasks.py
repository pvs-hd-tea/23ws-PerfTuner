from pathlib import Path
from openai import OpenAI

def findSubTasks(file_in_name, file_out_name):

    # Structuring files
    script_dir = Path(__file__).resolve().parent
    file_path = script_dir / file_in_name
    file_in = open(file_path,"r")
    file_path = script_dir / file_out_name
    file_out = open(file_path,"w")

    promt = file_in.read()

    client = OpenAI()

    response = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=[
        {"role": "system", "content": "You will be given C++-code. \
        Which subtasks are tackeled? Give me a list!"},
        {"role": "user", "content": promt}
    ])
    
    generated_text = response.choices[0].message.content
    file_out.write(generated_text)

    file_out.close()
    file_in.close()