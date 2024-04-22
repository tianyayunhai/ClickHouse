#pragma once
#include <Disks/ObjectStorages/IObjectStorage.h>
#include <Storages/NamedCollectionsHelpers.h>
#include "StorageObjectStorage.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace DB
{

class StorageObjectStorageConfiguration;
using StorageObjectStorageConfigurationPtr = std::shared_ptr<StorageObjectStorageConfiguration>;

class StorageObjectStorageConfiguration
{
public:
    StorageObjectStorageConfiguration() = default;
    StorageObjectStorageConfiguration(const StorageObjectStorageConfiguration & other);
    virtual ~StorageObjectStorageConfiguration() = default;

    using Path = std::string;
    using Paths = std::vector<Path>;

    static void initialize(
        StorageObjectStorageConfiguration & configuration,
        ASTs & engine_args,
        ContextPtr local_context,
        bool with_table_structure);

    virtual std::string getTypeName() const = 0;
    virtual std::string getEngineName() const = 0;

    virtual Path getPath() const = 0;
    virtual void setPath(const Path & path) = 0;

    virtual const Paths & getPaths() const = 0;
    virtual Paths & getPaths() = 0;
    virtual void setPaths(const Paths & paths) = 0;

    virtual String getDataSourceDescription() = 0;
    virtual String getNamespace() const = 0;
    virtual StorageObjectStorage::QuerySettings getQuerySettings(const ContextPtr &) const = 0;
    virtual void addStructureAndFormatToArgs(
        ASTs & args, const String & structure_, const String & format_, ContextPtr context) = 0;

    bool withWildcard() const;
    bool withGlobs() const { return isPathWithGlobs() || isNamespaceWithGlobs(); }
    bool isPathWithGlobs() const;
    bool isNamespaceWithGlobs() const;
    virtual std::string getPathWithoutGlob() const;

    virtual void check(ContextPtr context) const;
    virtual void validateNamespace(const String & /* name */) const {}

    virtual ObjectStoragePtr createObjectStorage(ContextPtr context, bool is_readonly = true) = 0; /// NOLINT
    virtual StorageObjectStorageConfigurationPtr clone() = 0;
    virtual bool isStaticConfiguration() const { return true; }

    String format = "auto";
    String compression_method = "auto";
    String structure = "auto";

protected:
    virtual void fromNamedCollection(const NamedCollection & collection) = 0;
    virtual void fromAST(ASTs & args, ContextPtr context, bool with_structure) = 0;

    void assertInitialized() const;

    bool initialized = false;
};

using StorageObjectStorageConfigurationPtr = std::shared_ptr<StorageObjectStorageConfiguration>;

}
