# 📘 Module 4: Vector Databases — The AI's Memory System

> **Course:** Build a RAG System from Scratch
> **Prev:** [Module 3 ← Prompt Engineering](./03_prompt_engineering.md)
> **Next:** [Module 5 → What is RAG?](./05_what_is_rag.md)

---

## 🗃️ Why Regular Databases Aren't Enough

A traditional database is great at exact matching:

```sql
SELECT * FROM products WHERE name = 'laptop';
```

But what happens if the user types: *"portable computer"* or *"notebook"*? A regular database returns nothing — because it only matches exact text.

What if the user means *"What should I eat if I have low iron?"* and the answer is in a document about *"foods high in iron"*? Keyword search misses the connection entirely.

**Vector databases solve this.** They store meaning, not just text. They find "portable computer" when you search for "laptop," because those vectors are close to each other.

---

## 📦 What a Vector Database Actually Stores

Each entry in a vector database has three parts:

```
┌─────────────────────────────────────────────────────────────┐
│  ID       │  "chunk_001"                                    │
├─────────────────────────────────────────────────────────────┤
│  Vector   │  [0.23, -0.14, 0.87, 0.03, 0.61, ..., -0.22]  │
│           │  (1,536 numbers representing meaning)           │
├─────────────────────────────────────────────────────────────┤
│  Metadata │  {                                              │
│           │    "source": "company_report_2024.pdf",        │
│           │    "page": 12,                                  │
│           │    "section": "Financial Results",             │
│           │    "date": "2024-10-01"                        │
│           │  }                                              │
├─────────────────────────────────────────────────────────────┤
│  Text     │  "Q3 2024 revenue reached $4.2M, an 18%        │
│           │   increase driven by enterprise sales."         │
└─────────────────────────────────────────────────────────────┘
```

The **metadata** is crucial — it lets you filter results (e.g., "only search documents from 2024") and cite sources in your answers.

---

## 🔍 How Similarity Search Works Under the Hood

When you query a vector database:

```
User: "What was the company's revenue last quarter?"
```

1. The question gets embedded → `[0.21, -0.11, 0.84, ...]`
2. The DB computes the **distance** between this query vector and every stored vector
3. Returns the top-k closest ones (usually 3–10)

**Distance algorithms:**

| Algorithm | When to use |
|-----------|-------------|
| **Cosine Similarity** | Most common. Measures angle between vectors. Best for text. |
| **Euclidean (L2)** | Measures straight-line distance. Good for numeric data. |
| **Dot Product** | Fast, good when vectors are normalized. |
| **Manhattan** | Rarely used for text, mostly ML use cases. |

For RAG with text, **always use cosine similarity** unless you have a specific reason not to.

---

## 🏗️ The Full Ingestion Pipeline

This happens once (when you load documents), not on every query:

```python
from langchain_community.document_loaders import PyPDFLoader
from langchain.text_splitter import RecursiveCharacterTextSplitter
from langchain_openai import OpenAIEmbeddings
from langchain_community.vectorstores import Chroma

# Step 1: Load
loader = PyPDFLoader("company_report.pdf")
pages = loader.load()
# pages = list of Document objects, one per PDF page

# Step 2: Split into chunks
splitter = RecursiveCharacterTextSplitter(
    chunk_size=600,
    chunk_overlap=100
)
chunks = splitter.split_documents(pages)
# Each chunk is a Document with .page_content and .metadata

# Step 3: Embed + Store
embeddings = OpenAIEmbeddings(model="text-embedding-3-small")
vectorstore = Chroma.from_documents(
    documents=chunks,
    embedding=embeddings,
    persist_directory="./chroma_db",  # save to disk
    collection_name="company_docs"    # named collection
)
print(f"Stored {len(chunks)} chunks")
```

---

## 🔎 Querying the Vector Database

```python
# Load existing DB (after ingestion)
vectorstore = Chroma(
    persist_directory="./chroma_db",
    embedding_function=OpenAIEmbeddings()
)

# Basic similarity search
results = vectorstore.similarity_search(
    query="What was Q3 revenue?",
    k=4  # return top 4 most similar chunks
)

for doc in results:
    print(doc.page_content)
    print(doc.metadata)
    print("---")
```

**With scores** (so you can filter by relevance threshold):

