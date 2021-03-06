// Copyright 2018 yuzu emulator team
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <vector>
#include "core/hle/kernel/event.h"
#include "core/hle/service/service.h"

namespace Service::NFP {

class Module final {
public:
    class Interface : public ServiceFramework<Interface> {
    public:
        explicit Interface(std::shared_ptr<Module> module, const char* name);
        ~Interface() override;

        struct ModelInfo {
            std::array<u8, 0x8> amiibo_identification_block;
            INSERT_PADDING_BYTES(0x38);
        };
        static_assert(sizeof(ModelInfo) == 0x40, "ModelInfo is an invalid size");

        struct AmiiboFile {
            std::array<u8, 10> uuid;
            INSERT_PADDING_BYTES(0x4a);
            ModelInfo model_info;
        };
        static_assert(sizeof(AmiiboFile) == 0x94, "AmiiboFile is an invalid size");

        void CreateUserInterface(Kernel::HLERequestContext& ctx);
        bool LoadAmiibo(const std::vector<u8>& buffer);
        const Kernel::SharedPtr<Kernel::Event>& GetNFCEvent() const;
        const AmiiboFile& GetAmiiboBuffer() const;

    private:
        Kernel::SharedPtr<Kernel::Event> nfc_tag_load{};
        AmiiboFile amiibo{};

    protected:
        std::shared_ptr<Module> module;
    };
};

void InstallInterfaces(SM::ServiceManager& service_manager);

} // namespace Service::NFP
