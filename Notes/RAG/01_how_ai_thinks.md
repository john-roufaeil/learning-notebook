# 📘 Module 1: How AI Thinks — Tokens, Embeddings & Vectors

> **Course:** Build a RAG System from Scratch
> **Next:** [Module 2 → What is an LLM?](./02_what_is_an_llm.md)

---

## 🧠 The Fundamental Truth

Computers speak one language: **numbers**.

Not words. Not sentences. Not ideas. Just numbers.

So for a computer to understand *"The cat sat on the mat"*, it must first convert every part of that sentence into numbers. This whole process — from raw text to numbers — is what we explore in this module.

---

## 🔤 Step 1: Tokenization

Before turning words into numbers, the AI breaks text into small pieces called **tokens**.

A token is not always a full word. It could be:
- A full word: `"cat"` → 1 token
- Part of a word: `"unbelievable"` → `["un", "believ", "able"]` → 3 tokens
- A punctuation mark: `"!"` → 1 token
- A space + word: `" the"` → 1 token

**Why not just split by spaces?**

Because new words, names, and rare words would completely break the system. By splitting into smaller pieces, the AI can handle almost anything — even words it has never seen before — because their *parts* are familiar.

**Example:**

```
Input: "ChatGPT is unbelievable!"

Tokens: ["Chat", "G", "PT", " is", " un", "believ", "able", "!"]
```

Each token gets a **Token ID** — just a number in a giant dictionary:

```
"Chat"    → 9474
"G"       → 38
"PT"      → 2898
" is"     → 318
" un"     → 555
"believ"  → 12102
"able"    → 540
"!"       → 0
```

Now the model has numbers to work with. But these IDs alone don't carry *meaning* — the number `9474` doesn't tell you anything about how "Chat" relates to "talk" or "message". That's what embeddings do.

---

## 🔢 Step 2: Embeddings — Meaning as Numbers

An **embedding** converts a word or sentence into a **list of numbers** (called a **vector**). The magic property: **similar meanings produce similar vectors**.

**Super simplified 2D example:**

```
"king"    → [0.90, 0.80]
"queen"   → [0.80, 0.90]   ← close to king (both royalty)
"man"     → [0.60, 0.20]
"woman"   → [0.50, 0.30]   ← close to man (both people)
"pizza"   → [0.05, 0.08]   ← far from all of them
```

In reality, embeddings have **hundreds or thousands of dimensions**, not just 2. This lets the model encode incredibly nuanced meaning — grammar, sentiment, topic, context.

**The famous analogy test:**

```
vector("king") - vector("man") + vector("woman") ≈ vector("queen")
```

This actually works mathematically. The AI has learned that the *difference* between king and queen is the same as the difference between man and woman. Concepts are encoded as directions in this mathematical space.

**Who creates embeddings?**

Special AI models called **embedding models**, trained specifically for this task:

| Model | Dimensions | Cost | Notes |
|-------|-----------|------|-------|
| `text-embedding-3-small` (OpenAI) | 1,536 | Paid, cheap | Great default |
| `text-embedding-3-large` (OpenAI) | 3,072 | Paid, moderate | More accurate |
| `all-MiniLM-L6-v2` (HuggingFace) | 384 | Free | Runs locally |
| `nomic-embed-text` | 768 | Free | Open source, strong |
| `bge-large-en` | 1,024 | Free | Top open source |

---

## 📐 Step 3: Similarity — How Close Are Two Vectors?

Once you have vectors, you can measure how *similar* two pieces of text are mathematically. The most common method is **cosine similarity**.

You do not need to understand the math. Just know the scale:

```
Cosine similarity = 1.0  → identical meaning
Cosine similarity = 0.5  → somewhat related
Cosine similarity = 0.0  → completely unrelated
Cosine similarity =-1.0  → opposite meaning
```

**Real example:**

```
"What is machine learning?"  vs  "How does ML work?"
→ Cosine similarity: 0.91   (very similar — same question, different wording)

"What is machine learning?"  vs  "What should I cook for dinner?"
→ Cosine similarity: 0.07   (totally unrelated)
```

This is the engine that powers **semantic search** in RAG. Instead of looking for keyword matches, you find chunks whose *meaning* is closest to the user's question. A question about "revenue" will find chunks about "earnings" and "income" even if those exact words weren't in the query.

---

## 🪟 Step 4: The Context Window

Every LLM can only process a limited amount of text at once. This limit is called the **context window**, measured in tokens.

