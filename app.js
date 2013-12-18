var App = function() {
	var that = this
	this.minDelta = 0.05
	this.x = null
	this.y = null
	this.serverAddress = null
	this.socket = null
	
	this.run = function() {
		// init socket connection
		that.socket = socketConnect(that.socket, that.serverAddress);
		
		// register listeners
		$(window).on('resize', function() {
			indicateWindowSize()
		})
		indicateWindowSize()
		
		$(document).on('vmousemove', function(e) {
			e.preventDefault(); // prevent scroll
			
			var x = e.pageX / that.windowX
			var y = e.pageY / that.windowY
			
			if(that.x == null || that.y == null || Math.abs(that.x - x) > that.minDelta || Math.abs(that.y - y) > that.minDelta) {
				that.x = x
				that.y = y
				
				var data = {
					x: x,
					y: y
				}
				
				that.socket.emit('mouseMoveToPercent', data)
				console.log(data)
			}
		})
	}
	
	var indicateWindowSize = function() {
		that.windowX = $(window).width()
		that.windowY = $(window).height()
	}
	
	var socketConnect = function(socket, endpoint) {
		var socket;
		
		if(!socket) {
			socket = io.connect(endpoint, {
				reconnect: false
			});
		} else {
			socket.removeAllListeners();
			socket.disconnect();
			socket.socket.connect(endpoint);
		}

		return socket;
	};
	
	var socketDisconnect = function(socket) {
		if(socket) {
			socket.disconnect();
			socket.removeAllListeners();
		}
	};
}
