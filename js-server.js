const https = require('https');
const JSON = require('JSON');
const fs = require('fs');
const net=require('net');
const WWS = require('websocket').server;

const sslOptions = {
	key : fs.readFileSync('./your_private_key.pem'),
	cert : fs.readFileSync('./your_cert.pem')
};
const port = 3000;
const server_info = {
    'ip' : '127.0.0.1',
    'port' : 28000
};

const httpsServer = https.createServer(sslOptions, (req,res) => {
    res.writeHead(404);
    res.end();
});

httpsServer.listen(port, async () => {
    console.log(" START JS SERVER PORT ", port);
});

const wsServer = new WWS({
    httpServer: httpsServer,
    ssl: true,
    key : sslOptions.key,
    cert: sslOptions.cert
});

wsServer.on("request", (req)=> {
    let connection =  req.accept();
    let client = new net.Socket();
    client.connect(server_info.port, server_info.ip);
    client.on('data', (data) => {
        var intb = new Buffer.alloc(4);
        var strb = new Buffer.alloc(32);

        intb = data.readInt32LE(0);
        strb = data.toString("utf8",4,36).split('\u0000')[0];
        console.log(intb, strb)
        connection.sendUTF('{"int" : '+intb+', "str": "'+strb+'"}');
    });
    client.on('close', () => {
        console.log(" CLOSE client");
    })

    connection.on('message', (msg) => {
        console.log( msg )
        client.write(msg.utf8Data);
    });

    connection.on('close', () => {
        client.destroy();
        console.log(" USER OUT!");
    });
});
