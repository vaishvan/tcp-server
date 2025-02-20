first attempt at a rudimentary single thread tcp server written in cpp. uses winsock so only works on windows. ipv4, tryna add ipv6. 
run the client and server in two different terminals to send messages.

Known issues
-
1. Chat can only be altered between the server and client - once the client sends the message, the server must send a message back; there is no way to allow for multiple messages
2. Text is being sent as an array, and thus there is an upper limit of how many characters can be sent
3. Only Windows compatible (self-explanatory: Winsock)
