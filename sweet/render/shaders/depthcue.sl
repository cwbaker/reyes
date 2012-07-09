volume depthcue(
    float mindistance = 0, maxdistance = 1;
    color background = 0;
)
{
    float d;
    d = clamp((depth(P) - mindistance) / (maxdistance - mindistance), 0.0, 1.0);
    Ci = mix(Ci, background, d);
    Oi = mix(Oi, color(1, 1, 1), d);
}