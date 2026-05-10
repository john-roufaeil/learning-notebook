# 📘 Module 5: What is RAG? — The Complete Picture

> **Course:** Build a RAG System from Scratch
> **Prev:** [Module 4 ← Vector Databases](./04_vector_databases.md)
> **Next:** [Module 6 → Fine-Tuning vs RAG](./06_finetuning_vs_rag.md)

---

## 🎯 The Problem RAG Solves

You have valuable private documents — company reports, product manuals, legal contracts, research papers, your own notes. You want to ask questions about them in natural language and get accurate answers.

**Without RAG, you have a problem at every option:**

| Option | Problem |
|--------|---------|
| Just ask the LLM | It has never seen your documents. It will hallucinate. |
| Paste the whole doc in the prompt | Hits context window limits. Expensive. Quality drops. |
| Train a new AI from scratch | Costs millions of dollars and months of time. |
| Fine-tune the LLM on your docs | Expensive, slow, still hallucination-prone for specific facts. |

**RAG is the practical solution:**
- No retraining
- No context limit issues (sends only relevant chunks)
- Always cites actual document content
- Easy to update (just add new documents)
- Works with any LLM

---

## 📖 RAG = Retrieval-Augmented Generation

**Breaking it down:**

```
Retrieval   → Find the most relevant chunks from your documents
Augmented   → Add those chunks to the LLM's prompt as context
Generation  → The LLM reads the context and generates an answer
```

> 💡 Open-book exam analogy: Instead of the LLM having to *memorize* your documents (fine-tuning), it gets to *look them up* at exam time (RAG). The LLM still needs to be smart enough to read and interpret — but it doesn't need to memorize every fact.

---

## 🔄 The Full RAG Pipeline

### Phase 1: Ingestion (done once, or when documents change)

```
Your Documents (PDFs, Word, text, web pages...)
         │
         ▼ Load
   Raw text content extracted
         │
         ▼ Chunk
   Split into 300–800 character pieces with overlap
         │
         ▼ Embed
   Each chunk → vector [0.23, -0.14, 0.87, ...]
         │
         ▼ Store
   Vector + text + metadata → Vector Database
```

### Phase 2: Retrieval & Generation (every time a user asks)

```
User Question: "What was our Q3 revenue?"
         │
         ▼ Embed the question
   [0.21, -0.11, 0.84, ...]
         │
         ▼ Similarity search in vector DB
   Top 4 matching chunks retrieved:
   - "Q3 revenue was $4.2M..."
   - "Growth was driven by enterprise..."
   - "Compared to Q2's $3.5M..."
   - "New clients signed in Q3..."
         │
         ▼ Build prompt
   System: "Answer based on this context: [4 chunks]"
   User:   "What was our Q3 revenue?"
         │
         ▼ LLM generates answer
   "Based on the Q3 report, revenue reached $4.2 million,
    representing an 18% increase over Q2's $3.5 million,
    driven primarily by new enterprise client acquisitions."
         │
         ▼ (Optional) cite sources
   Sources: company_report_2024.pdf, pages 12-13
```

---

## 🏗️ The RAG Prompt Template in Detail

The prompt you send to the LLM after retrieval typically looks like this:

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SYSTEM MESSAGE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
You are a helpful assistant. Answer the user's question 
using ONLY the provided context. If the answer is not 
in the context, say "I don't have that information."
Never fabricate facts, statistics, or citations.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
RETRIEVED CONTEXT  (injected dynamically)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
[Source: company_report_2024.pdf, page 12]
Q3 2024 revenue reached $4.2M, an 18% increase over Q2.

[Source: company_report_2024.pdf, page 13]  
The growth was primarily driven by enterprise sales...

[Source: meeting_notes_oct2024.txt]
Three Fortune 500 clients signed contracts in Q3...

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
USER MESSAGE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
What was our Q3 revenue and what drove the growth?
```

---

## 🏆 Advanced RAG Patterns

Basic RAG is just: retrieve → generate. But smarter RAG systems add more steps:

### Query Rewriting
*Fix vague or poorly worded questions before searching*

```
User types:   "what about last quarter numbers?"
              ↓ LLM rewrites it to:
