# Build a RAG System from Scratch
### A Complete Beginner's Course — No AI Background Needed

---

## What You'll Be Able to Do

By the end of this course you will understand how AI language models work at a fundamental level, and you will be able to build a fully working RAG (Retrieval-Augmented Generation) system using Python, LangChain, LangGraph, and ChromaDB.

---

## Course Modules

| # | Module | What You Learn |
|---|--------|----------------|
| 1 | [How AI Thinks — Tokens, Embeddings & Vectors](./01_how_ai_thinks.md) | Tokenization, embeddings, cosine similarity, context window, chunking |
| 2 | [What is an LLM?](./02_what_is_an_llm.md) | Pre-training, RLHF, hallucination, temperature, system prompts |
| 3 | [Prompt Engineering](./03_prompt_engineering.md) | Role prompting, few-shot, chain-of-thought, structured output, RAG prompt design |
| 4 | [Vector Databases](./04_vector_databases.md) | Semantic search, ChromaDB, MMR, metadata filtering, loaders |
| 5 | [What is RAG?](./05_what_is_rag.md) | The full pipeline, query rewriting, HyDE, multi-query, self-RAG |
| 6 | [Fine-Tuning vs RAG](./06_finetuning_vs_rag.md) | Deep comparison, LoRA, when to use each, decision framework, cost reality |
| 7 | [LangChain Basics](./07_langchain_basics.md) | All 7 core components, LCEL chains, streaming, full code example |
| 8 | [LangGraph & Agents](./08_langgraph_and_agents.md) | State, nodes, edges, conditional routing, tools, ReAct, memory |
| 9 | [Build Your RAG System](./09_build_your_rag.md) | Complete production code: ingest.py + rag_agent.py + chat.py |
| 10 | [Evaluation & Improvement](./10_evaluation_and_improvement.md) | RAGAS metrics, diagnosis cheat sheet, test suites, next steps |

---

## Learning Path

```
[1] Tokens & Embeddings
        |
        v
[2] What is an LLM
        |
        v
[3] Prompt Engineering         ← Often skipped. Don't skip it.
        |
        v
[4] Vector Databases
        |
        v
[5] What is RAG                ← The core concept
        |
        v
[6] Fine-Tuning vs RAG         ← Critical decision framework
        |
        v
[7] LangChain Basics           ← The toolkit
        |
        v
[8] LangGraph & Agents         ← Smart workflows
        |
        v
[9] Build Your RAG             ← Hands-on project
        |
        v
[10] Evaluate & Improve        ← Production quality
```

---

## Tech Stack

| Tool | Purpose |
|------|---------|
| Python 3.10+ | Language |
| LangChain | RAG building blocks |
| LangGraph | Agent/workflow graphs |
| ChromaDB | Local vector database |
| OpenAI GPT-4o | LLM for generation |
| OpenAI text-embedding-3-small | Embedding model |
| RAGAS | Evaluation framework |

---

## Quick Start

```bash
pip install langchain langchain-core langchain-openai langchain-community \
            langchain-chroma langgraph chromadb pypdf python-dotenv ragas
```

Then open [Module 1](./01_how_ai_thinks.md) and follow the path.

---

*Start here → [Module 1: How AI Thinks](./01_how_ai_thinks.md)*
