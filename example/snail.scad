// Pascal's snail as cylinder

a = 100;
l = a * sqrt (2);
h = 200;

function x(fi) = (a * cos (fi) + l) * cos (fi);
function y(fi) = (a * cos (fi) + l) * sin (fi);

ps = [
    for (fi = [0 : 2 : 360]) [x (fi), y (fi)]
];

translate ([0, 0, -h/2])
    rotate ([0, 0, 90])
        linear_extrude (height = h)
            polygon (points = ps);
