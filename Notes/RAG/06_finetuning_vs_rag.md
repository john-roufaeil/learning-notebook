# 📘 Module 6: Fine-Tuning vs RAG — When to Use What

> **Course:** Build a RAG System from Scratch
> **Prev:** [Module 5 ← What is RAG?](./05_what_is_rag.md)
> **Next:** [Module 7 → LangChain Basics](./07_langchain_basics.md)

---

## 🤔 The Big Question

You have private data and you want an AI to be smart about it. Two main paths exist:

- **Fine-Tuning** — retrain the LLM on your data
- **RAG** — give the LLM your data at query time

They sound like they do the same thing. But they're fundamentally different approaches with very different trade-offs. Choosing the wrong one can waste months of work or thousands of dollars.

---

## 🧠 What is Fine-Tuning? (Deep Dive)

**Fine-tuning** means taking a pre-trained LLM and training it *further* on your specific dataset. You are literally changing the model's internal numbers (weights) to embed new knowledge or behaviors.

### The Analogy

Imagine hiring a brilliant but generalist consultant (the base LLM). They know everything. But they don't know how your specific company works.

- **Fine-tuning** = sending them to a 3-month intensive training course about your company. After that, it's *baked into them*.
- **RAG** = giving them a company handbook they can reference whenever needed. They stay general but look things up.

### How Fine-Tuning Works

```
Start: A pre-trained LLM (e.g., Llama 3)
            │
            ▼
Your training data (hundreds to millions of examples):
  Input: "What is our refund policy?"
  Output: "Our refund policy allows returns within 30 days..."

  Input: "Summarize this customer complaint: [complaint]"
  Output: "[ideal summary style your company uses]"

  Input: "Write a support ticket response for: [issue]"
  Output: "[formal, branded response in your company's voice]"
            │
            ▼
Training loop:
  Feed example → model predicts output → compare to correct output
  → adjust weights slightly → repeat thousands of times
            │
            ▼
Fine-tuned model: now "baked in" with your style/knowledge
```

### Types of Fine-Tuning

| Type | What changes | Cost | Use case |
|------|-------------|------|---------|
| **Full fine-tuning** | All model parameters | Very expensive | Rare, massive datasets |
| **LoRA** | Small adapter layers only | Moderate | Most common approach |
| **QLoRA** | LoRA + quantization | Cheap, runs on consumer GPU | Open-source models |
| **Instruction tuning** | Teach new response style | Moderate | Custom personas/formats |
| **RLHF** | Add human preference feedback | Expensive | ChatGPT-style alignment |

**LoRA (Low-Rank Adaptation)** is by far the most popular today — it freezes most of the model and only trains small "adapter" layers, making it 10-100x cheaper than full fine-tuning while achieving similar results.

### What Fine-Tuning is Good At

- ✅ Teaching the model a **new writing style** or tone
- ✅ Teaching the model **how to format** outputs (always respond as JSON, etc.)
- ✅ Making it fluent in **domain-specific language** (medical, legal, coding jargon)
- ✅ Teaching it to **follow specific instructions** reliably
- ✅ Reducing hallucination on **narrow, well-defined tasks**
- ✅ **Smaller, faster model** that beats a bigger general model on your specific task

### What Fine-Tuning is Bad At

