func fib(n int) int {
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

println(fib(30))
