#pragma once

class BinaryResource {
public:
    /**
     * Loads a binary resource. Be sure to free the data afterwards.
     *
     * @param const char* id   - Resource identifier as specified in the script.
     * @param const char* type - The type of the resource.
     *
     * @return BinaryResource*|nullptr.
     */
    static BinaryResource* Load(const char* id, const char* type);

    /**
     * Destructor that deallocates the resource data.
     */
    ~BinaryResource();

    // Data and size
    void* data;
    int size;
};
