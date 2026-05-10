# 📘 Module 9: Build Your Complete RAG System 🚀

> **Course:** Build a RAG System from Scratch
> **Prev:** [Module 8 ← LangGraph & Agents](./08_langgraph_and_agents.md)
> **Next:** [Module 10 → Evaluation & Improvement](./10_evaluation_and_improvement.md)

---

## 🎯 What We're Building

A production-quality RAG system with:
- Multi-document ingestion (PDF + text files)
- Smart chunking with metadata
- ChromaDB vector store
- LangGraph agent with retry logic and query rewriting
- Multi-turn conversation memory
- Source citation in answers

**Project structure:**

```
rag_project/
├── docs/              ← Put your documents here
├── chroma_db/         ← Auto-created by ChromaDB
├── .env               ← API keys
├── ingest.py          ← Step 1: Load documents
├── rag_agent.py       ← Step 2: The RAG graph
└── chat.py            ← Step 3: The chat interface
```

---

## Step 0 — Setup

Install dependencies:
```bash
pip install langchain langchain-core langchain-openai langchain-community \
            langchain-chroma langgraph chromadb pypdf python-dotenv
```

Create `.env`:
```
OPENAI_API_KEY=sk-your-key-here
```

---

## Step 1 — Ingestion (`ingest.py`)

Runs once, or whenever documents change:

```python
import os
from pathlib import Path
from dotenv import load_dotenv
from langchain_community.document_loaders import PyPDFLoader, TextLoader, DirectoryLoader
from langchain.text_splitter import RecursiveCharacterTextSplitter
from langchain_openai import OpenAIEmbeddings
from langchain_chroma import Chroma

load_dotenv()

DOCS_DIR = "./docs"
DB_DIR   = "./chroma_db"

def run_ingestion():
    # 1. Load all PDFs and .txt files
    docs = []
    for cls, glob in [(PyPDFLoader, "**/*.pdf"), (TextLoader, "**/*.txt")]:
        loader = DirectoryLoader(DOCS_DIR, glob=glob, loader_cls=cls, show_progress=True)
        docs.extend(loader.load())
    print(f"Loaded {len(docs)} pages/files")

    # 2. Split into chunks
    splitter = RecursiveCharacterTextSplitter(
        chunk_size=600, chunk_overlap=100,
        separators=["\n\n", "\n", ". ", " ", ""]
    )
    chunks = splitter.split_documents(docs)
    print(f"Split into {len(chunks)} chunks")

    # 3. Embed and store
    print("Embedding and storing... (may take a moment)")
    embeddings = OpenAIEmbeddings(model="text-embedding-3-small")
    Chroma.from_documents(chunks, embeddings, persist_directory=DB_DIR)
    print(f"Done! Stored {len(chunks)} chunks in {DB_DIR}")

if __name__ == "__main__":
    run_ingestion()
```

---

## Step 2 — RAG Agent (`rag_agent.py`)

