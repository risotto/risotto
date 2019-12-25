f1()
f2()

p := new Person()
p.Name()

func f1() {
    println("1")
}

func f2() {
    println("2")
}

type Person struct {}

new (p &Person) () Person {
    return p
}

func (p &Person) Name() {
    println("My Name")
}
