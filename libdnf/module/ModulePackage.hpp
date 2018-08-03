/*
 * Copyright (C) 2018 Red Hat, Inc.
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef LIBDNF_MODULEPACKAGE_HPP
#define LIBDNF_MODULEPACKAGE_HPP

#include <memory>
#include <string>

#include "libdnf/dnf-types.h"
#include "modulemd/ModuleMetadata.hpp"
#include "modulemd/profile/ModuleProfile.hpp"
#include "libdnf/repo/solvable/Package.hpp"
#include "../goal/IdQueue.hpp"

class ModulePackage;
typedef std::shared_ptr< ModulePackage > ModulePackagePtr;

class ModulePackageContainer;

class ModulePackage;
typedef std::shared_ptr< ModulePackage > ModulePackagePtr;

class ModulePackage // TODO inherit in future; : public Package
{
public:
    enum class ModuleState {
        UNKNOWN,
        ENABLED,
        DEFAULT
    };
    ~ModulePackage();

    /**
    * @brief Create module provides based on modulemd metadata.
    */
    std::string getName() const;
    std::string getStream() const;
    std::string getNameStream() const;
    std::string getNameStreamVersion() const;
    std::string getVersion() const;
    std::string getContext() const;
    const char * getArchitecture() const;
    std::string getFullIdentifier() const;

    std::string getSummary() const;
    std::string getDescription() const;

    std::vector<std::string> getArtifacts() const;

    std::shared_ptr<Profile> getProfile(const std::string &name) const;
    std::vector<std::shared_ptr<ModuleProfile> > getProfiles() const;

    std::vector<std::shared_ptr<ModuleDependencies> > getModuleDependencies() const;

    bool isEnabled();

    void addStreamConflict(const std::shared_ptr<ModulePackage> &package);

    void enable();
    ModuleState getState();
    void setState(ModuleState newState);

    Id getId() const { return id; };

private:
    friend struct ModulePackageContainer;

    ModulePackage(DnfSack * moduleSack, Repo *repo, const std::shared_ptr<ModuleMetadata> &metadata);

    static Id createPlatformSolvable(DnfSack * moduleSack, const std::string &osReleasePath,
        const std::string install_root, const char *  platformModule);
    void createDependencies(Solvable *solvable) const;

    std::shared_ptr<ModuleMetadata> metadata;
    ModuleState state;

    // TODO: remove after inheriting from Package
    DnfSack * moduleSack;
    Id id;
};

inline ModulePackage::ModuleState ModulePackage::getState() { return state; }

inline void ModulePackage::setState(ModulePackage::ModuleState newState) { state = newState; }

#endif //LIBDNF_MODULEPACKAGE_HPP