```python
results_with_scores = vectorstore.similarity_search_with_score(
    query="What was Q3 revenue?",
    k=5
)

for doc, score in results_with_scores:
    print(f"Score: {score:.3f}")  # Lower = more similar (L2 distance)
    print(doc.page_content[:100])
    print()
```

**With metadata filtering** (only search certain documents):

```python
results = vectorstore.similarity_search(
    query="revenue",
    k=3,
    filter={"source": "company_report_2024.pdf"}  # only this file
)
```

---

## 🏋️ MMR — Max Marginal Relevance

Regular similarity search sometimes returns chunks that are all saying the same thing. **MMR** balances relevance *and* diversity:

```python
retriever = vectorstore.as_retriever(
    search_type="mmr",            # Max Marginal Relevance
    search_kwargs={
        "k": 5,                   # return 5 chunks
        "fetch_k": 20,            # initially fetch 20 candidates
        "lambda_mult": 0.5        # 0=max diversity, 1=max relevance
    }
)
```

MMR works like this:
1. Fetch 20 candidates by similarity
2. Pick the most relevant one
3. Pick the next one that is relevant AND most different from already-picked ones
4. Repeat until you have 5

This prevents the LLM from getting 5 chunks that all say the same thing.

---

## 🗂️ Popular Vector Databases Compared

| Database | Where it runs | Best for | Notes |
|----------|--------------|---------|-------|
| **ChromaDB** | Local or server | Learning, small projects | Easiest to start with |
| **FAISS** | Local (in memory) | Fast retrieval, research | Made by Meta, no persistence by default |
| **Pinecone** | Cloud (managed) | Production apps | No setup, scalable, paid |
| **Weaviate** | Local or cloud | Large-scale production | Full-featured, complex |
| **Qdrant** | Local or cloud | Production RAG | Fast, good filtering |
| **pgvector** | PostgreSQL extension | You already use Postgres | SQL + vectors together |
| **Milvus** | Local or cloud | Enterprise scale | Very fast at scale |

**For this course:** We use **ChromaDB**. It runs locally, requires no account, and is dead simple.

---

## 🗂️ Document Loaders — Getting Data In

LangChain has loaders for almost every document format:

```python
from langchain_community.document_loaders import (
    PyPDFLoader,          # PDF files
    TextLoader,           # Plain text
    CSVLoader,            # CSV files
    UnstructuredWordDocumentLoader,  # .docx files
    WebBaseLoader,        # Web pages (scraping)
    YoutubeLoader,        # YouTube transcripts
    NotionDirectoryLoader, # Notion exports
    GitLoader,            # Git repositories
)

# PDF example
loader = PyPDFLoader("report.pdf")

# Web page example
loader = WebBaseLoader("https://docs.python.org/3/tutorial/")

# Multiple files at once
from langchain_community.document_loaders import DirectoryLoader

loader = DirectoryLoader(
    "./docs/",
    glob="**/*.pdf",  # all PDFs in folder and subfolders
    loader_cls=PyPDFLoader
)

docs = loader.load()
print(f"Loaded {len(docs)} documents")
```

---

## 🏷️ Metadata — The Underrated Feature

Metadata is extra information stored alongside each chunk. It's incredibly useful:

```python
from langchain.schema import Document

# Create a document with rich metadata
doc = Document(
    page_content="Revenue grew 18% in Q3 2024.",
    metadata={
        "source": "annual_report.pdf",
        "page": 15,
        "section": "Financial Highlights",
        "year": 2024,
        "department": "Finance",
        "confidential": True
    }
)
```

**Why metadata matters:**

1. **Source citations** — Tell users where the answer came from
2. **Filtering** — Search only documents from a certain year, department, or file
3. **Debugging** — When something goes wrong, trace back to the exact source
4. **Access control** — Only show chunks a user has permission to see

---

## ✅ Full Recap

| Term | Definition |
|------|-----------|
| **Vector Database** | Stores embeddings and searches by semantic similarity |
| **Semantic Search** | Finding similar meaning, not just matching keywords |
| **Cosine Similarity** | Measures similarity between two vectors (0 to 1) |
| **MMR** | Retrieves diverse AND relevant results |
| **Ingestion** | The process of loading, chunking, embedding, and storing documents |
| **Metadata** | Extra info stored with each chunk (source, page, date, etc.) |
| **Collection** | A named group of documents in a vector database |
| **ChromaDB** | Simple local vector database, perfect for learning |

---

> ➡️ **Next:** [Module 5 — What is RAG?](./05_what_is_rag.md)
