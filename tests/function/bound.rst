func a() string {
    return "working"
}

println(a())

func (i int) plus2() int {
    return i + 2
}

func (i int) shout() {
    println("HI I'M "+i.plus2())
    println("HI I'M "+i)
}

30.shout()