Better query: "What were the financial results for Q3 2024?"
              ↓
              Now search the vector DB with the better query
```

### HyDE (Hypothetical Document Embeddings)
*Generate a fake answer, embed that, search with it*

```
User: "What is the refund policy?"
      ↓ Generate a hypothetical answer:
LLM:  "Products can be returned within 30 days for a full refund..."
      ↓ Embed the hypothetical answer (not the question!)
      ↓ Search for chunks similar to this hypothetical
```

Why? Because the hypothetical answer *sounds like the documents* — it uses document-style language. The question uses user-style language. Embedding the hypothetical gives better semantic matches.

### Multi-Query Retrieval
*Generate multiple versions of the question, search for all, combine results*

```
Original: "What is machine learning?"
Generated versions:
  - "How does machine learning work?"
  - "Define machine learning"
  - "Explain ML to a beginner"

Search all 3 → combine unique results → better coverage
```

### Contextual Compression
*After retrieving chunks, ask an LLM to extract ONLY the relevant sentences*

```
Chunk retrieved (500 chars):
"The company was founded in 2010. Revenue grew steadily. 
In Q3 2024, revenue hit $4.2M, up 18%. The CEO joined in 2019.
The main office is in Cairo. New products launched this year."

User question: "What was Q3 revenue?"

After compression (only 1 sentence kept):
"In Q3 2024, revenue hit $4.2M, up 18%."
```

Now the LLM only sees the actually relevant sentence — less noise, better answer.

### Self-RAG
*The LLM decides whether it even needs to retrieve, and checks its own answer*

```
User: "What is 2 + 2?"
LLM:  [Decides: this is a math question, no retrieval needed]
      "4"

User: "What was our Q3 revenue?"
LLM:  [Decides: this requires specific document data, RETRIEVE]
      [Retrieves chunks]
      [Generates answer]
      [Self-checks: "Is my answer supported by the retrieved text?"]
      [If yes → return. If no → retry or say "I don't know"]
```

---

## 🔢 How Many Chunks to Retrieve? (k)

**k** is the number of chunks returned by the similarity search. Choosing k is a real design decision:

| k value | Pros | Cons |
|---------|------|------|
| k=2 | Fast, focused | Might miss relevant info |
| k=4–6 | Good balance | Standard recommendation |
| k=10+ | Very comprehensive | Fills context window, can confuse LLM |

**Rule of thumb:** Start with k=4. Increase if answers are too incomplete. Decrease if answers are unfocused or the LLM seems confused.

Also consider: what's your context window budget? 
- Each chunk ≈ 150 tokens
- k=4 chunks ≈ 600 tokens for context
- That leaves plenty of room for question + answer

---

## 🌍 Real-World RAG Use Cases

| Industry | Use Case | Documents |
|----------|---------|-----------|
| **Customer Support** | AI answers customer questions | Product manuals, FAQs, policies |
| **Legal** | Search through contracts and case law | Contracts, court rulings, statutes |
| **Healthcare** | Clinical decision support | Drug databases, treatment guidelines |
| **Finance** | Earnings call Q&A | Annual reports, SEC filings |
| **Education** | Tutoring from specific textbooks | Course materials, lecture notes |
| **HR** | Employee policy assistant | HR handbooks, company policies |
| **Engineering** | Code documentation assistant | API docs, architecture docs |

---

## ✅ Full Recap

| Term | Definition |
|------|-----------|
| **RAG** | Retrieval-Augmented Generation — AI that looks up docs before answering |
| **Ingestion** | One-time process of loading, chunking, embedding, storing docs |
| **Retrieval** | Finding the most relevant chunks for a given query |
| **Augmentation** | Adding retrieved chunks to the LLM's prompt as context |
| **Generation** | The LLM producing a final answer using the context |
| **k** | Number of chunks to retrieve |
| **Query Rewriting** | Improving the user's question before searching |
| **HyDE** | Searching with a hypothetical answer instead of the question |
| **Multi-query** | Running multiple variations of the question and combining results |
| **Self-RAG** | LLM that checks its own output and decides when to retrieve |

---

> ➡️ **Next:** [Module 6 — Fine-Tuning vs RAG](./06_finetuning_vs_rag.md)
