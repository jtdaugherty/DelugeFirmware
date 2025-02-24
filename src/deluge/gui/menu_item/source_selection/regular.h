/*
 * Copyright (c) 2014-2023 Synthstrom Audible Limited
 *
 * This file is part of The Synthstrom Audible Deluge Firmware.
 *
 * The Synthstrom Audible Deluge Firmware is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include "definitions_cxx.hpp"
#include "gui/menu_item/source_selection.h"

namespace deluge::gui::menu_item::source_selection {
class Regular final : public SourceSelection {
public:
	using SourceSelection::SourceSelection;
	void beginSession(MenuItem* navigatedBackwardFrom = nullptr) override;
	ParamDescriptor getDestinationDescriptor() override;
	MenuItem* selectButtonPress() override;
	MenuItem* patchingSourceShortcutPress(PatchSource newS, bool previousPressStillActive) override;
	std::string_view getTitle() const override { return l10n::getView(l10n::String::STRING_FOR_MODULATE_WITH); };
};

extern Regular regularMenu;
} // namespace deluge::gui::menu_item::source_selection
