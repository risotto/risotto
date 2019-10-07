func false_fun() bool {
    println("false_fun")
    return false
}

func true_fun() bool {
    println("true_fun")
    return true
}


if(false_fun() || true_fun()) {
    println("or > test1")
}

if(true_fun() || false_fun()) {
    println("or > test2")
}

if(true_fun() || true_fun()) {
    println("or > test3")
}

if(false_fun() || false_fun()) {
    println("or > test4")
}



if(false_fun() && true_fun()) {
    println("and > test1")
}

if(true_fun() && false_fun()) {
    println("and > test2")
}

if(true_fun() && true_fun()) {
    println("and > test3")
}

if(false_fun() && false_fun()) {
    println("and > test4")
}
