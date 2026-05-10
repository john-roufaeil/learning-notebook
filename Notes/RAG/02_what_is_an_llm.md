# 📘 Module 2: What is an LLM?

> **Course:** Build a RAG System from Scratch
> **Prev:** [Module 1 ← How AI Thinks](./01_how_ai_thinks.md)
> **Next:** [Module 3 → Prompt Engineering](./03_prompt_engineering.md)

---

## 🤖 LLM = Large Language Model

An LLM is an AI trained on a massive amount of text that learned to **predict the next token** — over and over, billions of times, until it became incredibly good at generating human-like text.

That's it. That's the core trick.

But from this simple task ("predict the next word"), something remarkable emerges: the model has to learn grammar, facts, reasoning, logic, coding, poetry, and more — because all of these help predict what comes next.

---

## 🎮 The Next-Token Game

Imagine this game:

```
Input:  "The Eiffel Tower is located in ___"
Model picks: "Paris" (probability: 97%)
```

Now feed "Paris" back in:

```
Input:  "The Eiffel Tower is located in Paris ___"
Model picks: "," (probability: 85%)
```

Keep going:

```
"The Eiffel Tower is located in Paris, France. It was built in ___"
Model: "1889"
```

Repeat thousands of times and you get a full, coherent answer. The LLM isn't "thinking" in the way humans do — it's doing incredibly sophisticated pattern prediction based on everything it ever read.

---

## 📚 How Training Works (Simplified)

Training an LLM happens in two main stages:

### Stage 1: Pre-training (Learn Everything)

```
Data: ~trillions of tokens from the internet
       (Wikipedia, books, Reddit, GitHub, news, scientific papers...)
       
Task: Given this text → predict the next token
      If wrong → slightly adjust 100 billion internal numbers
      Repeat... billions of times
      
Result: A "base model" that can complete text very well
        but doesn't know how to be a helpful assistant yet
```

**The numbers inside the model are called "weights" or "parameters".**
GPT-4 has an estimated ~1.8 trillion parameters. Each one is a decimal number that gets nudged during training.

### Stage 2: Fine-tuning for Helpfulness (RLHF)

The base model just completes text — it doesn't know how to answer questions helpfully or refuse harmful requests. So after pre-training, there's a second phase:

```
Technique: RLHF = Reinforcement Learning from Human Feedback

1. Generate many responses to prompts
2. Human raters rank which responses are better
3. Train a "reward model" to predict human ratings
4. Adjust the LLM to produce higher-rated responses

Result: ChatGPT, Claude, Gemini — helpful, safe assistants
```

---

## 🧩 What Can LLMs Do?

Because they've read almost everything humans have written, LLMs can:

| Capability | Example |
|-----------|---------|
| Answer questions | "What causes thunder?" |
| Summarize | "Summarize this 10-page report in 3 bullet points" |
| Write code | "Write a Python function to sort a list" |
| Translate | "Translate this to Arabic" |
| Reason step by step | "Solve this math word problem" |
| Extract information | "Find all dates mentioned in this document" |
| Classify text | "Is this email spam or not?" |
| Write creatively | "Write a short story about a robot" |
| Explain concepts | "Explain quantum physics like I'm 12" |

---

## ⚠️ What LLMs Cannot Do (Critical Weaknesses)

Understanding these weaknesses is the whole reason RAG, agents, and fine-tuning exist.

### 1. Hallucination — Making Things Up

LLMs sometimes produce false information confidently. They don't "know" they're wrong. They just predict the most plausible-sounding next token.

```
You:  "Who wrote the novel 'The Glass Kingdom'?"
LLM:  "The Glass Kingdom was written by Sarah Collins in 2017."
      ← Completely fabricated. That book/author may not exist.
```

**Why does this happen?** The model learned to produce fluent, confident text. It has no internal "fact-checker." It's optimizing for *sounds right*, not *is right*.

### 2. Knowledge Cutoff — Frozen in Time

LLMs only know what was in their training data, which has a cutoff date. Anything after that date is unknown to them.

```
Training cutoff: April 2024
You: "Who won the 2025 World Cup?"
LLM: "I don't have information about events after April 2024."
     (Or worse: it makes something up)
```

