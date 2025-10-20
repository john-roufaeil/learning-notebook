# Babel

Babel is a popular JavaScript compiler widely adopted within the React community.

We want to use features of the language
that have not yet been added in the browser, our target environment. Those advanced features make our code cleaner for developers, but the browser cannot understand and execute it.
The solution is to write our scripts in JSX and ES2015, and when we are ready to ship, we compile the sources into ES5, the standard specification implemented in major browsers today.

Babel can compile ES2015 code into ES5 JavaScript, as well as compile JSX into JavaScript functions. The process is called transpilation, because it compiles the source into a new source rather than into an executable.

Using it is pretty straightforward; we just install it:

```bash
 npm install -g babel-cli
```

to compile any JavaScript file:

 ```bash
 babel source.js -o output.js
 ```

Babel is a tool to transpile a source file into an output file, and could be configured to apply some transformations.

```bash
npm install --global babel-preset-es2015
 babel-preset-react
```

in /.babelrc

```json
{
 "presets": [
    "es2015",
    "react"
 ]
}
```