```python
from typing import TypedDict, List, Annotated
from dotenv import load_dotenv
from langchain_openai import ChatOpenAI, OpenAIEmbeddings
from langchain_chroma import Chroma
from langchain_core.messages import HumanMessage, AIMessage
from langgraph.graph import StateGraph, END
from langgraph.graph.message import add_messages
from langgraph.checkpoint.memory import MemorySaver

load_dotenv()

llm        = ChatOpenAI(model="gpt-4o", temperature=0)
embeddings = OpenAIEmbeddings(model="text-embedding-3-small")
vectorstore = Chroma(persist_directory="./chroma_db", embedding_function=embeddings)

# ── State ──────────────────────────────────────────────────────────
class RAGState(TypedDict):
    messages:  Annotated[list, add_messages]
    question:  str
    documents: List[str]
    sources:   List[str]
    answer:    str
    attempts:  int

# ── Nodes ──────────────────────────────────────────────────────────
def retrieve_node(state: RAGState) -> dict:
    results = vectorstore.similarity_search_with_score(state["question"], k=5)
    relevant = [r for r in results if r[1] < 1.5] or results[:3]
    documents = [doc.page_content for doc, _ in relevant]
    sources   = list({doc.metadata.get("source","?") for doc,_ in relevant})
    return {"documents": documents, "sources": sources,
            "attempts": state.get("attempts", 0) + 1}

def rewrite_node(state: RAGState) -> dict:
    resp = llm.invoke(
        f"Rewrite this query to better match document language.\n"
        f"Original: {state['question']}\nRewritten (one sentence):"
    )
    print(f"  Rewritten query: {resp.content.strip()}")
    return {"question": resp.content.strip()}

def generate_node(state: RAGState) -> dict:
    context = "\n\n".join(
        f"[Excerpt {i+1}]: {doc}" for i, doc in enumerate(state["documents"])
    )
    # Build conversation history string
    history = ""
    if state.get("messages") and len(state["messages"]) > 1:
        lines = []
        for m in state["messages"][:-1][-4:]:
            role = "User" if isinstance(m, HumanMessage) else "Assistant"
            lines.append(f"{role}: {m.content}")
        history = "\nConversation so far:\n" + "\n".join(lines) + "\n"

    prompt = (
        "Answer ONLY from the excerpts below. "
        "If the answer is absent, say exactly: \"I don't have that information.\"\n"
        f"{history}\nExcerpts:\n{context}\n\nQuestion: {state['question']}\nAnswer:"
    )
    response = llm.invoke(prompt)

    import os
    src_names = ", ".join(os.path.basename(s) for s in state.get("sources", []))
    final = response.content
    if src_names:
        final += f"\n\n📎 Sources: {src_names}"

    return {"answer": final, "messages": [AIMessage(content=final)]}

def no_results_node(state: RAGState) -> dict:
    msg = "I couldn't find relevant information in my documents."
    return {"answer": msg, "messages": [AIMessage(content=msg)]}

# ── Routing ────────────────────────────────────────────────────────
def route(state: RAGState) -> str:
    if not state.get("documents"):
        return "no_results" if state.get("attempts", 0) >= 2 else "rewrite"
    if state.get("attempts", 0) >= 3:
        return "generate"
    check = llm.invoke(
        f"Is this excerpt relevant to '{state['question']}'?\n"
        f"Excerpt: {state['documents'][0][:250]}\nAnswer yes or no only:"
    )
    return "generate" if "yes" in check.content.lower() else "rewrite"

# ── Build Graph ────────────────────────────────────────────────────
def build_rag_graph():
    g = StateGraph(RAGState)
    g.add_node("retrieve",   retrieve_node)
    g.add_node("rewrite",    rewrite_node)
    g.add_node("generate",   generate_node)
    g.add_node("no_results", no_results_node)

    g.set_entry_point("retrieve")
    g.add_conditional_edges("retrieve", route,
        {"generate": "generate", "rewrite": "rewrite", "no_results": "no_results"})
    g.add_edge("rewrite",    "retrieve")
    g.add_edge("generate",   END)
    g.add_edge("no_results", END)

    return g.compile(checkpointer=MemorySaver())

rag_app = build_rag_graph()
```

---

## Step 3 — Chat Interface (`chat.py`)

```python
import uuid
from langchain_core.messages import HumanMessage
from rag_agent import rag_app

def chat():
    print("\n" + "="*50)
    print("  RAG Assistant — Ask about your documents")
    print("="*50)
    print("  'quit' to exit  |  'new' for new conversation\n")

    thread_id = str(uuid.uuid4())
    config = {"configurable": {"thread_id": thread_id}}

    while True:
        user_input = input("You: ").strip()
        if not user_input:
            continue
        if user_input.lower() in ["quit", "exit", "q"]:
            print("Goodbye!")
            break
        if user_input.lower() == "new":
            thread_id = str(uuid.uuid4())
            config = {"configurable": {"thread_id": thread_id}}
            print("Started new conversation.\n")
            continue

        result = rag_app.invoke(
            {"question": user_input,
             "messages": [HumanMessage(content=user_input)],
             "attempts": 0},
            config=config
        )
        print(f"\nAssistant: {result['answer']}\n")

if __name__ == "__main__":
    chat()
```

---

## Step 4 — Run It

```bash
# Add documents to docs/ folder, then:
python ingest.py    # Run once

python chat.py      # Start chatting
```

---

## The Full Agent Flow

```
START
  |
  v
[retrieve] ──> route() ──> "generate"   ──> [generate]   ──> END
                    |
                    |──> "rewrite"    ──> [rewrite] ──> [retrieve]  (loop)
                    |
                    └──> "no_results" ──> [no_results] ──> END
```

- First attempt: retrieve and check relevance
- If not relevant: rewrite the query and try again
- After 3 attempts: generate with whatever was found
- If completely empty: politely say "I don't know"

---

## Common Customizations

**Use a different LLM (free, runs locally):**
```python
from langchain_community.llms import Ollama
llm = Ollama(model="llama3.1")  # No API key needed
```

**Load from a website instead of PDF:**
```python
from langchain_community.document_loaders import WebBaseLoader
loader = WebBaseLoader("https://your-docs-site.com")
docs = loader.load()
```

**Add more documents without re-ingesting everything:**
```python
vectorstore = Chroma(persist_directory="./chroma_db", embedding_function=embeddings)
vectorstore.add_documents(new_chunks)  # Adds without deleting existing
```

---

## Checklist — You Built a RAG System

- [x] Loaded documents from multiple formats
- [x] Split with proper chunk size and overlap
- [x] Embedded and stored in ChromaDB
- [x] Built a LangGraph agent with retry logic
- [x] Added query rewriting when retrieval fails
- [x] Multi-turn memory with MemorySaver
- [x] Source citations in every answer
- [x] Clean chat loop interface

---

> Next: [Module 10 — Evaluation and Improvement](./10_evaluation_and_improvement.md)
