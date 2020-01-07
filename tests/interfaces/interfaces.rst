type Human interface {
    Role() string;
}

// Student

type Student struct {
}

new (s &Student) () Student {
    return s
}

func (s &Student) Role() string {
    return "Student"
}

// Teacher

type Teacher struct {
    name string
}

new (t &Teacher) () Teacher {
    return t
}

func (t &Teacher) Role() string {
    return "Teacher"
}

// Test

s Human := new Student()
t Human := new Teacher()

println(s.Role())
println(t.Role())
