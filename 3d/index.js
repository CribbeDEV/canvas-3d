const FOREGROUND = "#20C20E";
const BACKGROUND = "#101010";
let DRAW_POINTS = false;
var colorArray = [
        "#FF6633",
        "#FFB399",
        "#FF33FF",
        "#FFFF99",
        "#00B3E6",
        "#E6B333",
        "#3366E6",
        "#999966",
        "#809980",
        "#E6FF80",
        "#1AFF33",
        "#999933",
        "#FF3380",
        "#CCCC00",
        "#66E64D",
        "#4D80CC",
        "#FF4D4D",
        "#99E6E6",
        "#6666FF"
    ];

const ctx = game.getContext("2d");

game.width = 800;
game.height = 800;

function clear() {
    ctx.fillStyle = BACKGROUND;
    ctx.fillRect(0, 0, game.width, game.height);
}

function point(p, i) {
    ctx.font = "48px serif";
    const color = (i < colorArray.length ? colorArray[i] : "#FFFFFF")
    ctx.fillStyle = color;
    ctx.fillText(i, p.x, p.y);
    const s = 20;
    ctx.fillRect(p.x - s/2, p.y - s/2, s, s)
}

function screen(p) {
    // -1..1 => 0..2 => 0..1 => 0..w
    return {
        x: (p.x + 1)/2*game.width,
        y: (1 - (p.y + 1)/2)*game.height,
    }
}

function project({x, y, z}) {
    return {
        x: x/z,
        y: y/z
    }
}

function translate_z({x, y, z}, dz) {
    return {
        x, y, z: z+dz
    }
}

function rotate_xz({x, y, z}, angle) {
    const c = Math.cos(angle);
    const s = Math.sin(angle);
    return {
        x: x*c-z*s,
        y,
        z: x*s+z*c,
    };
}

function rotate_xy({x, y, z}, angle) {
    const c = Math.cos(angle);
    const s = Math.sin(angle);
    return {
        x: x*c-y*s,
        y: x*s+y*c,
        z,
    };
}

function line(p1, p2) {
    ctx.strokeStyle = FOREGROUND
    ctx.lineWidth = 5;
    ctx.lineJoin = "round";
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.closePath();
    ctx.stroke();
}

const vs = [
    {x:  0.25, y:  0.25, z:  0.25},
    {x: -0.25, y:  0.25, z:  0.25},
    {x: -0.25, y: -0.25, z:  0.25},
    {x:  0.25, y: -0.25, z:  0.25},

    {x:  0.25, y:  0.25, z: -0.25},
    {x: -0.25, y:  0.25, z: -0.25},
    {x: -0.25, y: -0.25, z: -0.25},
    {x:  0.25, y: -0.25, z: -0.25},

    {x:  0.0, y: 0.6, z:  0.0},
    {x:  0.0, y: -0.6, z:  0.0},
    {x:  0.6, y: 0.0, z:  0.0},
    {x:  -0.6, y: 0.0, z:  0.0},
    {x:  0.0, y: 0.0, z:  0.6},
    {x:  0.0, y: 0.0, z:  -0.6},
]

const fs = [
    [0, 1, 2, 3],
    [4, 5, 6, 7],
    [0, 4],
    [1, 5],
    [2, 6],
    [3, 7],
    [0, 8],
    [1, 8],
    [4, 8],
    [5, 8],
    [2, 9],
    [3, 9],
    [6, 9],
    [7, 9],
    [0, 10],
    [4, 10],
    [3, 10],
    [7, 10],
    [5, 11],
    [6, 11],
    [2, 11],
    [1, 11],
    [1, 12],
    [2, 12],
    [3, 12],
    [0, 12],
    [4, 13],
    [5, 13],
    [6, 13],
    [7, 13],
    

]

const FPS = 60;

let dz = 1;
let angle = 0;

function frame() {
    const dt = 0.5/FPS;

    angle+=Math.PI*dt;

    clear()
    if (DRAW_POINTS) {
        for (const [i, v] of vs.entries()) {
            point(screen(project(translate_z(rotate_xz(v, angle), dz))), i)
        }
    }
    for (const f of fs) {
        for (let i = 0; i < f.length; ++i) {
            const a = vs[f[i]];
            const b = vs[f[(i+1)%f.length]];
            line(screen(project(translate_z(rotate_xz(a, angle), dz))),
                 screen(project(translate_z(rotate_xz(b, angle), dz))))
            line(screen(project(translate_z(rotate_xy(rotate_xz(a, angle), angle), dz))),
                 screen(project(translate_z(rotate_xy(rotate_xz(b, angle), angle), dz))))
        }
    }

    setTimeout(frame, 1000/FPS);
}
setTimeout(frame, 1000/FPS);

console.log(ctx.getContextAttributes())

document.getElementById("draw").addEventListener('change', (e) => { 
    console.log(e.target.checked); 
    DRAW_POINTS = e.target.checked; 
})