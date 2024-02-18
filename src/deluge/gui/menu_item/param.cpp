/*
 * Copyright © 2017-2023 Synthstrom Audible Limited
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

#include "param.h"
#include "gui/l10n/l10n.h"
#include "gui/ui/sound_editor.h"
#include "gui/views/automation_view.h"
#include "hid/buttons.h"
#include "hid/display/display.h"
#include "model/action/action.h"
#include "model/action/action_logger.h"
#include "model/clip/clip.h"
#include "model/model_stack.h"
#include "model/song/song.h"
#include "modulation/automation/auto_param.h"
#include "modulation/params/param_set.h"

namespace deluge::gui::menu_item {

ActionResult Param::buttonAction(deluge::hid::Button b, bool on) {
	using namespace deluge::hid::button;

	bool clipMinder = rootUIIsClipMinderScreen();
	bool arrangerView = !clipMinder && (currentSong->lastClipInstanceEnteredStartPos != -1);
	RootUI* rootUI = getRootUI();

	// Clip or Song button
	// Used to enter automation view from sound editor
	if ((b == CLIP_VIEW && clipMinder) || (b == SESSION_VIEW && arrangerView)) {
		if (on) {
			if (rootUI != &automationView) {
				selectAutomationViewParameter(clipMinder);
				swapOutRootUILowLevel(&automationView);
				automationView.openedInBackground();
			}
			soundEditor.exitCompletely();
		}
		return ActionResult::DEALT_WITH;
	}
	else if (b == SELECT_ENC) {
		// if we're in a context where automation view can be activated, switch to it
		//
		// XXX(sapphire): unclear if this can ever actually get hit?
		if (clipMinder || arrangerView) {
			if (on) {
				if (rootUI == &automationView) {
					selectAutomationViewParameter(clipMinder);
					uiNeedsRendering(&automationView);
					return ActionResult::DEALT_WITH;
				}
			}
		}

		if (on) {
			if (Buttons::isShiftButtonPressed()) {
				// If shift button pressed, delete automation
				Action* action = actionLogger.getNewAction(ActionType::AUTOMATION_DELETE, ActionAddition::NOT_ALLOWED);

				char modelStackMemory[MODEL_STACK_MAX_SIZE];
				ModelStackWithAutoParam* modelStack = getModelStack(modelStackMemory);

				modelStack->autoParam->deleteAutomation(action, modelStack);

				display->displayPopup(l10n::get(l10n::String::STRING_FOR_AUTOMATION_DELETED));
			}
			else {
				// otherwise, leave
				soundEditor.goUpOneLevel();
			}
		}

		return ActionResult::DEALT_WITH;
	}

	return ActionResult::NOT_DEALT_WITH;
}

void Param::selectAutomationViewParameter(bool clipMinder) {
	char modelStackMemory[MODEL_STACK_MAX_SIZE];
	ModelStackWithAutoParam* modelStack = getModelStack(modelStackMemory);

	int32_t p = getP();
	modulation::params::Kind kind = modelStack->paramCollection->getParamKind();

	if (clipMinder) {
		Clip* clip = getCurrentClip();
		clip->lastSelectedParamID = p;
		clip->lastSelectedParamKind = kind;
	}
	else {
		currentSong->lastSelectedParamID = p;
		currentSong->lastSelectedParamKind = kind;
		automationView.onArrangerView = true;
	}
}

} // namespace deluge::gui::menu_item
