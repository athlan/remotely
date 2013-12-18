var app = require('express')()
  , server = require('http').createServer(app)
  , io = require('socket.io').listen(server);

server.listen(8080);

io.set('log level', 1); // reduce logging
io.sockets.on('connection', function (socket) {
	
	socket.on('mouseMoveToPercent', function(data) {
		console.log(data)
	})
});
