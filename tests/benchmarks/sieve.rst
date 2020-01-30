int sieve(flags []boolean, size int) {
    primeCount := 0

    for i := 2; i <= size; i += 1 {
      if (flags[i - 1]) {
        primeCount += 1
        k := i + i
        while (k <= size) {
          flags[k - 1] = false
          k += i
        }
      }
    }
    return primeCount
}

flags := []boolean{}
for i := 0; i < 5000; i += 1 {
    flags.push(true)
}

println(sieve(flags, 5000))
