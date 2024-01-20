import os
from openai import OpenAI
from googlesearch import search
from bs4 import BeautifulSoup
import requests
import re

# Set your OpenAI API key
openai_api_key = os.getenv("OPENAI_API_KEY")
client = OpenAI(api_key=openai_api_key)

# Function to read C++ code from a file
def read_cpp_code_from_file(file_path):
    with open(file_path, 'r') as file:
        return file.read()

# Function to perform Google search and scrape content
def get_google_search_query(cpp_code):
    # Make the API call to ChatGPT to get the Google search query
    prompt = [
        {"role": "user", "content": "Given the following C++ code:\n\n"},
        {"role": "user", "content": cpp_code},
        {"role": "assistant", "content": "What is the program?(in 5 words) What is the best search that can be used to convert this program to AVX2 code? Mention the search in quotes(within 20 words)"}
    ]

    response = client.chat.completions.create(
        model="gpt-3.5-turbo",
        temperature=0.7,
        max_tokens=150,
        messages=prompt
    )

    # Extract the Google search query from the response
    query = re.findall(r'"(.*?)"', response.choices[0].message.content.strip())
    return query

def scrape_content_from_google(query):
    # Perform the Google search and get the first result
    try:
        query = str(query)

        search_results = search(query, num=1, stop=1, pause=2)
        first_result = next(search_results)
    except StopIteration:
        print("No search results found.")
        return None
    except Exception as e:
        print(f"Error during Google search: {e}")
        return None

    # Word limit, as chatgpt tokens must not be exhausted
    word_limit = 3000

    try:
        # Send a GET request to the URL
        response = requests.get(first_result)

        # Check if the request was successful (status code 200)
        if response.status_code == 200:
            # Parse the HTML content with BeautifulSoup
            soup = BeautifulSoup(response.text, 'html.parser')

            # Extract text content from paragraph tags
            paragraphs = soup.find_all('p')

            # Initialize variables
            word_count = 0
            extracted_content = []

            # Iterate over paragraphs until the word limit is reached
            for paragraph in paragraphs:
                paragraph_text = paragraph.get_text()
                words_in_paragraph = paragraph_text.split()
                if word_count + len(words_in_paragraph) <= word_limit:
                    extracted_content.append(paragraph_text)
                    word_count += len(words_in_paragraph)
                else:
                    break

            return '\n'.join(extracted_content)

        else:
            print(f"Error: Unable to fetch content from {first_result}. Status code: {response.status_code}")
            return None

    except Exception as e:
        print(f"Error: {e}")
        return None

def prompt_chatgpt_with_useful_content_and_store(useful_content):
    # Step 4: Use the useful content in ChatGPT prompts
    if useful_content is None:
        print("No useful content to add to the prompt. Skipping ChatGPT interaction.")
        return None

    prompt = [
        {"role": "user", "content": "Here is some information I found:\n\n" + useful_content},
        {"role": "user", "content": "Can you provide AVX2 code snippets based on this information?"}
    ]

    response = client.chat.completions.create(
        model="gpt-3.5-turbo",
        temperature=0.7,
        messages=prompt
    )

    avx2_code = response.choices[0].message.content

    # Store AVX2 code in a file
    with open("google_response.cc", "w") as avx2_file:
        avx2_file.write(avx2_code)

    return avx2_code

# Function to interact with ChatGPT, Google search, and scrape content
def interact_with_chatgpt():
    # Step 1: Read C++ code from the file
    cpp_code = read_cpp_code_from_file("function.cc")

    # Step 2: Generate Google queries from ChatGPT response
    google_query = get_google_search_query(cpp_code)
    print(google_query)
    # Step 3: Perform Google search and scrape content
    useful_content = scrape_content_from_google(google_query)

    # Step 4: Use the useful content in ChatGPT prompts
    avx2_code = prompt_chatgpt_with_useful_content_and_store(useful_content)

# Start the interaction
interact_with_chatgpt()
