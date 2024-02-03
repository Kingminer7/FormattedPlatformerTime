#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace geode::prelude;

float offset = 80.f;

class $modify(FPTPlayLayer, PlayLayer) {

	CCLabelBMFont* m_timeLabel = nullptr;
	float m_xContentSize;

	void updateProgressbar() {
		PlayLayer::updateProgressbar();
		// Finding the progress label and storing it
		if (m_fields->m_timeLabel == nullptr) {
			
			float rainbowDelay = Mod::get()->getSettingValue<double>("rainbow-speed");
			auto tint1 = CCTintTo::create(rainbowDelay, 255, 175, 175);
			auto tint2 = CCTintTo::create(rainbowDelay, 255, 255, 175);
			auto tint3 = CCTintTo::create(rainbowDelay, 175, 255, 175);
			auto tint4 = CCTintTo::create(rainbowDelay, 175, 255, 255);
			auto tint5 = CCTintTo::create(rainbowDelay, 175, 175, 255);
			auto tint6 = CCTintTo::create(rainbowDelay, 255, 175, 255);
			auto seq = CCSequence::create(tint1, tint2, tint3, tint4, tint5, tint6, nullptr);
			auto rseq = CCRepeatForever::create(seq);

			CCObject* search;
			CCARRAY_FOREACH(this->getChildren(), search) {
				if (auto newLabel = dynamic_cast<CCLabelBMFont*>(search)) {
					if (std::strlen(newLabel->getString()) < 10) {
						m_fields->m_timeLabel = newLabel;
						if (Mod::get()->getSettingValue<bool>("rainbow-mode")) {
							m_fields->m_timeLabel->runAction(rseq);
						}
						m_fields->m_xContentSize = newLabel->getContentSize().width + offset;
						newLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
						break;
					}
				}
			}
		}
	}

	void updateTimeLabel(int p0, int p1, bool p2) {
		PlayLayer::updateTimeLabel(p0, p1, p2);

		if (m_fields->m_timeLabel == nullptr) {
			return;
		}

		if (!m_level->isPlatformer()) {
			return;
		}

		int labelTime = p0;
		
		int hr = static_cast<int>(labelTime / 3600);
		labelTime -= (hr * 3600);

		int min = static_cast<int>(labelTime / 60);
		labelTime -= (min * 60);

		int sec = labelTime;

		std::stringstream oss;
		oss << hr << ':' << std::setfill('0') << std::setw(2) << min << ':' << std::setw(2) << sec << '.' << std::setw(2) << p1;
		m_fields->m_timeLabel->setAnchorPoint({0.5, 0.5});
		m_fields->m_timeLabel->setPositionX((this->getContentSize().width * 0.5f));
		m_fields->m_timeLabel->setString(oss.str().c_str());
		m_fields->m_timeLabel->updateLabel();
		m_fields->m_timeLabel->setContentSize({m_fields->m_xContentSize, m_fields->m_timeLabel->getContentSize().height});
		
	}
};
