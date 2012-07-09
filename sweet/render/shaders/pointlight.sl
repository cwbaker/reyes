light pointlight(
    float intensity = 1;
    color lightcolor = 1;
    point from = point "shader" (0,0,0); 
)
{
    illuminate(from)
        Cl = intensity * lightcolor / L.L;
}