### 3. No Memory Between Conversations

Every conversation starts completely fresh. The model does not remember you from yesterday.

```
Session 1: "My name is Ahmed."
LLM: "Nice to meet you, Ahmed!"

[New conversation]
You: "What's my name?"
LLM: "I don't know your name. Could you remind me?"
```

*This is why adding memory to agents (Module 7) matters so much.*

### 4. Cannot Access Private or External Data

The LLM only knows what was in its public training data. It has no access to:
- Your company's documents
- Your personal files
- Live databases or APIs
- Real-time information

### 5. Context Window Limits

Even with large context windows, you can't just paste your entire 500-page manual and expect great results. The "lost in the middle" problem is real — LLMs pay more attention to the beginning and end of context.

---

## 🗣️ Anatomy of an LLM Request

When you talk to an LLM via API, there are typically three types of messages:

```python
messages = [
    {
        "role": "system",
        "content": "You are a helpful assistant that answers questions 
                    only about Egyptian history."
    },
    {
        "role": "user", 
        "content": "Who built the pyramids?"
    },
    {
        "role": "assistant",
        "content": "The ancient Egyptians built the pyramids..."
    },
    {
        "role": "user",
        "content": "When were they built?"
    }
]
```

- **System** — Instructions for how the AI should behave (tone, persona, rules)
- **User** — What the human says
- **Assistant** — What the AI previously said (for multi-turn conversation)

The entire history is sent with every request — that's how the LLM appears to "remember" the conversation. It's re-reading everything every time.

---

## 🌡️ Temperature — How Creative vs Predictable

Every LLM has a **temperature** parameter that controls randomness:

```
Temperature = 0.0  → Always picks the most probable next token
                      Deterministic, consistent, "safe"
                      Best for: facts, code, data extraction

Temperature = 0.7  → Balanced (most common default)
                      Good mix of accuracy and variety
                      Best for: general Q&A, summaries

Temperature = 1.5  → Very random, creative, unpredictable  
                      Best for: brainstorming, creative writing

Temperature > 2.0  → Often incoherent gibberish
```

**For RAG systems, use temperature 0 or close to it.** You want factual, consistent answers — not creative interpretations.

---

## 🏷️ Popular LLMs You'll Encounter

| Model | Made By | Notes |
|-------|---------|-------|
| GPT-4o | OpenAI | Fast, multimodal (text + images) |
| Claude 3.5 Sonnet | Anthropic | Strong at reasoning and following instructions |
| Gemini 1.5 Pro | Google | Massive context window, multimodal |
| Llama 3.1 405B | Meta | Open source, can run on your own hardware |
| Mistral Large | Mistral AI | Strong open-source option |
| Qwen 2.5 | Alibaba | Excellent multilingual support |

---

## 🔁 How LLMs Connect to RAG

The LLM is the **final step** in a RAG pipeline — it takes the retrieved context and generates a human-readable answer. But without RAG, it:

- Doesn't know your private documents ❌
- Might hallucinate facts ❌
- Can't access recent information ❌

With RAG, you feed it the relevant context and tell it: *"Answer based only on this."* The LLM's job becomes much easier and much more accurate.

```
Without RAG:
  User: "What is our refund policy?"
  LLM: *makes up a plausible-sounding policy* ❌

With RAG:
  Retrieve: [Actual refund policy text from your docs]
  LLM: "Based on our policy: refunds are available within 30 days..." ✅
```

---

## ✅ Full Recap

| Term | Definition |
|------|-----------|
| **LLM** | A large AI trained to predict the next token |
| **Pre-training** | Training on massive internet text |
| **Fine-tuning** | Additional training to make models helpful/safe |
| **RLHF** | Learning from human preference ratings |
| **Parameters / Weights** | The internal numbers adjusted during training |
| **Hallucination** | When an LLM confidently makes up false info |
| **Knowledge cutoff** | The date after which the model knows nothing |
| **System prompt** | Instructions telling the LLM how to behave |
| **Temperature** | Controls randomness in the LLM's output |

---

> ➡️ **Next:** [Module 3 — Prompt Engineering](./03_prompt_engineering.md)
