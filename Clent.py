#!/usr/local/python2.7
# -*- encoding:utf-8 -*-

import socket

host = '192.168.163.130'
port = 8000

sockfd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sockfd.connect((host,port))
print sockfd.recv(1024).decode('utf-8')
