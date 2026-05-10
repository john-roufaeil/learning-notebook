# 📘 Module 8: LangGraph & Agents — Smart AI Workflows

> **Course:** Build a RAG System from Scratch
> **Prev:** [Module 7 ← LangChain Basics](./07_langchain_basics.md)
> **Next:** [Module 9 → Build Your RAG System](./09_build_your_rag.md)

---

## 🤔 When a Simple Chain Isn't Enough

A simple RAG chain does this every time:

```
Question → Retrieve → Generate → Answer
```

That's great for basic use cases. But real-world AI systems need to:

- **Make decisions** — Should I retrieve? Or do I know this already?
- **Handle failures** — What if the retrieved chunks are irrelevant?
- **Loop** — Retry with a better query if the first attempt fails
- **Use multiple tools** — Search the DB, search the web, run calculations
- **Remember context** — Keep state across a multi-turn conversation
- **Break complex tasks** into sub-steps and execute them in order

For all of this, you need **agents** and **LangGraph**.

---

## 🕵️ What is an Agent?

An **agent** is an LLM that can:
1. Look at a task
2. Choose which tool to use
3. Use the tool and observe the result
4. Decide what to do next (use another tool, or return a final answer)
5. Repeat until the task is done

```
User: "Find the Q3 revenue, calculate the growth rate vs Q2, and 
       email a summary to the finance team."

Agent decides:
  Step 1: Use vector_search tool → find Q3 and Q2 revenue
  Step 2: Use calculator tool → compute (4.2 - 3.5) / 3.5 * 100 = 20%
  Step 3: Use email tool → send summary to finance@company.com
  Done.
```

Without an agent, you'd have to hard-code every step. The agent figures out the steps itself.

---

## 🔄 What is LangGraph?

**LangGraph** is a library for building agent workflows as **graphs** (nodes connected by edges).

```
Install:
pip install langgraph
```

A LangGraph application is like a flowchart where:
- **Nodes** = steps (functions that do something)
- **Edges** = arrows (what comes next)
- **State** = data shared between all nodes
- **Conditional edges** = branches (if X then go here, else go there)

---

## 🧩 LangGraph Core Concepts

### State — Shared Data

The **state** is a Python dictionary that flows through the entire graph. Every node reads from it and can write to it.

```python
from typing import TypedDict, List, Annotated
from langgraph.graph import add_messages

class RAGState(TypedDict):
    # User's question
    question: str
    
    # Retrieved document chunks
    documents: List[str]
    
    # Number of retrieval attempts (for retry logic)
    attempts: int
    
    # Final generated answer
    answer: str
    
    # Conversation history (for multi-turn chat)
    messages: Annotated[list, add_messages]
```

> `Annotated[list, add_messages]` is a special LangGraph type that automatically appends new messages instead of replacing them. Essential for chat history.

---

### Nodes — The Steps

Each node is a function:
- Takes the current state as input
- Returns a *partial* state update (only the fields that changed)

```python
from langchain_openai import ChatOpenAI, OpenAIEmbeddings
from langchain_chroma import Chroma

llm = ChatOpenAI(model="gpt-4o", temperature=0)
embeddings = OpenAIEmbeddings()
vectorstore = Chroma(persist_directory="./chroma_db", embedding_function=embeddings)

def retrieve_node(state: RAGState) -> dict:
    """Search vector database for relevant chunks."""
    print(f"🔍 Retrieving for: {state['question']}")
    
    docs = vectorstore.similarity_search(state["question"], k=4)
    doc_texts = [doc.page_content for doc in docs]
    
    return {
        "documents": doc_texts,
        "attempts": state.get("attempts", 0) + 1
    }

def generate_node(state: RAGState) -> dict:
    """Generate answer from retrieved context."""
    context = "\n\n---\n\n".join(state["documents"])
    
    response = llm.invoke(
        f"Answer based ONLY on this context:\n{context}\n\nQuestion: {state['question']}"
    )
    
    return {"answer": response.content}

def rewrite_query_node(state: RAGState) -> dict:
    """Rewrite the question for better retrieval."""
    rewrite = llm.invoke(
        f"Rewrite this question to be more specific for document search: {state['question']}"
    )
    return {"question": rewrite.content}
```

---

### Edges — The Connections

**Simple edge** (always goes to next node):

```python
graph.add_edge("retrieve", "generate")
```

**Conditional edge** (chooses path based on logic):

```python
def check_quality(state: RAGState) -> str:
    """Decide whether retrieved docs are good enough."""
    if not state["documents"]:
        return "no_docs"
    
    if state["attempts"] >= 3:
        return "give_up"  # Prevent infinite loops!
    
    # Ask LLM if the docs are relevant
    check = llm.invoke(
        f"Are these documents relevant to '{state['question']}'? "
        f"First document: {state['documents'][0][:200]}"
        f"\nAnswer only: yes or no"
    )
    
    if "yes" in check.content.lower():
        return "generate"
    else:
        return "rewrite"

graph.add_conditional_edges(
    "retrieve",           # From this node...
    check_quality,        # ...call this function to decide...
    {
        "generate": "generate",     # if "generate" → go to generate
        "rewrite": "rewrite_query", # if "rewrite" → rewrite the query
        "no_docs": "no_docs",       # if "no_docs" → handle empty result
        "give_up": "generate"       # if tried 3 times → just generate what we have
    }
)
```

---

### Building the Full Graph

