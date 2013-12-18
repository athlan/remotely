var app = require('express')()
  , server = require('http').createServer(app)
  , io = require('socket.io').listen(server)
  , net = require('net');

server.listen(8080);

io.set('log level', 1); // reduce logging
io.sockets.on('connection', function (socket) {
	
	var client = null
	var clientConnected = false
	var that = this
	
	socket.on('connectToRemote', function(data) {
		var host = data.host || null
		var port = data.port || null
		
		if(host == null || port == null) {
			socket.emit('error', createErrorPacket('Invalid remote params.'))
			socket.close()
			return
		}
		
		client = net.connect(port, host, function() {
			clientConnected = true
			console.log('connected!!!!!')
		})
		
		// register listeners
		client.on('error', function(e) {
			clientConnected = false
			socket.emit('error', createErrorPacket('Remote connection error.', e))
			socket.disconnect()
		})
		client.on('end', function() {
			clientConnected = false
			socket.emit('error', createErrorPacket('Remote connection end.'))
			socket.disconnect()
		})
	})
	
	socket.on('mouseMoveToPercent', function(data) {
		console.log(data)
		console.log(clientConnected)
		
		if(clientConnected == true) {
			client.write("x:" + data.x + ":y:" + data.y + ";")
		}
	})
});

var createErrorPacket = function(message, data) {
	return {
		message: message || "Unknown error",
		data: data || null
	}
}
