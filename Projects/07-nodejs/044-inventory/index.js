const fs = require('fs');
const path = require('path');

// Gather the todos from file, command and value from args
// -------------------------------------------------------

const todosFilePath = path.join(__dirname, 'todos.json');
let fileContent, parsedFile, parsedTodos, itemsId;
// either file doesn't exist, or not having any of the correct fields or not json file

try {
    fileContent = fs.readFileSync(todosFilePath, 'utf-8');
    parsedFile = JSON.parse(fileContent);
    if (!parsedFile["data"] || !parsedFile["nextItemId"]) {
        throw error;
    }
    parsedTodos = parsedFile["data"];
    itemsId = parsedFile["nextItemId"];
} catch {
    console.log("todos.json file doesn't exist or not correctly formatted. Resetting database.");
    fs.writeFileSync(todosFilePath, '{"data":[], "nextItemId":0}');
    fileContent = fs.readFileSync(todosFilePath, 'utf-8');
    parsedFile = JSON.parse(fileContent);
    parsedTodos = parsedFile["data"];
    itemsId = parsedFile["nextItemId"];
}

// TODO: figure the largest itemsId in file and set it
const [, , command, value1, value2] = process.argv;


// Interpret command and do corresponding action
// ---------------------------------------------

if (command === 'add') {
    // Add to inventory - node index.js add [name]
    if (!value1) {
        console.log("Incorrect input.\nCorrect input is: node index.js add [name]")
        return;
    }
    let newItem = {
        name: value1,
        quantity: 1,
        category: "General",
        id: itemsId
    };
    const newTodos = parsedTodos.concat(newItem);
    fs.writeFileSync(todosFilePath, `{"data": ${JSON.stringify(newTodos)}, "nextItemId": ${itemsId + 1}}`);

} else if (command === 'destock') {
    // Destock item - node index.js destock [id] [quantity]
    if (!value1 || !value2) {
        console.log("Incorrect input.\nCorrect input is: node index.js destock [id] [quantity]")
        return;
    }

    const oldTodo = parsedTodos.filter((todo) => todo.id == parseInt(value1))[0];
    if (!oldTodo) {
        console.log("Item not found.")
        return;
    }
    if (oldTodo.quantity < parseInt(value2)) {
        console.log(`Not enough stock to destock ${value2} from ${oldTodo.name}. ${oldTodo.quantity} is available.`);
        return;
    }

    const newTodo = { ...oldTodo, quantity: oldTodo.quantity - parseInt(value2) };
    let newTodos = parsedTodos.filter((todo) => todo.id != parseInt(value1));
    newTodos = newTodos.concat(newTodo);
    fs.writeFileSync(todosFilePath, `{"data": ${JSON.stringify(newTodos)}, "nextItemId": ${itemsId}}`);

} else if (command === 'restock') {
    // Restock item - node index.js restock [id] [quantity]
    if (!value1 || !value2) {
        console.log("Incorrect input.\nCorrect input is: node index.js restock [id] [quantity]")
        return;
    }

    const oldTodo = parsedTodos.filter((todo) => todo.id == parseInt(value1))[0];
    if (!oldTodo) {
        console.log("Item not found.")
        return;
    }

    const newTodo = { ...oldTodo, quantity: oldTodo.quantity + parseInt(value2) };
    let newTodos = parsedTodos.filter((todo) => todo.id != parseInt(value1));
    newTodos = newTodos.concat(newTodo);
    fs.writeFileSync(todosFilePath, `{"data": ${JSON.stringify(newTodos)}, "nextItemId": ${itemsId}}`);

} else if (command === 'edit') {
    // Edit item name - node index.js edit [id] [new name]
    if (!value1 || !value2) {
        console.log("Incorrect input.\nCorrect input is: node index.js edit [id] [new name]")
        return;
    }

    const oldTodo = parsedTodos.filter((todo) => todo.id == parseInt(value1))[0];
    if (!oldTodo) {
        console.log("Item not found.")
        return;
    }

    const newTodo = { ...oldTodo, name: value2 };
    let newTodos = parsedTodos.filter((todo) => todo.id != parseInt(value1));
    newTodos = newTodos.concat(newTodo);
    fs.writeFileSync(todosFilePath, `{"data": ${JSON.stringify(newTodos)}, "nextItemId": ${itemsId}}`);

} else if (command === 'delete') {
    // Delete item - node index.js delete [id]
    if (!value1) {
        console.log("Incorrect input.\nCorrect input is: node index.js delete [id]")
        return;
    }

    let found = parsedTodos.find((todo) => todo.id === parseInt(value1));
    if (!found) {
        console.log("Item not found.");
        return;
    }

    const newTodos = parsedTodos.filter((todo) => todo.id !== parseInt(value1));
    fs.writeFileSync(todosFilePath, `{"data": ${JSON.stringify(newTodos)}, "nextItemId": ${itemsId}}`);

} else if (command === 'list') {
    // List items - node index.js list
    // console.log("ID\tName\t\tCategory\tStatus\t\tStock")
    // parsedTodos.map((todo) => {
    //     const todoStatus = todo.quantity > 2 ? "available" : todo.quantity > 0 ? "low stock" : "out of stock";
    //     console.log(`${todo.id}\t${todo.name}\t\t${todo.category}\t\t${todoStatus}\t${todo.quantity}`);
    // })
    console.table(parsedTodos.map((todo) => ({
        ID: todo.id,
        Name: todo.name,
        Category: todo.category,
        Status: todo.quantity > 2 ? "available" : todo.quantity > 0 ? "low stock" : "out of stock",
        Stock: todo.quantity
    })))

} else if (command === 'summary') {
    // Print summary - node index.js summary
    const itemsCount = parsedTodos.length;
    const quantities = parsedTodos.map((todo) => todo.quantity);
    const totalQuanities = quantities.reduce((partialSum, quantity) => partialSum + quantity, 0);
    const availableCount = quantities.filter((quantity) => quantity > 2).length;
    const lowStockCount = quantities.filter((quantity) => (quantity <= 2) && (quantity > 0)).length;
    const outOfStockCount = quantities.filter((quantity) => quantity <= 0).length;
    console.log(`
    SUMMARY OF INVENTORY
    --------------------
    Total number of items: ${itemsCount}
    Total quantity of all items: ${totalQuanities}
    Number of available items: ${availableCount}
    Number of low stock items: ${lowStockCount}
    Number of out of stock items: ${outOfStockCount}
    `)

} else {
    console.log("Commands allowed are: add/destock/restock/edit/delete/list/summary.");
    return;
}