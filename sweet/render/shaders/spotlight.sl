light spotlight(
    float intensity = 1;
    color lightcolor = 1;
    point from = point "shader" (0,0,0);
    point to = point "shader" (0,0,1);
    float coneangle = radians(30);
    float conedeltaangle = radians(5);
    float beamdistribution = 2;
)
{
    float atten, cosangle;
    uniform vector A = (to - from)/length(to-from);
    illuminate(from, A, coneangle) {
        cosangle = L . A / length(L);
        atten = pow(cosangle, beamdistribution) / L.L;
        atten *= smoothstep(cos(coneangle),
        cos(coneangle - conedeltaangle), cosangle);
        Cl = atten * intensity * lightcolor;
    }
}