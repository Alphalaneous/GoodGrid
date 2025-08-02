#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "../include/DrawGridAPI.hpp"

class $modify(MyEditorUI, EditorUI) {
	void updateZoom(float p0) {
		EditorUI::updateZoom(p0);
		DrawGridAPI::get().markDirty();
	}
};