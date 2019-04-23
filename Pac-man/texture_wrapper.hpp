//
//  texture_wrapper.hpp
//  pac-man
//
//

#ifndef texture_wrapper_hpp
#define texture_wrapper_hpp

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
//#include <SDL2_ttf/SDL_ttf.h>

#include <string>

class LTexture
{
public:
    //Initializes variables
    LTexture(SDL_Renderer* renderer);
    
    //Deallocates memory
    ~LTexture();
    
    //Loads image at specified path
    bool loadFromFile( std::string path );
    
#ifdef _SDL_TTF_H
    //Creates image from font string
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font);
#endif
    
    //Deallocates texture
    void free();
    
    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
    
    //Set blending
    void setBlendMode( SDL_BlendMode blending );
    
    //Set alpha modulation
    void setAlpha( Uint8 alpha );
    
    //Renders texture at given point
    void render( SDL_Point position, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
    
    //Gets image dimensions
    int getWidth();
    int getHeight();
    
    //Pixel manipulators
    bool lockTexture();
    bool unlockTexture();
    void* getPixels();
    int getPitch();
    Uint32 getPixel32( unsigned int x, unsigned int y );
    
private:
    //The actual hardware texture
    SDL_Texture* mTexture;
    SDL_Renderer* gRenderer;
    void* mPixels;
    int mPitch;
    
    //Image dimensions
    int mWidth;
    int mHeight;
};

class LBitmapFont
{
public:
    //The default constructor
    LBitmapFont();
    
    //Generates the font
    bool buildFont( LTexture *bitmap );
    
    //Shows the text
    void renderText( int x, int y, std::string text );
    
private:
    //The font texture
    LTexture* mBitmap;
    
    //The individual characters in the surface
    SDL_Rect mChars[ 256 ];
    
    //Spacing Variables
    int mNewLine, mSpace;
};


#endif /* texture_wrapper_hpp */
