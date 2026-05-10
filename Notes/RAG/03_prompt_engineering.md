# 📘 Module 3: Prompt Engineering — Talking to AI the Right Way

> **Course:** Build a RAG System from Scratch
> **Prev:** [Module 2 ← What is an LLM?](./02_what_is_an_llm.md)
> **Next:** [Module 4 → Vector Databases](./04_vector_databases.md)

---

## 💬 What is Prompt Engineering?

A **prompt** is anything you send to an LLM. **Prompt engineering** is the skill of crafting prompts that get you the best possible output.

Same question, different prompts → wildly different results. This is one of the highest-leverage skills when building AI systems.

> 💡 Think of it like giving instructions to a new employee. Vague instructions → confused results. Specific, clear instructions → exactly what you wanted.

---

## 🆚 Vague vs Specific Prompts

**Vague prompt:**
```
"Tell me about dogs."
```
*Could result in anything — breeds, history, biology, care tips. You get something, but not necessarily what you need.*

**Specific prompt:**
```
"In 3 bullet points, explain the main health risks for senior dogs 
over 8 years old. Use simple language suitable for a first-time dog owner."
```
*Now the AI knows: topic (senior dog health), format (3 bullets), depth (main risks only), audience (beginners).*

---

## 🏗️ Anatomy of a Good Prompt

A good prompt for a RAG system typically has these parts:

```
┌─────────────────────────────────────────────────────┐
│  ROLE       │ "You are a helpful medical assistant." │
├─────────────────────────────────────────────────────┤
│  TASK       │ "Answer the user's question."          │
├─────────────────────────────────────────────────────┤
│  CONTEXT    │ [The retrieved document chunks]        │
├─────────────────────────────────────────────────────┤
│  RULES      │ "Only use the provided context.        │
│             │  Say 'I don't know' if unsure."        │
├─────────────────────────────────────────────────────┤
│  FORMAT     │ "Answer in 2–3 sentences."             │
├─────────────────────────────────────────────────────┤
│  QUESTION   │ "What is the maximum safe dose of X?"  │
└─────────────────────────────────────────────────────┘
```

---

## 🛠️ Key Prompt Engineering Techniques

### 1. Role Prompting

Tell the AI *who it is* to set its behavior and tone:

```python
system = "You are a senior software engineer with 20 years of Python experience.
          Give concise, expert-level answers. Assume the user knows basic programming."
```

vs.

```python
system = "You are a friendly coding tutor for beginners. 
          Always explain concepts with simple analogies and avoid jargon."
```

Same question → completely different style of response.

---

### 2. Few-Shot Prompting

Show the AI examples of good input/output pairs before asking your real question:

```
Classify whether the following text is positive, negative, or neutral.

Example 1:
Text: "I absolutely love this product!"
Label: Positive

Example 2:
Text: "It broke after one day. Very disappointing."
Label: Negative

Example 3:
Text: "The package arrived on Tuesday."
Label: Neutral

Now classify this:
Text: "It's okay, nothing special but gets the job done."
Label: ???
```

The AI sees the pattern from examples and applies it correctly.

**Zero-shot** = no examples (just ask)
**Few-shot** = give 2–5 examples before asking

Few-shot almost always outperforms zero-shot on structured tasks.

---

### 3. Chain of Thought (CoT)

For complex reasoning, tell the AI to think step by step:

```
Without CoT:
  Q: "A store has 500 apples. They sell 60% on Monday and 25% of the 
      remaining on Tuesday. How many are left?"
  A: "112" ← might be wrong, no explanation

With CoT:
  Q: "...think through this step by step."
  A: "Step 1: 60% of 500 = 300 sold Monday. Remaining: 200.
      Step 2: 25% of 200 = 50 sold Tuesday. Remaining: 150.
      Answer: 150 apples." ← correct, verifiable
```

For RAG, you can use CoT to make the LLM reason through retrieved evidence before answering.

---

### 4. Structured Output Prompting

Force the AI to respond in a specific format (JSON, markdown, etc.):

