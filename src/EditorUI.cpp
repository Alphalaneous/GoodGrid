#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "../include/DrawGridAPI.hpp"

class $modify(MyEditorUI, EditorUI) {
	void updateZoom(float p0) {
		EditorUI::updateZoom(p0);
		DrawGridAPI::get().markDirty();
	}

	// fix bug when grid snap breaks the duration lines
	void ccTouchEnded(CCTouch* p0, CCEvent* p1) {
		bool shouldUpdateEndPos = (
			p0->m_nId != m_rotationTouchID && 
			p0->m_nId != m_scaleTouchID && 
			p0->m_nId != m_transformTouchID && 
			p0->m_nId == m_touchID && 
			m_snapObjectExists && 
			m_continuousSnap && 
			m_snapObject && 
			GameManager::get()->getGameVariable("0008") && 
			(int)m_snapObject->getRotation() % 90 == 0
		);
	
		EditorUI::ccTouchEnded(p0, p1);
	
		if (shouldUpdateEndPos) {
			m_editorLayer->m_drawGridLayer->m_updateTimeMarkers = true;
		}
	}
};
