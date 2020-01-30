type Permute struct {
    count int;
    v []int;
}

new (p Permute) () Permute {
    return p
}

func (p Permute) run() int {
    p.count = 0
    p.v     = []int{0, 0, 0, 0, 0, 0}
    p.permute(6)

    return p.count
}

func (p Permute) permute(n int) {
    p.count += 1
    if (n != 0) {
      n1 := n - 1
      p.permute(n1)
      for i := n1+0; i >= 0; i -= 1 {
        p.swap(n1, i)
        p.permute(n1)
        p.swap(n1, i)
      }
    }
}

func (p Permute) swap(i int, j int) {
    tmp := p.v[i]
    p.v[i] = p.v[j]
    p.v[j] = tmp
}

p := new Permute()

println(p.run())
