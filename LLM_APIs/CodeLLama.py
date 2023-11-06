import requests

API_URL = "https://api-inference.huggingface.co/models/codellama/CodeLlama-34b-Instruct-hf"
headers = {"Authorization": "Bearer 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"}

def query(payload):
	response = requests.post(API_URL, headers=headers, json=payload)
	return response.json()
	
output = query({
	"inputs": "Hello",
})

print(output)