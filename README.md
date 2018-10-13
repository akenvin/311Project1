# UnixDomainSocketTest

Both the server and the client are reading and writing on one single socket. This is one significant advantage in compare with FIFO.

I am not testing the multi-clients to single server case because that would require multi-process on the server side to make a good demo.
