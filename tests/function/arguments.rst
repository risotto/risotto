func testFunction1() int {
    return 0
}

testFunction1()

func testFunction2(a1 int, a2 int, a3 int) string {
    println(a1)
    println(a2)
    println(a3)

    return a1 + " " + a2 + " " + a3
}

println(testFunction2(1, 2, 3))

func testFunction3() {
    println("test")
}

testFunction3()

println("done")
