# 📘 Module 10: Evaluation & Improvement — Making Your RAG Better

> **Course:** Build a RAG System from Scratch
> **Prev:** [Module 9 ← Build Your RAG System](./09_build_your_rag.md)

---

## 🤔 How Do You Know If Your RAG Is Actually Good?

You built it. It gives answers. But are those answers correct? Relevant? Based on real document content? Comprehensive enough?

You can't improve what you can't measure. This module gives you the tools to measure and improve every part of your RAG system.

---

## 🔬 The Two Stages That Can Fail

RAG has two independent stages, and each can fail independently:

```
Stage 1: Retrieval   → Did we get the RIGHT chunks?
Stage 2: Generation  → Did the LLM use those chunks CORRECTLY?
```

If Stage 1 fails (wrong chunks retrieved), Stage 2 can't save you.
If Stage 1 succeeds but Stage 2 fails (LLM hallucinates), you still get a bad answer.

You need to evaluate BOTH separately.

---

## 📊 The 4 Key RAG Metrics

### Metric 1: Context Precision
*Are the retrieved chunks actually relevant to the question?*

```
Question: "What is the refund policy?"

Retrieved chunks:
  Chunk A: "Refunds are accepted within 30 days."         ← relevant ✅
  Chunk B: "Our CEO joined the company in 2019."          ← not relevant ❌
  Chunk C: "Products must be in original packaging."      ← relevant ✅

Precision = 2 relevant / 3 retrieved = 0.67 (67%)
```

High precision = you're not retrieving noise.

---

### Metric 2: Context Recall
*Did you find ALL the relevant information that exists in your documents?*

```
Your documents actually contain 5 chunks about the refund policy.
You retrieved 3 of them.

Recall = 3 found / 5 total = 0.60 (60%)
```

High recall = you're not missing important information.

Note: Precision and Recall trade off against each other. Retrieving more chunks (higher k) usually increases recall but decreases precision.

---

### Metric 3: Faithfulness
*Is the answer actually based on the retrieved context, or did the LLM make things up?*

```
Context says:  "Refunds accepted within 30 days of purchase."

Good answer:   "You can get a refund within 30 days."          ✅ faithful
Bad answer:    "You can get a refund within 60 days."          ❌ hallucinated
Worse answer:  "Refunds depend on the product type and region" ❌ not in context
```

Faithfulness = what percentage of claims in the answer can be traced to the context.

---

### Metric 4: Answer Relevance
*Does the answer actually address what the user asked?*

```
Question: "What is the refund policy?"

Relevant:     "Products can be returned within 30 days for a full refund." ✅
Irrelevant:   "The company was founded in Cairo in 2015."                  ❌
Partially:    "Our customer service team is very helpful."                 ⚠️
```

---

## 🛠️ Automated Evaluation with RAGAS

**RAGAS** is the standard library for evaluating RAG systems. It uses an LLM as a judge to score your system automatically.

```bash
pip install ragas datasets
```

```python
from ragas import evaluate
from ragas.metrics import (
    faithfulness,
    answer_relevancy,
    context_precision,
    context_recall
)
from datasets import Dataset

# Build a test dataset
# You need: questions, your system's answers, retrieved contexts, and ground truth answers
test_data = {
    "question": [
        "What is the refund policy?",
        "Who is the CEO?",
        "What was Q3 revenue?"
    ],
    "answer": [
        "Products can be returned within 30 days.",
        "Ahmed Hassan has been CEO since 2019.",
        "Q3 revenue was $4.2 million."
    ],
    "contexts": [
        ["Refunds are accepted within 30 days of purchase in original packaging."],
        ["Ahmed Hassan joined as CEO in January 2019.", "He previously worked at Google."],
        ["Q3 2024 revenue reached $4.2M, an 18% increase over Q2."]
    ],
    "ground_truth": [
        "The refund policy allows returns within 30 days.",
        "The CEO is Ahmed Hassan.",
        "Revenue in Q3 2024 was $4.2 million."
    ]
}

dataset = Dataset.from_dict(test_data)

results = evaluate(
    dataset,
    metrics=[faithfulness, answer_relevancy, context_precision, context_recall]
)

print(results)
```

**Example output:**
```
{
  'faithfulness':      0.95,   ← 95% of claims grounded in context
  'answer_relevancy':  0.88,   ← 88% of answers actually address the question
  'context_precision': 0.83,   ← 83% of retrieved chunks were relevant
  'context_recall':    0.76    ← found 76% of all relevant information
}
```