Think of it like working memory. You can hold a certain amount in your head at once. Past that limit, earlier content gets dropped.

```
GPT-3.5 Turbo  →    4,096 tokens  ≈    3,000 words
GPT-4o         →  128,000 tokens  ≈   96,000 words  (≈ a short novel)
Claude 3.5     →  200,000 tokens  ≈  150,000 words
Gemini 1.5 Pro →1,000,000 tokens  ← one million!
```

**Why does this matter for RAG?**

Even with a massive context window, you should not dump an entire document library into every prompt because:

1. **Cost** — APIs charge per token. 100k tokens × 1000 users = expensive fast
2. **Quality drops** — Research shows LLMs actually perform *worse* when given too much irrelevant context ("lost in the middle" problem)
3. **Speed** — More tokens = slower response

RAG solves this by sending *only the relevant 3–5 chunks* instead of everything.

---

## ✂️ Step 5: Chunking — Splitting Documents for RAG

Before storing documents in a vector database, you split them into smaller pieces called **chunks**. This is one of the most important design decisions in a RAG system.

**Why chunk?**

- Embedding models work best on short focused text
- You can retrieve *specific* relevant sections, not entire 50-page PDFs
- Smaller chunks = more precise semantic search

**The chunking tradeoff:**

```
Too small (< 100 characters):
  "Revenue grew."
  → No context. Who? What revenue? Useless alone.

Too large (> 2,000 characters):
  [Three full pages of text]
  → Hard to search precisely, floods the context window

Sweet spot (300–800 characters):
  "Q3 2024 revenue reached $4.2M, an 18% increase over Q2 2024.
   Growth was driven by enterprise client acquisitions, including 
   three Fortune 500 companies signing multi-year contracts."
  → Specific, contextually complete, searchable ✅
```

**Overlap — don't lose information at the seams:**

```
Chunk 1: "...Revenue grew 18% in Q3. The main drivers were enterprise
           sales and the launch of the new product line..."
                                    ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
Chunk 2:                           "The main drivers were enterprise
                                    sales and the launch of the new 
                                    product line. Three Fortune 500..."
```

The overlap ensures a query about "product line launch" hits either chunk.

**Chunking strategies:**

| Strategy | How it works | Best for |
|----------|-------------|---------|
| **Fixed-size** | Cut every N characters | Fast, simple |
| **Recursive** | Try `\n\n`, then `\n`, then `.`, then ` ` | Best default |
| **Semantic** | Split where meaning changes (uses LLM to detect) | Highest quality |
| **By heading** | Split at markdown/HTML headers | Structured docs |
| **Sentence-level** | Split at sentence boundaries | Legal, academic text |

---

## 🔁 How It All Fits Together

```
Your Document (raw text)
         │
         ▼  Chunking
  ┌──────────────────────┐
  │ Chunk 1  │ Chunk 2   │  Chunk 3  │ ...
  └──────────────────────┘
         │
         ▼  Embedding Model
  Each chunk → vector of 1,536 numbers
  [0.23, -0.14, 0.87, 0.03, ..., 0.61]
         │
         ▼  Vector Database
  Stored with original text + metadata
         │
  ─── Later, user asks a question ───
         │
         ▼  Embed the question
  "What was Q3 revenue?" → [0.21, -0.11, 0.84, ...]
         │
         ▼  Similarity Search
  Find closest vectors → retrieve Chunk 2
         │
         ▼  Retrieved!
  "Q3 2024 revenue reached $4.2M..."  ✅
```

---

## ✅ Full Glossary for This Module

| Term | Definition |
|------|-----------|
| **Token** | Smallest unit of text the AI processes |
| **Tokenization** | Breaking text into tokens |
| **Token ID** | The number assigned to a token in a dictionary |
| **Embedding** | A list of numbers encoding the *meaning* of text |
| **Vector** | A list of numbers (same as embedding output) |
| **Dimensions** | How many numbers are in a vector |
| **Cosine Similarity** | Score from -1 to 1 measuring how similar two vectors are |
| **Context Window** | Max tokens an LLM can read at once |
| **Chunking** | Splitting documents into smaller, embeddable pieces |
| **Chunk Overlap** | Shared text at the boundary between adjacent chunks |
| **Embedding Model** | AI model that converts text → vectors |

---

> ➡️ **Next:** [Module 2 — What is an LLM?](./02_what_is_an_llm.md)
