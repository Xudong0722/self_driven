def func():
    print("a")
    yield
    print("b")
    yield
    print("c")

def A():
    co = func()
    next(co)
    print("In function A")
    next(co)

A()