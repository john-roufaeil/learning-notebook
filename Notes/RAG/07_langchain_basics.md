# 📘 Module 7: LangChain — Your AI Toolkit

> **Course:** Build a RAG System from Scratch
> **Prev:** [Module 6 ← Fine-Tuning vs RAG](./06_finetuning_vs_rag.md)
> **Next:** [Module 8 → LangGraph & Agents](./08_langgraph_and_agents.md)

---

## 🧰 What is LangChain?

**LangChain** is a Python (and JavaScript) framework for building applications powered by LLMs. It provides ready-made components for every step of the AI pipeline, so you don't have to build everything from scratch.

Without LangChain, building a RAG system means:
- Manually calling the OpenAI API
- Parsing and formatting all responses yourself
- Writing your own chunking logic
- Managing embedding calls in batches
- Building your own retrieval logic
- Handling errors and retries everywhere

LangChain gives you all of this pre-built and composable.

> 💡 Think of LangChain like React for web development — you don't have to build from raw HTML/JS every time. You use components, hooks, and a clear structure.

---

## 📦 LangChain's Package Structure

LangChain is split into multiple packages:

```
langchain-core         → Base types, interfaces (always install this)
langchain              → Main chains, agents, document tools
langchain-openai       → OpenAI LLM + embeddings integration
langchain-community    → 100+ third-party integrations (loaders, DBs...)
langchain-chroma       → ChromaDB integration
langgraph              → Agent orchestration (next module)
```

```bash
pip install langchain langchain-core langchain-openai \
            langchain-community langchain-chroma \
            chromadb pypdf python-dotenv
```

---

## 🏗️ The 7 Core Building Blocks

### 1. Chat Models (LLMs)

Connect to any LLM with a unified interface:

```python
from langchain_openai import ChatOpenAI

# OpenAI
llm = ChatOpenAI(
    model="gpt-4o",
    temperature=0,           # Deterministic (good for RAG)
    max_tokens=1000,         # Max response length
    api_key="your-key"       # Or set OPENAI_API_KEY env var
)

response = llm.invoke("What is the capital of Egypt?")
print(response.content)   # "Cairo"
print(response.usage_metadata)  # token counts
```

**Using other models with the same interface:**

```python
from langchain_community.llms import Ollama  # Local models
from langchain_anthropic import ChatAnthropic  # Claude
from langchain_google_genai import ChatGoogleGenerativeAI  # Gemini

# Local open-source model (no API key needed!)
llm = Ollama(model="llama3.1")

# Anthropic Claude
llm = ChatAnthropic(model="claude-3-5-sonnet-20241022")
```

*Swap any of these in — your chains stay the same.*

---

### 2. Prompt Templates

Create reusable prompt structures with placeholders:

```python
from langchain.prompts import ChatPromptTemplate, PromptTemplate

# Chat prompt (system + user messages)
chat_prompt = ChatPromptTemplate.from_messages([
    ("system", "You are an expert in {domain}. Answer concisely."),
    ("human", "{question}")
])

# Fill in values
filled = chat_prompt.invoke({
    "domain": "Egyptian history",
    "question": "Who built the Step Pyramid?"
})

response = llm.invoke(filled)
print(response.content)
```

**Why templates instead of f-strings?**

```python
# Bad (f-string):
prompt = f"You are an expert in {domain}. Answer: {question}"
# → No type checking, no reuse, no composability

# Good (template):
template = ChatPromptTemplate.from_messages([...])
# → Reusable, composable with chains, works with LangSmith tracing
```

---

### 3. Output Parsers

Transform the raw LLM response into something useful:

```python
from langchain_core.output_parsers import (
    StrOutputParser,      # Just get the text
    JsonOutputParser,     # Parse JSON output
    PydanticOutputParser  # Parse into a Python class
)

# String parser (most common)
parser = StrOutputParser()
chain = llm | parser
result = chain.invoke("What is 2+2?")
print(result)           # "4" (plain string, not AIMessage object)

# JSON parser
from langchain_core.output_parsers import JsonOutputParser

json_llm = ChatOpenAI(model="gpt-4o", temperature=0)
json_prompt = ChatPromptTemplate.from_messages([
    ("system", "Return ONLY valid JSON, no other text."),
    ("human", "Extract: name, age, city from: {text}")
])

json_chain = json_prompt | json_llm | JsonOutputParser()
result = json_chain.invoke({"text": "Ahmed is 28 years old and lives in Cairo."})
print(result)
# {"name": "Ahmed", "age": 28, "city": "Cairo"}
```

**Pydantic parser** (most powerful — validates the output against a schema):

