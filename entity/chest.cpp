#include "chest.h"
#include "../util.h"

std::string ChestEntity::get_interact_name() const {
    return "Chest";
}

void ChestEntity::make_loot(std::string& loot) {
    auto pieces = split_by_char(loot, '|');

    for (auto& piece : pieces) {
        auto itemdat = split_by_char(piece, ':');

        if (itemdat.size() < 2) {
            std::cout << "Failed to decode chest loot!" << std::endl;
            std::cout << "  > " << loot << std::endl;
            exit(102);
        }

        int n = std::stoi(itemdat[1]);

        this->add_item(ItemData::inst().make_item(itemdat[0], n));
    }
}