```python
prompt = """
Extract the following information from the document and return ONLY valid JSON.
No explanation, no extra text. Just the JSON object.

Document: {document}

Required JSON format:
{
  "company_name": "string",
  "revenue_q3": "number or null",
  "ceo_name": "string or null",
  "founded_year": "number or null"
}
"""
```

Then parse the output:

```python
import json
raw = llm.invoke(prompt)
data = json.loads(raw.content)
```

---

### 5. The RAG Prompt Template

This is the most important prompt you'll write in this course — the one that takes retrieved chunks and generates an answer:

```python
RAG_SYSTEM_PROMPT = """You are a helpful assistant answering questions 
based on provided document excerpts.

STRICT RULES:
1. Answer ONLY based on the context provided below. 
2. If the answer is not in the context, say exactly: 
   "I don't have enough information to answer that."
3. Never make up facts, statistics, or names.
4. When possible, mention which part of the context supports your answer.
5. Keep your answer concise and directly responsive to the question.

CONTEXT:
{context}
"""

USER_PROMPT = "Question: {question}"
```

**Why these rules?**
- Rule 1 prevents hallucination
- Rule 2 gives a clean failure mode instead of a wrong answer
- Rule 3 reinforces no hallucination
- Rule 4 adds traceability (you can verify the source)
- Rule 5 prevents rambling

---

### 6. Negative Prompting

Tell the AI what NOT to do, not just what to do:

```python
# Weak:
"Answer the question."

# Stronger:
"""Answer the question.
Do NOT:
- Use bullet points
- Add unnecessary preamble like "Great question!"
- Mention information not in the provided context
- Use technical jargon the user didn't use themselves
"""
```

---

## 🧪 Prompt Testing — The Scientific Method

Treat prompts like experiments:

```
1. Write a prompt
2. Test it on 10-20 diverse inputs
3. Note where it fails
4. Form a hypothesis about why
5. Modify the prompt
6. Test again
7. Repeat
```

Keep a spreadsheet of:
- Prompt version
- What changed
- Test results
- Notes

Prompt engineering is empirical. You won't get it right the first time, and that's normal.

---

## 🚨 Common Prompt Mistakes in RAG Systems

| Mistake | Problem | Fix |
|---------|---------|-----|
| No system prompt | LLM behaves unpredictably | Always add a system prompt |
| Vague rules | LLM fills gaps with hallucinations | Be explicit about what's allowed |
| No "I don't know" escape | LLM makes up answers when unsure | Explicitly allow and define the failure response |
| Huge context dump | LLM gets confused by irrelevant text | Limit to 3–5 most relevant chunks |
| High temperature for facts | Random/inconsistent answers | Use temperature=0 for RAG |
| No output format specified | Unpredictable response structure | Define exact format expected |

---

## 📋 LangChain Prompt Templates in Practice

```python
from langchain.prompts import ChatPromptTemplate

rag_prompt = ChatPromptTemplate.from_messages([
    ("system", """You are a helpful assistant.
Answer ONLY based on the context below.
If the context doesn't contain the answer, say "I don't have that information."
Never make up facts.

Context:
{context}
"""),
    ("human", "{question}")
])

# Use it in a chain
from langchain_openai import ChatOpenAI
from langchain_core.output_parsers import StrOutputParser

llm = ChatOpenAI(model="gpt-4o", temperature=0)

chain = rag_prompt | llm | StrOutputParser()

answer = chain.invoke({
    "context": "The company was founded in 2015 by Ahmed Hassan in Cairo.",
    "question": "Who founded the company?"
})
print(answer)
# "The company was founded by Ahmed Hassan."
```

---

## ✅ Full Recap

| Term | Definition |
|------|-----------|
| **Prompt** | Any text you send to an LLM |
| **System prompt** | Instructions that define the AI's behavior and persona |
| **Zero-shot** | Asking without any examples |
| **Few-shot** | Providing 2–5 examples before the real question |
| **Chain of Thought** | Asking the AI to reason step by step |
| **Structured output** | Forcing the AI to respond in JSON or a specific format |
| **Temperature** | Controls randomness (0 = deterministic, 1+ = creative) |
| **Negative prompting** | Telling the AI what NOT to do |

---

> ➡️ **Next:** [Module 4 — Vector Databases](./04_vector_databases.md)
