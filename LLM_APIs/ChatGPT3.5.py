# Test how to call the API of ChatGPT 3.5

import openai

api_key = "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"
openai.api_key = api_key

response = openai.Completion.create(
    engine="davinci",  # GPT-3.5 Modell
    prompt="Übersetze den folgenden Text ins Englische: 'Hallo, wie geht es dir?'",
    max_tokens=50,  # Maximale Anzahl der generierten Tokens
)

generated_text = response.choices[0].text
print(generated_text)