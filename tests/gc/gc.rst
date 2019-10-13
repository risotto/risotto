type Pet struct {
}

type Person struct {
    Name string;
    Pets []Pet;
}

new (p Person) () Person {
    p.Name = "TEST"
    p.Pets = []Pet{}

    return p
}

{
    println("#####################################")

    for i := 0; i < 5; i += 1 {
        ui := new Person()
    }

    vm_stats()
    gc()
    vm_stats()
}

{
    println("#####################################")

    u0 := new Person()

    {
        u1 := new Person()
        u2 := new Person()
        u3 := new Person()
    }

    vm_stats()
    gc()
    vm_stats()
}

{
    println("#####################################")

    vm_stats()

    for i := 0; i < 950; i += 1 {
        ui := new Person()
    }

    vm_stats()
    gc()
    vm_stats()

    println("===============================================")

    gc()
    vm_stats()
}
