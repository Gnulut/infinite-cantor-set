#pragma once

#include <SDL3/SDL.h>

class cantor_set
{
public:
    double l=0.0, r=1.0;

    // expand/zoom the set size by (x>1.0) multiplier, center of expansion determines where to zoom
    void expand(double multiplier, double center_of_expansion);

    // removes unecessary part that won't be rendered;
    void recenter();

    // checks whether this instance is empty or blank
    bool is_empty() const;

    void render(SDL_Renderer* renderer, SDL_Window* window, const SDL_FColor* color) const;

};

void expand_cantor(cantor_set* cantor, double multiplier, int window_width);