- ❌ Giving the model access to **specific private facts** (it still hallucinates)
- ❌ Keeping knowledge **up to date** (you'd need to retrain constantly)
- ❌ **Citing sources** (it can't tell you where it learned something)
- ❌ Quick iteration — **takes days/weeks** to train and evaluate
- ❌ **Cost** — even with LoRA, training + GPU time adds up

> 🚨 Common misconception: "If I fine-tune the LLM on my documents, it will always give accurate facts from those documents."
>
> This is **wrong**. Fine-tuning on facts is unreliable — the model still hallucinates. Fine-tuning teaches *behavior*, not reliable *fact retrieval*. RAG is the right tool for factual accuracy.

---

## 🔍 What is RAG? (Recap in Comparison Context)

RAG doesn't change the model at all. Instead, it changes the *input* to the model at runtime:

```
Every query:
  User question
      │
      ▼
  Search your documents → retrieve relevant chunks
      │
      ▼
  Feed model: "Here is some context [chunks]. Answer this: [question]"
      │
      ▼
  Model reads context + generates answer
```

The model stays the same. Only the context changes per query.

---

## ⚔️ The Full Comparison

| Dimension | RAG | Fine-Tuning |
|-----------|-----|-------------|
| **Changes the model?** | No | Yes |
| **Setup time** | Hours | Days to weeks |
| **Cost** | Low (API calls only) | Medium–High (GPU training) |
| **Data required** | Just documents | Hundreds to thousands of Q&A pairs |
| **Update when data changes** | Just add new docs to DB | Retrain the model |
| **Accurate on specific facts** | ✅ Very good | ❌ Unreliable (still hallucinates) |
| **Can cite sources** | ✅ Yes | ❌ No |
| **Custom writing style** | ⚠️ Partial (via prompt) | ✅ Deeply learned |
| **Domain-specific vocabulary** | ⚠️ Partial | ✅ Deeply learned |
| **Works at scale (many docs)** | ✅ Yes | ❌ Can't scale infinitely |
| **Private data stays private** | ✅ (docs on your server) | ⚠️ Training data may be sent to provider |
| **No internet dependency** | ❌ Needs embedding API | ✅ Model runs locally after training |
| **Explainability** | ✅ Can show which chunk was used | ❌ Black box |

---

## 🎯 Decision Framework: Which to Choose?

Ask these questions:

```
Q1: Do I need the AI to know SPECIFIC FACTS from my documents?
    → Yes → RAG
    → No  → maybe Fine-Tuning

Q2: Does my data change frequently (weekly, monthly)?
    → Yes → RAG (just add docs)
    → No  → Either works

Q3: Do I need to cite sources or explain where answers come from?
    → Yes → RAG

Q4: Do I need the AI to respond in a very specific style/format/persona?
    → Yes, deeply → Fine-Tuning
    → Yes, somewhat → Prompt engineering first, fine-tune if needed

Q5: Is my budget under $500?
    → Yes → RAG

Q6: Do I have fewer than 1,000 Q&A training examples?
    → Yes → RAG (fine-tuning needs data)

Q7: Do I need to reduce latency or API costs at massive scale?
    → Fine-tuning a smaller model may be cheaper per query at huge scale
```

---

## 🤝 The Best Answer: Use Both Together

In production, the best systems often combine RAG + Fine-Tuning:

```
Fine-tune the model to:
  - Use the right tone and format
  - Follow company-specific instructions reliably
  - Understand domain jargon

+

RAG to:
  - Retrieve current, specific facts
  - Cite sources
  - Handle knowledge that changes
```

**Example:**

> A legal firm trains a fine-tuned model that *speaks legalese*, understands legal document structure, and formats output consistently. Then they add RAG on top so it can retrieve specific case law, contract clauses, and client files.

---

## 💰 Cost Reality Check

**RAG costs:**
- Embedding your docs: ~$0.10 per million tokens (one-time)
- Vector DB: Free (ChromaDB) to ~$70/month (Pinecone starter)
- LLM API calls: ~$0.01–0.10 per query depending on model

**Fine-tuning costs:**
- Data preparation: Hours to weeks of human time
- LoRA fine-tuning on a 7B model: ~$50–200 for a training run (cloud GPU)
- Fine-tuning GPT-4: Currently not available; GPT-3.5 fine-tune ≈ $8 per million training tokens
- Hosting your fine-tuned model: $50–500/month depending on GPU needed

**For most use cases, RAG is 10–100x cheaper to start with.**

---

## 📊 Other Approaches (The Full Spectrum)

There are actually more than just RAG and fine-tuning:

```
Increasing customization →→→→→→→→→→→→→→→→→→→→→→→→→→→→→

Prompt       →  RAG     →  Fine-Tuning  →  Train from scratch
Engineering                              
                                         
Cheapest    ←──────────────────────────────→  Most Expensive
Fastest     ←──────────────────────────────→  Slowest
No data     ←──────────────────────────────→  Lots of data needed
```

| Approach | When to use |
|----------|-------------|
| **Prompt engineering** | First thing to try. Gets you 80% of the way. |
| **RAG** | Need the LLM to know specific private/recent facts. |
| **Fine-tuning** | Need specific behavior, style, or domain language. Prompt alone isn't enough. |
| **Train from scratch** | Only for massive organizations (Google, OpenAI, Meta). Costs millions. |

---

## ✅ Full Recap

| Term | Definition |
|------|-----------|
| **Fine-tuning** | Retraining an LLM on your data to change its behavior/knowledge |
| **LoRA** | Efficient fine-tuning that only trains small adapter layers |
| **RAG** | Retrieval system that feeds relevant docs to the LLM at query time |
| **Base model** | The original pre-trained LLM before any customization |
| **Hallucination** | LLM making up facts (fine-tuning does not reliably prevent this) |
| **Instruction tuning** | Fine-tuning to follow instructions in a specific way |
| **Hybrid** | Using both RAG and fine-tuning together for best results |

---

> ➡️ **Next:** [Module 7 — LangChain Basics](./07_langchain_basics.md)
