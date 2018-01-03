light shadowpointlight(
    float intensity = 1;
    color lightcolor = 1;
    point from = point "shader" (0,0,0); 
    string shadowname = "";
    float bias = 0.001;
)
{
    illuminate(from)
        Cl = shadow(shadowname, Ps, bias) * intensity * lightcolor / L.L;
}
