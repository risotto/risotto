type Random struct {
    seed int;
}

new (r Random) () Random {
    r.seed = 74755

    return r
}

func (r Random) next() int {
    r.seed = ((r.seed * 1309) + 13849) & 65535

    return r.seed
}

type Storage struct {
    count int;
}

new (s Storage) () Storage {
    s.count = 0

    return s
}

func (s Storage) buildTreeDepth(depth int, random Random) interface{} {
    s.count += 1
    if (depth == 1) {
        s := random.next() % 10 + 1
        a := []bool{}
        for i := 0; i < s; i +=1 {
            a.push(true)
        }
        return a
    } else {
      arr := []interface{}{}
      for i := 0; i < 4; i +=1 {
          arr.push(s.buildTreeDepth(depth - 1, random))
      }
      return arr
    }
}

r := new Random()
s := new Storage()
s.buildTreeDepth(7, r)

println(s.count)
