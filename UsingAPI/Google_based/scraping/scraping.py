import requests
from bs4 import BeautifulSoup

def scrape_content(url,word_limit):
    try:
        # Send a GET request to the URL
        response = requests.get(url)

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
            print(f"Error: Unable to fetch content from {url}. Status code: {response.status_code}")
            return None

    except Exception as e:
        print(f"Error: {e}")
        return None

def store_content_in_file(content, file_name):
    try:
        # Open the file in write mode
        with open(file_name, 'w', encoding='utf-8') as file:
            # Write the content to the file
            file.write(content)

        print(f"Content successfully stored in {file_name}")
    except Exception as e:
        print(f"Error: {e}")

    # Example usage with a valid URL:
url = "https://en.wikipedia.org/wiki/Advanced_Vector_Extensions"
extracted_content = scrape_content(url, word_limit=3000)

if extracted_content:
    store_content_in_file(extracted_content, "output.txt")
