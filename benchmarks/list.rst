type Element struct {
    val int;
    next Element;
}

new (e Element) (v int) Element {
    e.val = v

    return e
}

func (e Element) length() int {
    if e.next == nil {
        return 1
    } else {
        return 1 + e.next.length()
    }
}

func isShorterThan(x Element, y Element) bool {
    xTail := x
    yTail := y

    while (yTail != nil) {
       if (xTail == nil) {
         return true
       }

       xTail = xTail.next
       yTail = yTail.next
    }

    return false
}

func tail(x Element, y Element, z Element) Element {
    if (isShorterThan(y, x)) {
      return tail(
        tail(x.next, y, z),
        tail(y.next, z, x),
        tail(z.next, x, y)
      )
    } else {
      return z
    }
}

func makeList(length int) Element {
    if (length == 0) {
        return nil
    } else {
        e := new Element(length)
        e.next = makeList(length - 1)

        return e
    }
}

// Run

result := tail(makeList(15), makeList(10), makeList(6))
println(result.length())
