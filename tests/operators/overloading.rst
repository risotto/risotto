func (i int) incr1() {
    i = i + 1
}

func (i &int) incr2() {
    i = i + 1
}

op (i &int) - (i2 int) {
    println("CALLED")
}

a := 1

println(a)

a.incr1()
println(a)

a.incr2()
println(a)

1 - 2