**Scores to aim for:** All metrics above 0.80 is a well-functioning RAG system.

---

## 🔧 Diagnosis and Fixes

### Problem: Low Context Precision (retrieving irrelevant chunks)

**Diagnosis:** The retrieved chunks often don't match the question.

**Fixes:**

```python
# Fix 1: Use MMR for diverse retrieval
retriever = vectorstore.as_retriever(
    search_type="mmr",
    search_kwargs={"k": 5, "fetch_k": 20, "lambda_mult": 0.7}
)

# Fix 2: Reduce k (fewer but better chunks)
retriever = vectorstore.as_retriever(search_kwargs={"k": 3})

# Fix 3: Add a relevance threshold filter
results = vectorstore.similarity_search_with_score(query, k=10)
good_results = [(doc, score) for doc, score in results if score < 1.2]

# Fix 4: Use better embedding model
embeddings = OpenAIEmbeddings(model="text-embedding-3-large")  # More accurate
```

---

### Problem: Low Context Recall (missing relevant chunks)

**Diagnosis:** The answer is incomplete because relevant info wasn't retrieved.

**Fixes:**

```python
# Fix 1: Increase k
retriever = vectorstore.as_retriever(search_kwargs={"k": 8})

# Fix 2: Smaller chunks so retrieval is more precise
splitter = RecursiveCharacterTextSplitter(
    chunk_size=300,   # Smaller chunks
    chunk_overlap=50
)

# Fix 3: MultiQuery retriever (generate multiple queries)
from langchain.retrievers import MultiQueryRetriever
multi_retriever = MultiQueryRetriever.from_llm(retriever=retriever, llm=llm)

# Fix 4: Query rewriting before retrieval
def rewrite_query(question: str) -> str:
    return llm.invoke(
        f"Rephrase for document search. Be specific. Original: {question}"
    ).content
```

---

### Problem: Low Faithfulness (LLM hallucinating)

**Diagnosis:** The LLM is going beyond the context or making things up.

**Fixes:**

```python
# Fix 1: Stricter system prompt
STRICT_PROMPT = """You are a precise assistant. 
RULES (never break these):
- Use ONLY the provided excerpts to answer
- If information is not in the excerpts, say "I don't have that information"
- Quote or closely paraphrase the excerpts rather than paraphrasing loosely
- Never add context from your training data
- If you're unsure, say so explicitly

Excerpts:
{context}"""

# Fix 2: Lower temperature
llm = ChatOpenAI(model="gpt-4o", temperature=0)  # Deterministic

# Fix 3: Ask the LLM to cite its source
CITATION_PROMPT = """Answer the question and for each claim, 
add [Excerpt N] to show which excerpt it came from.

Excerpts:
{context}

Question: {question}"""
```

---

### Problem: Low Answer Relevance (answering a different question)

**Diagnosis:** The answer drifts off-topic or misunderstands the question.

**Fixes:**

```python
# Fix 1: Include the original question at the end of the prompt (recency bias)
prompt = f"""Context: {context}

Question: {question}

Important: Answer specifically and only what was asked: {question}
Answer:"""

# Fix 2: Output format specification
prompt = f"""Context: {context}

Question: {question}

Provide a direct answer in 1-3 sentences. 
Start your response with a direct answer to the question."""

# Fix 3: Post-generation relevance check
def check_answer_relevance(question: str, answer: str) -> bool:
    check = llm.invoke(
        f"Does this answer actually answer the question?\n"
        f"Question: {question}\nAnswer: {answer}\n"
        f"Reply only: yes or no"
    )
    return "yes" in check.content.lower()
```

---

### Problem: Slow Responses

**Diagnosis:** Retrieval or generation takes too long.

**Fixes:**

```python
# Fix 1: Reduce k
retriever = vectorstore.as_retriever(search_kwargs={"k": 3})

# Fix 2: Use a faster/cheaper model for simple questions
from langchain_openai import ChatOpenAI
fast_llm = ChatOpenAI(model="gpt-4o-mini", temperature=0)  # Much cheaper and faster

# Fix 3: Cache repeated queries
from langchain.cache import InMemoryCache
from langchain.globals import set_llm_cache
set_llm_cache(InMemoryCache())  # Same question = instant response

# Fix 4: Stream the response so users see output immediately
for chunk in rag_chain.stream(question):
    print(chunk, end="", flush=True)
```

