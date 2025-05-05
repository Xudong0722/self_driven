def fib(n):
    """
    Calculates the nth number in the Fibonacci sequence.

    Args:
        n (int): The index of the Fibonacci number to calculate.

    Returns:
        int: The nth Fibonacci number.
    """
    if n == 0 or n == 1:
        return n
    return fib(n - 1) + fib(n - 2)