const http = require('http');
const express = require('express');
const path = require('path');
const fs = require('fs');
const secrets = require('./express/secrets.json');

let heliumHeader = new Headers({
    "key": secrets.helium.key,
});

async function getDevices() {
    const response = await fetch("https://console.helium.com/api/v1/devices", {headers:heliumHeader});
    return response.json(); // parses JSON response into native JavaScript objects
}
  
getDevices().then((data) => {
    console.log(data); // JSON data parsed by `data.json()` call
});

const app = express();
app.use(express.json());
app.use(express.static("express"));
// default URL for website
app.get('/', function(request,response){
    console.log(request);
    response.sendFile(path.join(__dirname+'/express/index.html'));
    //__dirname : It will resolve to your project folder.
});

app.post('/', function(request,response){
    console.log(request.body);
    response.end('howdy do!');
    //__dirname : It will resolve to your project folder.
});

function updateUplink(){
    const fileName = './express/data/sensors.json';
    const file = require(fileName);
    file.key = "new value";
    fs.writeFile(fileName, JSON.stringify(file), function writeJSON(err) {
        if (err) return console.log(err);
    });
}


const server = http.createServer(app);
const port = 9000;
server.listen(port);
console.log('Server listening on port ' + port);