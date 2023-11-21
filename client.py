import xmlrpc.client
proxy = xmlrpc.client.ServerProxy('http://localhost:8000/')

print("client is created....")
while(True):
    c=int(input("\t1.Factorial \n\t2.Square \n\t3.EXIT\n"))
    if(c==1):
        a=int(input("Enter number to find factorial : "))
        print("Factorial of ",a," is : %s" % str(proxy.factorial_rpc(a)))
    if(c==2):
        b=int(input("Enter number to find square : "))
        print("Square of ", b," is : %s" % str(proxy.square_rpc(b)))
    if(c==3):
        break
    if(c>3 or c<0):
        print("Enter valid option")
        