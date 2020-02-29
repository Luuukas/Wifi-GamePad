var addon = require('.\\build\\Release\\win_sendkey.node');
var express = require('express');
var app = express();

var server = require('http').createServer(app);
var io = require('socket.io')(server);

app.use('/', express.static(__dirname + '/public'));
app.get('/',function(req,res){
  res.redirect('entry.html')
})
server.listen(8081);

//socket部分
var player_cnt = 0;

io.on('connection', function (socket) {
  if(player_cnt>=4){
    socket.emit('login',1);
  }else{
    socket.emit('login',0);
    ++player_cnt;
  }
  //接收并处理客户端的hi事件
  socket.on('presskey', function (data) {
    console.log(data);
    addon.do_press(data);
  })
  socket.on('releasekey', function (data) {
    console.log(data);
    addon.do_release(data);
  })

  //断开事件j
  socket.on('disconnect', function (data) {
    --player_cnt;
    console.log('A player left just now.')
  })

});