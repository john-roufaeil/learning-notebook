const users = [
  { id: "1", fullname: "John Doe", email: "john@example.com", dob: "1985-01-01", role: "ADMIN" },
  { id: "2", fullname: "Jane Smith", email: "jane@example.com", dob: "1990-05-15", role: "USER" },
  { id: "3", fullname: "Alice Johnson", email: "alice@example.com", dob: "1988-08-20", role: "USER" },
  { id: "4", fullname: "Bob Brown", email: "bob@example.com", dob: "1987-12-10", role: "USER" },
  { id: "5", fullname: "Charlie Davis", email: "charlie@example.com", dob: "1989-03-25", role: "USER" }
];

const articles = [
  { id: "1", title: "GraphQL Basics", content: "Learn the basics of GraphQL.", authorId: "1" },
  { id: "2", title: "Advanced GraphQL", content: "Dive deeper into GraphQL features.", authorId: "2" },
  { id: "3", title: "GraphQL vs REST", content: "Compare GraphQL with REST APIs.", authorId: "1" },
  { id: "4", title: "GraphQL Tools", content: "Explore tools for GraphQL development.", authorId: "3" },
  { id: "5", title: "GraphQL Performance", content: "Optimize GraphQL performance.", authorId: "2" },
];

const comments = [
  { id: "1", title: "Great article!", content: "I found this very helpful.", articleId: "1" },
  { id: "2", title: "Needs more examples", content: "Could you add more code samples?", articleId: "1" },
  { id: "3", title: "Very informative", content: "Thanks for sharing!", articleId: "2" },
  { id: "4", title: "I disagree", content: "I think REST is better for some use cases.", articleId: "3" },
  { id: "5", title: "Well explained", content: "The comparison was very clear.", articleId: "3" },
  { id: "6", title: "Question about GraphQL", content: "How does GraphQL handle authentication?", articleId: "2" },
  { id: "7", title: "Typo in the article", content: "There's a typo in the second paragraph.", articleId: "1" },
  { id: "8", title: "More on GraphQL", content: "Can you write about GraphQL subscriptions?", articleId: "2" },
  { id: "9", title: "GraphQL performance", content: "How does GraphQL perform with large datasets?", articleId: "3" },
  { id: "10", title: "GraphQL tools", content: "What are some good tools for GraphQL development?", articleId: "1" },
];

module.exports = { users, articles, comments };