---

## 🗺️ The RAG Improvement Cheat Sheet

| Symptom | Root Cause | Fix |
|---------|-----------|-----|
| Wrong chunks retrieved | Embedding mismatch | Better embedding model, smaller chunks |
| Missing relevant chunks | k too low, chunks too big | Increase k, smaller chunks, multi-query |
| LLM makes things up | Weak prompt, high temperature | Strict prompt, temperature=0 |
| LLM ignores context | Context too long, distracting | Reduce k, contextual compression |
| Off-topic answers | Question misunderstood | Repeat question in prompt, query rewriting |
| Slow responses | Too many chunks, large model | Reduce k, faster model, caching |
| Works for one question, not another | Inconsistent chunking | Semantic chunking, overlap tuning |
| Can't handle follow-up questions | No conversation memory | Add MemorySaver with thread IDs |

---

## 🧪 Build a Test Suite

The best way to continuously improve your RAG is to maintain a test suite:

```python
# tests/rag_tests.py
import json

TEST_CASES = [
    {
        "question": "What is the refund policy?",
        "expected_keywords": ["30 days", "return", "refund"],
        "should_find_answer": True
    },
    {
        "question": "What is the weather like today?",
        "expected_keywords": [],
        "should_find_answer": False  # Should say "I don't know"
    },
    {
        "question": "Who is the CEO?",
        "expected_keywords": ["Ahmed Hassan"],
        "should_find_answer": True
    }
]

def run_tests(chain):
    results = []
    for test in TEST_CASES:
        answer = chain.invoke(test["question"])
        
        # Check if keywords are present
        keywords_found = all(
            kw.lower() in answer.lower() 
            for kw in test["expected_keywords"]
        )
        
        # Check if "I don't know" appears when it should
        has_answer = "don't have" not in answer.lower()
        answer_correct = (has_answer == test["should_find_answer"])
        
        passed = keywords_found and answer_correct
        results.append({
            "question": test["question"],
            "passed": passed,
            "answer": answer[:100]
        })
        
        status = "PASS" if passed else "FAIL"
        print(f"[{status}] {test['question']}")
    
    total = len(results)
    passed = sum(1 for r in results if r["passed"])
    print(f"\nResult: {passed}/{total} tests passed ({100*passed//total}%)")
    return results
```

---

## 🚀 What to Learn Next

You now have a solid, working RAG system. Here's the path forward:

| Topic | What It Adds |
|-------|-------------|
| **Re-ranking** | Sort retrieved chunks by relevance using a cross-encoder model (more accurate than embedding similarity) |
| **Hybrid search** | Combine vector search + keyword search (BM25) for better recall |
| **Parent-child chunking** | Store small chunks for retrieval but pass large parent chunks to LLM |
| **Self-RAG** | Agent that checks whether retrieval is even needed, and verifies its own answer |
| **Fine-tuning embeddings** | Train custom embedding model on your domain for better retrieval |
| **LangSmith** | Tracing and debugging tool — see exactly what your chain is doing |
| **Streamlit / FastAPI** | Wrap your RAG in a web app |
| **Production deployment** | Docker, cloud hosting, rate limiting, auth |

---

## ✅ Course Complete!

You now understand and can build:

- [x] Tokens, embeddings, vectors, and context windows
- [x] How LLMs work, their strengths and limitations
- [x] Prompt engineering for RAG
- [x] Vector databases and semantic search
- [x] The full RAG pipeline: ingestion and retrieval
- [x] When to use RAG vs fine-tuning (and why)
- [x] LangChain: loaders, splitters, embeddings, chains, LCEL
- [x] LangGraph: state, nodes, edges, agents, memory
- [x] A complete working RAG system with retry logic and citations
- [x] How to evaluate and systematically improve your RAG

**You can now build a RAG system from scratch. Go build something.**

---

## 📚 Resources

| Resource | Link |
|----------|------|
| LangChain Docs | https://docs.langchain.com |
| LangGraph Docs | https://langchain-ai.github.io/langgraph/ |
| RAGAS Docs | https://docs.ragas.io |
| ChromaDB Docs | https://docs.trychroma.com |
| OpenAI Embeddings | https://platform.openai.com/docs/guides/embeddings |
| Hugging Face Models | https://huggingface.co/models?pipeline_tag=feature-extraction |
