#pragma once

#include "Texture2D.hpp"

#include <map>
#include <string>
#include <optional>

template <class Resource>
class AssetManager final
{
public:
    AssetManager();
    AssetManager(const AssetManaget<Resource>& other) = delete;
    AssetManager operator = (const AssetManaget<Resource>& other) = delete;
    AssetManager(const AssetManaget<Resource>&& other) = delete;
    AssetManager operator = (const AssetManaget<Resource>&& other) = delete;
    ~AssetManager();

    void loadFromDirectory(const std::string& filepath);
    std::optional<Resource> get(const std::string& name);
    void release();

private:
    std::map<std::string, Resource> m_resources;
};

#include "AssetManager.inl"