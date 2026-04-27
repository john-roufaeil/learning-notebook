const typeDefs = `#graphql
enum UserRole {
  USER
  ADMIN
}

type User {
  id: ID!
  fullname: String!
  email: String!
  dob: String
  role: UserRole!
}

type Article {
  id: ID!
  title: String!
  content: String!
  author: User!
  comments: [Comment!]!
}

type Comment {
  id: ID!
  title: String!
  content: String
  article: Article!
}

type Query {
  users(role: UserRole): [User!]!
  user(id: ID!): User

  articles: [Article!]!
  article(id: ID!): Article
  
  comments: [Comment!]!
  comment(id: ID!): Comment
}

input CreateArticleInput {
  title: String!
  content: String
  author: ID!
}

type Mutation {
  createArticle(input: CreateArticleInput!): Article!
}
`

module.exports = typeDefs;