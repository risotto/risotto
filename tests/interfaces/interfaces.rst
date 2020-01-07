type Human interface {
    Role(greet string) string;
}

// Student

type Student struct {
}

new (s &Student) () Student {
    return s
}

func (s &Student) Role(greet string) string {
    return greet+" Student"
}

// Teacher

type Teacher struct {
}

new (t &Teacher) () Teacher {
    return t
}

func (t &Teacher) Role(greet string) string {
    return greet+" Teacher"
}

// Test

s Human := new Student()
t Human := new Teacher()

println(s.Role("Hey!"))
println(t.Role("Hello."))