```python
from pydantic import BaseModel
from langchain_core.output_parsers import PydanticOutputParser

class Person(BaseModel):
    name: str
    age: int
    city: str

parser = PydanticOutputParser(pydantic_object=Person)

prompt = ChatPromptTemplate.from_messages([
    ("system", "Extract info. {format_instructions}"),
    ("human", "{text}")
]).partial(format_instructions=parser.get_format_instructions())

chain = prompt | llm | parser
result = chain.invoke({"text": "Ahmed is 28, he lives in Cairo."})
print(result.name)   # "Ahmed"
print(result.age)    # 28
```

---

### 4. Document Loaders

Load content from nearly any source:

```python
from langchain_community.document_loaders import (
    PyPDFLoader,
    TextLoader,
    WebBaseLoader,
    UnstructuredWordDocumentLoader,
    CSVLoader,
    YoutubeLoader
)

# PDF (one Document per page)
loader = PyPDFLoader("report.pdf")
docs = loader.load()
print(docs[0].page_content[:200])   # first 200 chars of page 1
print(docs[0].metadata)             # {'source': 'report.pdf', 'page': 0}

# Load all PDFs in a folder
from langchain_community.document_loaders import DirectoryLoader
loader = DirectoryLoader("./docs/", glob="**/*.pdf", loader_cls=PyPDFLoader)
all_docs = loader.load()

# Web page
loader = WebBaseLoader("https://en.wikipedia.org/wiki/Cairo")
docs = loader.load()

# YouTube transcript
loader = YoutubeLoader.from_youtube_url("https://youtube.com/watch?v=...")
docs = loader.load()
```

---

### 5. Text Splitters

Split documents into chunks for embedding:

```python
from langchain.text_splitter import (
    RecursiveCharacterTextSplitter,  # Best default
    CharacterTextSplitter,
    TokenTextSplitter,               # Split by tokens instead of chars
    MarkdownHeaderTextSplitter       # Split at markdown headers
)

# Best all-around splitter
splitter = RecursiveCharacterTextSplitter(
    chunk_size=600,       # Characters per chunk
    chunk_overlap=100,    # Overlap between chunks
    separators=[          # Try these in order
        "\n\n",           # Paragraph break (preferred)
        "\n",             # Line break
        ". ",             # Sentence end
        " ",              # Word boundary (last resort)
        ""                # Character boundary (absolute last resort)
    ]
)

chunks = splitter.split_documents(docs)
print(f"{len(docs)} documents → {len(chunks)} chunks")
print(f"Average chunk size: {sum(len(c.page_content) for c in chunks)//len(chunks)} chars")

# See what a chunk looks like
print(chunks[5].page_content)
print(chunks[5].metadata)  # Inherits metadata from parent document
```

**Markdown-aware splitting (great for technical docs):**

```python
from langchain.text_splitter import MarkdownHeaderTextSplitter

splitter = MarkdownHeaderTextSplitter(
    headers_to_split_on=[
        ("#", "Header 1"),
        ("##", "Header 2"),
        ("###", "Header 3"),
    ]
)
chunks = splitter.split_text(markdown_text)
# Each chunk knows which section/subsection it belongs to
```

---

### 6. Embeddings + Vector Stores

Embed chunks and store them for retrieval:

```python
from langchain_openai import OpenAIEmbeddings
from langchain_chroma import Chroma

# Create embeddings model
embeddings = OpenAIEmbeddings(model="text-embedding-3-small")

# Test it manually
vector = embeddings.embed_query("What is machine learning?")
print(f"Vector has {len(vector)} dimensions")  # 1536

# Create vector store from documents (ingestion)
vectorstore = Chroma.from_documents(
    documents=chunks,
    embedding=embeddings,
    persist_directory="./chroma_db",
    collection_name="my_docs"
)

# Load existing vector store (after ingestion)
vectorstore = Chroma(
    persist_directory="./chroma_db",
    embedding_function=embeddings,
    collection_name="my_docs"
)

# Search
results = vectorstore.similarity_search("What is the refund policy?", k=3)
for r in results:
    print(r.page_content)
    print(r.metadata)
    print()
```

---

### 7. Retrievers

The bridge between vector stores and chains:

```python
# Basic retriever
retriever = vectorstore.as_retriever(
    search_type="similarity",  # or "mmr"
    search_kwargs={"k": 4}
)

# Test the retriever
docs = retriever.invoke("What is the refund policy?")
print(f"Retrieved {len(docs)} chunks")

# MultiQueryRetriever — generates multiple queries for better recall
from langchain.retrievers import MultiQueryRetriever

multi_retriever = MultiQueryRetriever.from_llm(
    retriever=retriever,
    llm=llm
)
# Internally generates 3 variants of your query and merges results
```

---

## 🔗 LCEL — LangChain Expression Language

