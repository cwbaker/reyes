displacement wavy(
    float Km = 1;
    float sfreq = 32;
    float tfreq = 32;
)
{
    float amp = Km * sin(sfreq * s) * cos(tfreq * t);
    P += amp * normalize(N);
    N = calculatenormal(P);
}
