#include "utils.hpp"

#include <filesystem>

#include <SDL.h>

std::string getResourcePath(std::string file) {
    std::string path = "";

    char * base_path = SDL_GetBasePath();
    if (base_path) {
        path += base_path;
        SDL_free(base_path);
    }

    path += file;

    return path;
}

std::string getRomPath() {
    std::string rom_path = getResourcePath("roms");
    if (!std::filesystem::exists(rom_path)) {
        std::string home_dir = getenv("HOME");
        if (home_dir.empty()) {
            // If HOME is not set, lets just use the current working directory for now
            home_dir = ".";
        }
        rom_path = home_dir + "/multiboy-roms";
        if (!std::filesystem::exists(rom_path)) {
            std::filesystem::create_directory(rom_path);
        }
    }
    return rom_path;
}

std::string getAssetPath(std::string file) {
    std::string asset_path = getResourcePath("assets/" + file);
    if (!std::filesystem::exists(asset_path)) {
        asset_path = getResourcePath("../share/multiboy/assets/" + file);
    }
    if (!std::filesystem::exists(asset_path)) {
        throw std::runtime_error("Could not load asset " + file);
    }
    return asset_path;
}
