# Node.js C++ addon尝试

## 简述

​	该项目主要是想用v8引擎提供的api来用C++开发一个简单的Node.js插件，并使用该插件。提供一个静态页面，页面内容是一个游戏手柄，点击相应的按钮，会对应键盘上的A、S、D等游戏常用按键，通过web socket，在局域网中发送置服务器（windows）。而C++插件中用到了Windows.h头文件，可以发起模拟按键的事件，从而在运行该服务器的计算机上进行控制。实现的功能就是把手机当作游戏手柄，在局域网内远程操作计算机，如果计算机正在运行游戏，则可以实现游戏手柄的功能。

## 说明

​	public目录下的entry.html为用于交互的静态html文件。

​	env_install目录下的env.md记录了visual studio中配置Node.js C++ addon开发环境的步骤。

​	build目录下的文件即为在visual studio中配好开发环境所创建的C++ addon项目。

​	该目录下的server.js为Node.js服务器文件。

## 运行

```bash
node server.js
```

手机或电脑局域网内连接开启服务器的服务器，输入网址http://xxx.xxx.xxx.xxx:8081/entry即可进入交互的页面。