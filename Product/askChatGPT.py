from pathlib import Path
from openai import OpenAI

def askChatGPT(files_in_name, file_out_name, message):

    # initialize an empty string to store the input prompt
    prompt = ""

    # check if input files are provided
    if files_in_name != None:
        script_dir = Path(__file__).resolve().parent

        # loop through each input file name
        for file_in_name in files_in_name:
            file_path = script_dir / file_in_name
            file_in = open(file_path,"r")
            # append the content of the file to the prompt string after opening
            prompt += file_in.read()

    # create an instance of the OpenAI class
    client = OpenAI()

    # request completions from the GPT-3.5 model
    response = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=[
        message,
        {"role": "user", "content": prompt}
    ])

    # extract the generated text from the response
    generated_text = response.choices[0].message.content

    # check if no output file name is provided and return the generated text
    if file_out_name == None:
        return generated_text
    
    else:
        # construct file if not already existing
        file_path = script_dir / file_out_name

        # write the generated text to the output file
        with open(file_path, "w") as file_out:
            file_out.write(generated_text)

        # close the input file if opened
        file_in.close()