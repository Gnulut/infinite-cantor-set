#include "cantor.hpp"

void cantor_set::expand(double multiplier, double center_of_expansion)
{
    if (this->is_empty()) return; // represents the condition where we have zoomed in on a blank space and there is nothing else
    if (multiplier <= 1.0) return; // function does not support shrinking
    if (center_of_expansion < 0.0 || 1.0 < center_of_expansion) return; // the source of center_of_expansion should be window mouse_x/width, which is [0, 1.0]
    multiplier -= 1.0;
    // length is always 1.0 window_width
    {// left
        double left_length = center_of_expansion - this->l; // divided by length which is 1.0
        double left_length_change = multiplier * left_length;
        this->l -= left_length_change;
    }
    {// right
        double right_length = this->r - center_of_expansion; // divided by length which is 1.0
        double right_length_change = multiplier * right_length;
        this->r += right_length_change;
    }
    this->recenter();
}

void cantor_set::recenter()
{
    if (this->is_empty()) return; // represents the condition where we have zoomed in on a blank space and there is nothing else
    double length = this->r-this->l;
    double p1 = this->l + length/3.0;
    double p2 = this->r - length/3.0;
    bool p1_ok = (0.0 <= p1 && p1 <= 1.0 || 0.0 <= this->l && this->l <= 1.0);
    bool p2_ok = (0.0 <= p2 && p2 <= 1.0 || 0.0 <= this->r && this->r <= 1.0);
    if (p1_ok && p2_ok) return;
    if (!p1_ok && !p2_ok) {
        this->l = 2.0;
        this->r = -1.0;
    }else if (!p1_ok) {
        this->l = p2;
    }else if (!p2_ok) {
        this->r = p1;
    }
    // im not sure if we need to do more than 1 recenter, so I'll do it again
    this->recenter();
}

bool cantor_set::is_empty() const
{
    return this->l>=this->r;
}

void cantor_set::render(SDL_Renderer* renderer, SDL_Window* window, const SDL_FColor* color) const
{
    if(renderer == nullptr || color == nullptr) return;
    if(this->is_empty()) return;
    
    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);
    double logical_length = (this->r - this->l);
    int image_width = logical_length * window_width;

    SDL_Texture * image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, image_width, window_height);
    {// first picture
        // color all
        SDL_SetRenderTarget(renderer, image);
        SDL_SetRenderDrawColorFloat(renderer, color->r, color->g, color->b, color->a);
        SDL_RenderClear(renderer);

        // blank middle
        SDL_SetRenderDrawColorFloat(renderer, 0.f, 0.f, 0.f, SDL_ALPHA_OPAQUE_FLOAT);
        SDL_FRect rect = { .x = (float)image_width/3.f, .y = 0.f, 
                           .w = (float)image_width/3.f, .h = (float)window_height};
        SDL_RenderFillRect(renderer, &rect);
    }
    {// fractal generation
        SDL_Texture * copy_image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, image_width, window_height);
        SDL_FRect rect = { .x = 0.f, .y = 0.f, 
                           .w = (float)image_width/3.f, .h = (float)window_height};
        for(int width_temporary = image_width/3; width_temporary>=3; width_temporary /= 3) {
            // copy
            SDL_SetRenderTarget(renderer, copy_image);
            SDL_RenderTexture(renderer, image, nullptr, nullptr);

            // fractal
            SDL_SetRenderTarget(renderer, image);
            for(int i=0; i<2; ++i){
                SDL_RenderTexture(renderer, copy_image, nullptr, &rect);
                rect.x += rect.w * 2.f;
            }
            rect.x = 0.f;
        }
        SDL_DestroyTexture(copy_image);
    }
    {// put image
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_FRect rect{ .x = (float)(this->l*window_width), .y = 0.f,
                        .w = (float)(image_width), .h = (float)window_height};
        SDL_RenderTexture(renderer, image, nullptr, &rect);
    }
    // release memory
    SDL_DestroyTexture(image);
}

void expand_cantor(cantor_set* cantor, double multiplier, int window_width)
{
    if(cantor==nullptr)return;
    float x;
    SDL_GetMouseState(&x, nullptr);
    cantor->expand(multiplier, x/(float)window_width);
}