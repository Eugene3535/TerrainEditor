#include "AssetManager.hpp"

#include <filesystem>

template<class Resource>
AssetManager<Resource>::AssetManager()
{
}

template<class Resource>
AssetManager<Resource>::~AssetManager()
{
    release();
}

template<class Resource>
void AssetManager<Resource>::loadFromDirectory(const std::string& filepath)
{
    for (auto& file : std::filesystem::directory_iterator(filepath))
    {
        Resource resource;

        if (file.path().has_extension())
            if (resource.loadFromFile(file.path().string()))
                m_resources.emplace(file.path().stem().string(), resource);
    }
}

template<class Resource>
std::optional<Resource> AssetManager<Resource>::get(const std::string& name)
{
    if (auto found = m_resources.find(name); found != m_resources.end())
        return found->second;

    return std::nullopt;
}

template<class Resource>
inline void AssetManager<Resource>::release()
{
    m_resources.clear();
}
