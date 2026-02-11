const http = require('http');

const {
    testConnection, returnNotFoundPage, listInventory,
    addItem, returnAstronomyPage, returnSerbalPage, returnCSS,
    returnAstronomyImg, returnSerbalImg
} = require('./server-actions');

const server = http.createServer((req, res) => {
    const { method, url } = req;
    console.log(url);
    if (url === '/test' && method === 'GET') {
        testConnection(res);
    } else if (url === '/' && method === 'GET') {
        listInventory(res);
    } else if (url === '/' && method === 'POST') {
        addItem(req, res);
    } else if (url === '/astronomy' && method === 'GET') {
        returnAstronomyPage(res);
    } else if (url === '/serbal' && method === 'GET') {
        returnSerbalPage(res);
    } else if (url === '/styles.css' && method === 'GET') {
        returnCSS(res);
    } else if (url === '/astronomy.jpg' && method === 'GET') {
        returnAstronomyImg(res);
    } else if (url === '/serbal.jpeg' && method === 'GET') {
        returnSerbalImg(res);
    } else {
        returnNotFoundPage(res);
    }
});

server.listen(3000, () => {
    console.log('Server is listening on localhost:3000')
})