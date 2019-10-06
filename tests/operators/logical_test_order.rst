fun bool false_fun() {
    print "false_fun";
    return false;
}

fun bool true_fun() {
    print "true_fun";
    return true;
}

if(false_fun() || true_fun()) {
    print "or > test1";
}

if(true_fun() || false_fun()) {
    print "or > test2";
}

if(true_fun() || true_fun()) {
    print "or > test3";
}

if(false_fun() || false_fun()) {
    print "or > test4";
}



if(false_fun() && true_fun()) {
    print "and > test1";
}

if(true_fun() && false_fun()) {
    print "and > test2";
}

if(true_fun() && true_fun()) {
    print "and > test3";
}

if(false_fun() && false_fun()) {
    print "and > test4";
}
