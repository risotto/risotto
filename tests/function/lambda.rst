printer := func (i int, s string) {
    println(i+s)
}

printer(42, " IT WORKS")

func printIt(printItPrinter func (i int, s string)) {
    printItPrinter(43, " IT REALLY WORKS")
}

printIt(printer)
