func fib(n int) int {
    if n == 0 {
        return 0
    }

    if n == 1 {
        return 1
    }

    return fib(n-1) + fib(n-2)
}

println(fib(25))

func fib2(n int) int {
    a := 0
    b := 1
    if n < 0 {
        println("Incorrect input")
    } else if n == 0 {
        return a
    } else if n == 1 {
        return b
    } else {
        for i := 2; i <= n; i+=1 {
            c := a + b
            a = b
            b = c
        }

        return b
    }
}

println(fib2(25))
