import time
import socket
HOST = "192.168.178.42"    # The remote host
PORT = 5555             # The same port as used by the server

s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

while True:
    #s.send(bytes("SELECT:CH1\n", "utf-8"))
    s.send(bytes("MEAS:ITEM? VAVG,CHAN1\n", "utf-8"))
    q = s.recv(1024)
    s.send(bytes("MEAS:ITEM? VAVG,CHAN2\n", "utf-8"))
    r = s.recv(1024)

    u = float(q.decode())
    i = float(r.decode())
    print(f"U={u}\tI={i}\tP={u*i}")
    with open("log.csv", "a") as f:

        out = f"{time.time()},{u},{i}\n"
        f.write(out)

    time.sleep(1)
s.close()
