type Person struct {
    A1 int;
    A2 int;
    A3 int;
}

new (p Person) (a1 int, a2 int, a3 int) Person {
    p.A1 = a1
    p.A2 = a2
    p.A3 = a3

    return p
}

p := new Person(1, 2, 3)

println(p.A1)
println(p.A2)
println(p.A3)
