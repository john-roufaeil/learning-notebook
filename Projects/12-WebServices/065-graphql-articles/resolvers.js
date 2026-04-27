const { users, articles, comments } = require('./data');

const resolvers = {
  Query: {
    users: (parent, { role }) => {
      if (role) return users.filter(user => user.role === role);
      return users;
    },
    user: (parent, { id }) => users.find(user => user.id === id),

    articles: () => articles,
    article: (parent, { id }) => articles.find(article => article.id === id),

    comments: () => comments,
    comment: (parent, { id }) => comments.find(comment => comment.id === id),
  },

  Mutation: {
    createArticle: (parent, { title, content, author }) => {
      const newId = String(articles.length + 1);

      const newArticle = {
        id: newId,
        title,
        content: content || '',
        authorId: author,
      };

      articles.push(newArticle);
      return newArticle;
    },
  },

  Article: {
    author: (parent) => users.find(user => user.id === parent.authorId),
    comments: (parent) => comments.filter(comment => comment.articleId === parent.id),
  },

  Comment: {
    article: (parent) => articles.find(article => article.id === parent.articleId),
  },
};

module.exports = resolvers;