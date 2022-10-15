#pragma once

#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

class AssetStore {
    private:
		std::map<std::string, TTF_Font*> fonts;
		std::map<std::string, SDL_Texture*> textures;
        // TODO: create a map for audio

    public:
        AssetStore();
        ~AssetStore();

		std::vector<std::string> textureList;

        void ClearAssets();

        void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
        SDL_Texture* GetTexture(const std::string& assetId);

        void AddFont(const std::string& assetId, const std::string& filePath, int fontSize);
        TTF_Font* GetFont(const std::string& assetId);
};

