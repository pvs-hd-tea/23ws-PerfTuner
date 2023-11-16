from openai import OpenAI
client = OpenAI()

completion = client.chat.completions.create(
  model="gpt-3.5-turbo",
  messages=[
    {"role": "system", "content": "You are a code vectorizing machine. You can vectorize c++ code into AVX2 code to create optimized programs."},
    {"role": "user", "content": "Convect a simple summation program from c++ to AVX2 based code."}
  ]
)

print(completion.choices[0].message)