```python
from langgraph.graph import StateGraph, END

def no_docs_node(state: RAGState) -> dict:
    return {"answer": "I couldn't find relevant information in my documents."}

# Create graph
graph = StateGraph(RAGState)

# Add all nodes
graph.add_node("retrieve", retrieve_node)
graph.add_node("rewrite_query", rewrite_query_node)
graph.add_node("generate", generate_node)
graph.add_node("no_docs", no_docs_node)

# Entry point
graph.set_entry_point("retrieve")

# Edges
graph.add_conditional_edges("retrieve", check_quality, {
    "generate": "generate",
    "rewrite": "rewrite_query",
    "no_docs": "no_docs",
    "give_up": "generate"
})
graph.add_edge("rewrite_query", "retrieve")  # Loop back!
graph.add_edge("generate", END)
graph.add_edge("no_docs", END)

# Compile
app = graph.compile()

# Run
result = app.invoke({"question": "What was Q3 revenue?", "attempts": 0})
print(result["answer"])
```

**The flow this creates:**

```
START
  │
  ▼
retrieve ──→ check_quality ──→ "generate" ──→ generate ──→ END
                 │
                 ├──→ "rewrite" ──→ rewrite_query ──→ retrieve (loop!)
                 │
                 ├──→ "no_docs" ──→ no_docs ──→ END
                 │
                 └──→ "give_up" ──→ generate ──→ END
```

---

## 🧠 Giving Agents Memory

By default, LangGraph agents have no memory between calls. To add memory:

```python
from langgraph.checkpoint.memory import MemorySaver

# Create memory backend
memory = MemorySaver()

# Compile graph WITH memory
app = graph.compile(checkpointer=memory)

# Each conversation needs a unique thread_id
config = {"configurable": {"thread_id": "user-ahmed-session-1"}}

# First message
result1 = app.invoke(
    {"question": "My name is Ahmed and I want to know about Q3 revenue."},
    config=config
)

# Second message — same thread_id = same conversation
result2 = app.invoke(
    {"question": "What was the growth rate compared to Q2?"},
    config=config
)
# The agent still has context from the first message!
```

**Thread IDs let you manage multiple users:**

```python
# User 1's conversation
config_user1 = {"configurable": {"thread_id": "ahmed-123"}}

# User 2's completely separate conversation
config_user2 = {"configurable": {"thread_id": "sara-456"}}
```

---

## 🛠️ Tools — What Agents Can Do

**Tools** are functions the agent can choose to call. You define them, the agent decides when and how to use them.

```python
from langchain_core.tools import tool

@tool
def search_documents(query: str) -> str:
    """Search the internal document database for information."""
    docs = vectorstore.similarity_search(query, k=3)
    return "\n\n".join(d.page_content for d in docs)

@tool
def calculate(expression: str) -> str:
    """Evaluate a mathematical expression. Example: '(4.2 - 3.5) / 3.5 * 100'"""
    try:
        result = eval(expression)  # In production, use a safer evaluator
        return str(result)
    except Exception as e:
        return f"Error: {e}"

@tool  
def get_current_date() -> str:
    """Get today's date."""
    from datetime import date
    return date.today().isoformat()

# Give tools to the LLM
llm_with_tools = llm.bind_tools([search_documents, calculate, get_current_date])

# The LLM can now decide to call any of these
response = llm_with_tools.invoke(
    "What was Q3 revenue and what percent is that of the annual target of $20M?"
)
# LLM will call search_documents, then calculate
```

---

## 🤖 ReAct Agent — A Classic Pattern

**ReAct (Reasoning + Acting)** is the most common agent pattern:

```
Thought:  What do I need to do?
Action:   [call a tool]
Observation: [tool result]
Thought:  What do I do with this result?
Action:   [call another tool, or answer]
...
Final Answer: [done]
```

```python
from langgraph.prebuilt import create_react_agent

# Create a ReAct agent with your tools
agent = create_react_agent(
    model=llm,
    tools=[search_documents, calculate, get_current_date]
)

# Run it
result = agent.invoke({
    "messages": [
        ("user", "What was Q3 revenue and what's the growth rate vs Q2?")
    ]
})

# Print the conversation (shows tool calls + results)
for message in result["messages"]:
    print(f"{message.type}: {message.content}")
```

**Output will look like:**
```
human: What was Q3 revenue and what's the growth rate vs Q2?
ai: [Calls search_documents("Q3 2024 revenue")]
tool: "Q3 revenue was $4.2M..."
ai: [Calls search_documents("Q2 2024 revenue")]
tool: "Q2 revenue was $3.5M..."
ai: [Calls calculate("(4.2 - 3.5) / 3.5 * 100")]
tool: "20.0"
ai: Q3 revenue was $4.2M, representing 20% growth over Q2's $3.5M.
```

---

## 🏁 When to Use LangGraph vs Simple Chain

| Use case | Use Chain | Use LangGraph |
|----------|----------|--------------|
| Simple Q&A | ✅ | |
| One-shot retrieval | ✅ | |
| Multi-step logic | | ✅ |
| Retry on failure | | ✅ |
| Multi-tool agent | | ✅ |
| Multi-turn chat with memory | | ✅ |
| Complex conditional branching | | ✅ |
| Human-in-the-loop workflows | | ✅ |

---

## ✅ Full Recap

| Term | Definition |
|------|-----------|
| **Agent** | An LLM that can choose and use tools to complete tasks |
| **LangGraph** | Library for building agent workflows as graphs |
| **State** | Shared data dictionary flowing through all nodes |
| **Node** | A step in the graph — a function that transforms state |
| **Edge** | Connection from one node to the next |
| **Conditional Edge** | Edge that branches based on logic |
| **MemorySaver** | Persists state across conversation turns |
| **Thread ID** | Unique ID for each conversation/user |
| **Tool** | A function the agent can choose to call |
| **ReAct** | Agent pattern: Reason → Act → Observe → Repeat |

---

> ➡️ **Next:** [Module 9 — Build Your RAG System](./09_build_your_rag.md)
