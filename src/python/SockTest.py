#!/usr/bin/python2.7
# -*- coding:utf-8 -*-
import socket
import threading
	
host = '192.168.163.132'
port = 8000

print "hello world"

def Recv(sockfd, addr):
	print "Accept Success.."
	# -*- recv 1024 bytes from clent-*-
	sockfd.send(b'Welcome to Connect..')
	data = sockfd.recv(1024)
	if not data or data.decode('utf-8') == 'exit':
		print "Nothing to Recv.."	
	else:
		print data


def main():
        print "hello socket"
	sockfd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	sockfd.bind((host,port))
	sockfd.listen(8)
	print "Wait For Connect..."
	while True:
		i = 0
		# -*- accept return Clent's Socket and Clent's Address -*- 
		sock, addr=sockfd.accept()
		try:
			threading.Thread(target = Recv,args = (sock, addr))
		except:
			print "Error:unable to create new thread"


main()
