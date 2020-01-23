srand() // Seed random

func abs(i int) int {
    if i < 0 {
        return -1 * i
    }

    return i
}

type Ball struct {
    x int;
    y int;
    xVel int;
    yVel int;
}

new (b Ball) () Ball {
    b.x = rand() % 500
    b.y = rand() % 500
    b.xVel = (rand() % 300) - 150
    b.yVel = (rand() % 300) - 150

    return b
}

func (b Ball) bounce() bool {
    xLimit := 500
    yLimit := 500
    bounced := false

    b.x += b.xVel
    b.y += b.yVel

    if (b.x > xLimit) {
        b.x = xLimit
        b.xVel = 0 - abs(b.xVel)
        bounced = true
    }

    if (b.x < 0) {
        b.x = 0
        b.xVel = abs(b.xVel)
        bounced = true
    }

    if (b.y > yLimit) {
        b.y = yLimit
        b.yVel = 0 - abs(b.yVel)
        bounced = true
    }

    if (b.y < 0) {
        b.y = 0
        b.yVel = abs(b.yVel)
        bounced = true
    }

    return bounced
}

// Run

ballCount := 100
bounces   := 0
balls := []Ball{}

for i := 0; i < ballCount; i += 1 {
    balls.push(new Ball())
}

for i := 0; i < 50; i += 1 {
    for b := 0; b < ballCount; b += 1 {
        ball := balls[b]

        if (ball.bounce()) {
          bounces += 1
        }
    }
}

println(bounces)
