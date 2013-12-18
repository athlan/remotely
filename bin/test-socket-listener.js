var net = require('net');

// connection listener
var server = net.createServer(function(c) {
        console.log('server connected');
        
        c.on('end', function() {
                console.log('server disconnected');
        });
        
        c.on('data', function(data) {
                console.log(data.toString());
                
                //c.write("pong");
                
                //client.end();
        });
        
        c.write('hello new client!!\r\n');
        //c.pipe(c);
});

// listen server
server.listen(8081, function() {
        console.log('server bound');
});

