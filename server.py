from xmlrpc.server import SimpleXMLRPCServer

def factorial(n):
    fact = 1
    if(n<0):
        return -1
    for i in range(1, n+1):
        fact = fact * i
    return fact


def square(n):
    print("square request arrived ")

    sq = n*n
    return sq


port = 8000
server = SimpleXMLRPCServer(("localhost", port), logRequests=False)## imp
server.register_function(factorial, 'factorial_rpc')
server.register_function(square, 'square_rpc')

try:
    print("Server started on the port ", port)
    server.serve_forever()

except:
    print("Exit")