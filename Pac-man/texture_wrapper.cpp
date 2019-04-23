//
//  texture_wrapper.cpp
//  pac-man
//
//  
//

#include "texture_wrapper.hpp"


LTexture::LTexture(SDL_Renderer* renderer)
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    mPixels = NULL;
    mPitch = 0;
    gRenderer = renderer;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();
    
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Convert surface to display format
        SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface, SDL_PIXELFORMAT_RGBA8888, NULL );
        if( formattedSurface == NULL )
        {
            printf( "Unable to convert loaded surface to display format! %s\n", SDL_GetError() );
        }
        else
        {
            //Create blank streamable texture
            newTexture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
            if( newTexture == NULL )
            {
                printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
            }
            else
            {
                //Enable blending on texture
                SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );
                
                //Lock texture for manipulation
                SDL_LockTexture( newTexture, &formattedSurface->clip_rect, &mPixels, &mPitch );
                
                //Copy loaded/formatted surface pixels
                memcpy( mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h );
                
                //Get image dimensions
                mWidth = formattedSurface->w;
                mHeight = formattedSurface->h;
                
                //Get pixel data in editable format
                Uint32* pixels = (Uint32*)mPixels;
                int pixelCount = ( mPitch / 4 ) * mHeight;
                
                //Map colors
                Uint32 colorKey = SDL_MapRGB( formattedSurface->format, 0, 0xFF, 0xFF );
                Uint32 transparent = SDL_MapRGBA( formattedSurface->format, 0x00, 0xFF, 0xFF, 0x00 );
                
                //Color key pixels
                for( int i = 0; i < pixelCount; ++i )
                {
                    if( pixels[ i ] == colorKey )
                    {
                        pixels[ i ] = transparent;
                    }
                }
                
                //Unlock texture to update
                SDL_UnlockTexture( newTexture );
                mPixels = NULL;
            }
            
            //Get rid of old formatted surface
            SDL_FreeSurface( formattedSurface );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font)
{
    //Get rid of preexisting texture
    free();
    
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
    if( textSurface != NULL )
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( *gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    else
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    
    
    //Return success
    return mTexture != NULL;
}
#endif

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
        mPixels = NULL;
        mPitch = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture rgb
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( SDL_Point position, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { position.x, position.y, mWidth, mHeight };
    
    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    
    //Render to screen
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

bool LTexture::lockTexture()
{
    bool success = true;
    
    //Texture is already locked
    if( mPixels != NULL )
    {
        printf( "Texture is already locked!\n" );
        success = false;
    }
    //Lock texture
    else
    {
        if( SDL_LockTexture( mTexture, NULL, &mPixels, &mPitch ) != 0 )
        {
            printf( "Unable to lock texture! %s\n", SDL_GetError() );
            success = false;
        }
    }
    
    return success;
}

bool LTexture::unlockTexture()
{
    bool success = true;
    
    //Texture is not locked
    if( mPixels == NULL )
    {
        printf( "Texture is not locked!\n" );
        success = false;
    }
    //Unlock texture
    else
    {
        SDL_UnlockTexture( mTexture );
        mPixels = NULL;
        mPitch = 0;
    }
    
    return success;
}

void* LTexture::getPixels()
{
    return mPixels;
}

int LTexture::getPitch()
{
    return mPitch;
}

Uint32 LTexture::getPixel32( unsigned int x, unsigned int y )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32*)mPixels;
    
    //Get the pixel requested
    return pixels[ ( y * ( mPitch / 4 ) ) + x ];
}

LBitmapFont::LBitmapFont()
{
    //Initialize variables
    mBitmap = NULL;
    mNewLine = 0;
    mSpace = 0;
}