Modern LangChain uses **LCEL** (the pipe `|` syntax) to chain components together. Read chains left to right — output of each step flows into the next.

```python
from langchain_core.output_parsers import StrOutputParser
from langchain_core.runnables import RunnablePassthrough

# Simple chain: prompt → LLM → parse output
simple_chain = prompt | llm | StrOutputParser()
result = simple_chain.invoke({"question": "What is gravity?"})
```

**Full RAG chain with LCEL:**

```python
from langchain_core.runnables import RunnablePassthrough
from langchain_core.output_parsers import StrOutputParser

# Format retrieved documents into a single string
def format_docs(docs):
    return "\n\n---\n\n".join([
        f"[Source: {doc.metadata.get('source', 'unknown')}]\n{doc.page_content}"
        for doc in docs
    ])

rag_prompt = ChatPromptTemplate.from_messages([
    ("system", """Answer based ONLY on the context below.
If the answer isn't in the context, say "I don't have that information."

Context:
{context}"""),
    ("human", "{question}")
])

# Build the chain
rag_chain = (
    {
        "context": retriever | format_docs,  # Retrieve + format
        "question": RunnablePassthrough()    # Pass question through unchanged
    }
    | rag_prompt    # Build the prompt with context + question
    | llm           # Send to LLM
    | StrOutputParser()  # Extract text from response
)

# Use it
answer = rag_chain.invoke("What is the company's refund policy?")
print(answer)
```

**Streaming responses** (for real-time output):

```python
for chunk in rag_chain.stream("What is the refund policy?"):
    print(chunk, end="", flush=True)
# Output appears word by word, like ChatGPT
```

---

## 🔁 Complete RAG Pipeline — All Together

```python
import os
from dotenv import load_dotenv
from langchain_openai import ChatOpenAI, OpenAIEmbeddings
from langchain_chroma import Chroma
from langchain_community.document_loaders import PyPDFLoader
from langchain.text_splitter import RecursiveCharacterTextSplitter
from langchain.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser
from langchain_core.runnables import RunnablePassthrough

load_dotenv()

# === INGESTION (run once) ===
def ingest(pdf_path: str, db_path: str = "./chroma_db"):
    loader = PyPDFLoader(pdf_path)
    docs = loader.load()
    
    splitter = RecursiveCharacterTextSplitter(chunk_size=600, chunk_overlap=100)
    chunks = splitter.split_documents(docs)
    
    embeddings = OpenAIEmbeddings(model="text-embedding-3-small")
    Chroma.from_documents(chunks, embeddings, persist_directory=db_path)
    print(f"Stored {len(chunks)} chunks from {pdf_path}")

# === RETRIEVAL (run every query) ===
def build_rag_chain(db_path: str = "./chroma_db"):
    embeddings = OpenAIEmbeddings(model="text-embedding-3-small")
    vectorstore = Chroma(persist_directory=db_path, embedding_function=embeddings)
    retriever = vectorstore.as_retriever(search_kwargs={"k": 4})
    
    llm = ChatOpenAI(model="gpt-4o", temperature=0)
    
    prompt = ChatPromptTemplate.from_messages([
        ("system", """You are a helpful assistant. Answer ONLY based on 
the context below. If you can't find the answer, say "I don't know."

Context:
{context}"""),
        ("human", "{question}")
    ])
    
    def format_docs(docs):
        return "\n\n---\n\n".join(d.page_content for d in docs)
    
    chain = (
        {"context": retriever | format_docs, "question": RunnablePassthrough()}
        | prompt
        | llm
        | StrOutputParser()
    )
    return chain

# === MAIN ===
if __name__ == "__main__":
    # ingest("my_document.pdf")  # Run once
    chain = build_rag_chain()
    
    while True:
        q = input("You: ")
        if q.lower() == "quit":
            break
        print(f"AI: {chain.invoke(q)}\n")
```

---

## ✅ Full Recap

| LangChain Component | What it does |
|--------------------|-------------|
| **ChatOpenAI** | Connects to GPT models |
| **ChatPromptTemplate** | Reusable prompts with placeholders |
| **StrOutputParser** | Extracts plain text from LLM response |
| **PydanticOutputParser** | Extracts and validates structured data |
| **PyPDFLoader** | Loads PDF files into Documents |
| **RecursiveCharacterTextSplitter** | Splits docs into chunks smartly |
| **OpenAIEmbeddings** | Converts text → vectors |
| **Chroma** | Vector database for storing and searching |
| **Retriever** | Searches vector DB for relevant chunks |
| **LCEL (`\|`)** | Connects components into a chain |
| **RunnablePassthrough** | Passes input through unchanged in a chain |

---

> ➡️ **Next:** [Module 8 — LangGraph & Agents](./08_langgraph_and_agents.md)
