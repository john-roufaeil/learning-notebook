const http = require('http');

const {
    testConnection, listInventory, addItem, returnResource
} = require('./server-actions');

const server = http.createServer((req, res) => {
    const { method, url } = req;
    console.log(url);
    if (method === 'GET') {
        if (url === '/test') {
            testConnection(res);
        } else if (url === '/') {
            listInventory(res);
        } else if (url === '/astronomy') {
            returnResource(res, 'astronomy.html', 'text/html');
        } else if (url === '/serbal') {
            returnResource(res, 'serbal.html', 'text/html');
        } else if (url === '/styles.css') {
            returnResource(res, 'styles.css', 'text/css');
        } else if (url === '/astronomy.jpg') {
            returnResource(res, 'astronomy.jpg', 'image/jpeg');
        } else if (url === '/serbal.jpeg') {
            returnResource(res, 'serbal.jpeg', 'image/jpeg');
        } else {
            returnResource(res, 'not-found.html', 'text/html');
        }
    } else if (url === '/' && method === 'POST') {
        addItem(req, res);
    } else {
        returnResource(res, 'not-found.html', 'text/html');
    }

});

server.listen(3000, () => {
    console.log('Server is listening on localhost:3000')
})