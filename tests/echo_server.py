""""
a simple TCP echo server on IPv6
""""
import socket
import sys

BUFFSIZE = 16
SERV_PORT = 8888

if __name__ == '__main__':

    sock = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
    sock.bind(('', SERV_PORT))
    sock.listen(1)
    print "Starting serving on port %d" % SERV_PORT

    try:
        while True:
            conn, saddr = sock.accept()
            print "Connection form: %r:%d" % (saddr[0], saddr[1])
            try:
                data = ''
                while True:
                    d = conn.recv(BUFFSIZE)
                    if d:
                        data += d
                        conn.sendall(d)
                    else:
                        break
                print data
            finally:
                conn.close()
    except KeyboardInterrupt:
        sock.close()