bool LBitmapFont::buildFont( LTexture* bitmap )
{
    bool success = true;
    
    //Lock pixels for access
    if( !bitmap->lockTexture() )
    {
        printf( "Unable to lock bitmap font texture!\n" );
        success = false;
    }
    else
    {
        //Set the background color
        Uint32 bgColor = bitmap->getPixel32( 0, 0 );
        
        //Set the cell dimensions
        int cellW = bitmap->getWidth() / 16;
        int cellH = bitmap->getHeight() / 16;
        
        //New line variables
        int top = cellH;
        int baseA = cellH;
        
        //The current character we're setting
        int currentChar = 0;
        
        //Go through the cell rows
        for( int rows = 0; rows < 16; ++rows )
        {
            //Go through the cell columns
            for( int cols = 0; cols < 16; ++cols )
            {
                //Set the character offset
                mChars[ currentChar ].x = cellW * cols;
                mChars[ currentChar ].y = cellH * rows;
                
                //Set the dimensions of the character
                mChars[ currentChar ].w = cellW;
                mChars[ currentChar ].h = cellH;
                
                //Find Left Side
                //Go through pixel columns
                for( int pCol = 0; pCol < cellW; ++pCol )
                {
                    //Go through pixel rows
                    for( int pRow = 0; pRow < cellH; ++pRow )
                    {
                        //Get the pixel offsets
                        int pX = ( cellW * cols ) + pCol;
                        int pY = ( cellH * rows ) + pRow;
                        
                        //If a non colorkey pixel is found
                        if( bitmap->getPixel32( pX, pY ) != bgColor )
                        {
                            //Set the x offset
                            mChars[ currentChar ].x = pX;
                            
                            //Break the loops
                            pCol = cellW;
                            pRow = cellH;
                        }
                    }
                }
                
                //Find Right Side
                //Go through pixel columns
                for( int pColW = cellW - 1; pColW >= 0; --pColW )
                {
                    //Go through pixel rows
                    for( int pRowW = 0; pRowW < cellH; ++pRowW )
                    {
                        //Get the pixel offsets
                        int pX = ( cellW * cols ) + pColW;
                        int pY = ( cellH * rows ) + pRowW;
                        
                        //If a non colorkey pixel is found
                        if( bitmap->getPixel32( pX, pY ) != bgColor )
                        {
                            //Set the width
                            mChars[ currentChar ].w = ( pX - mChars[ currentChar ].x ) + 1;
                            
                            //Break the loops
                            pColW = -1;
                            pRowW = cellH;
                        }
                    }
                }
                
                //Find Top
                //Go through pixel rows
                for( int pRow = 0; pRow < cellH; ++pRow )
                {
                    //Go through pixel columns
                    for( int pCol = 0; pCol < cellW; ++pCol )
                    {
                        //Get the pixel offsets
                        int pX = ( cellW * cols ) + pCol;
                        int pY = ( cellH * rows ) + pRow;
                        
                        //If a non colorkey pixel is found
                        if( bitmap->getPixel32( pX, pY ) != bgColor )
                        {
                            //If new top is found
                            if( pRow < top )
                            {
                                top = pRow;
                            }
                            
                            //Break the loops
                            pCol = cellW;
                            pRow = cellH;
                        }
                    }
                }
                
                //Find Bottom of A
                if( currentChar == 'A' )
                {
                    //Go through pixel rows
                    for( int pRow = cellH - 1; pRow >= 0; --pRow )
                    {
                        //Go through pixel columns
                        for( int pCol = 0; pCol < cellW; ++pCol )
                        {
                            //Get the pixel offsets
                            int pX = ( cellW * cols ) + pCol;
                            int pY = ( cellH * rows ) + pRow;
                            
                            //If a non colorkey pixel is found
                            if( bitmap->getPixel32( pX, pY ) != bgColor )
                            {
                                //Bottom of a is found
                                baseA = pRow;
                                
                                //Break the loops
                                pCol = cellW;
                                pRow = -1;
                            }
                        }
                    }
                }
                
                //Go to the next character
                ++currentChar;
            }
        }
        
        //Calculate space
        mSpace = cellW / 2;
        
        //Calculate new line
        mNewLine = baseA - top;
        
        //Lop off excess top pixels
        for( int i = 0; i < 256; ++i )
        {
            mChars[ i ].y += top;
            mChars[ i ].h -= top;
        }
        
        bitmap->unlockTexture();
        mBitmap = bitmap;
    }
    
    return success;
}

void LBitmapFont::renderText( int x, int y, std::string text )
{
    //If the font has been built
    if( mBitmap != NULL )
    {
        //Temp offsets
        SDL_Point cur = {x,y};
        
        //Go through the text
        for( int i = 0; i < text.length(); ++i )
        {
            //If the current character is a space
            if( text[ i ] == ' ' )
            {
                //Move over
                cur.x += mSpace;
            }
            //If the current character is a newline
            else if( text[ i ] == '\n' )
            {
                //Move down
                cur.y += mNewLine;
                
                //Move back
                cur.x = x;
            }
            else
            {
                //Get the ASCII value of the character
                int ascii = (unsigned char)text[ i ];
                
                //Show the character
                mBitmap->render( cur, &mChars[ ascii ] );
                //Move over the width of the character with one pixel of padding
                cur.x += mChars[ ascii ].w + 1;
            }
        }
    }
}
