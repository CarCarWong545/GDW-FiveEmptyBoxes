#include "HealthBar.h"
#include "Utilities.h"

void HealthBar::UpdateHealthBar(int hb, int hbb)
{
	auto& hbui = ECS::GetComponent<Transform>(hb);
	auto& hbspr = ECS::GetComponent<Sprite>(hb);
	auto& hbbui = ECS::GetComponent<Transform>(hbb);
	auto& cameraH = ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera());
	auto& cameraV = ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera());

	hbbui.SetPositionX(cameraH.GetCam()->GetPosition().x - 70);
	hbbui.SetPositionY(cameraV.GetCam()->GetPosition().y - 72);
	int hboffset = 0;
	if (MainEntities().Health() >= 0) {
		hboffset = (100 - MainEntities().Health()) / 4;
		hbspr.SetWidth(MainEntities().Health() / 2);
	}
	else
	{
		hboffset = (100 - 0) / 4;
		hbspr.SetWidth(0);
	}

	//int hboffset = (100 - MainEntities().Health()) / 4;
	
	hbui.SetPositionX((cameraH.GetCam()->GetPosition().x - 70) - hboffset);
	hbui.SetPositionY((cameraV.GetCam()->GetPosition().y - 72));
}

void HealthBar::UpdateGhostCounter(std::vector<int> ghosts, int fillColour, int backColour)
{
	

	auto& fcui = ECS::GetComponent<Transform>(fillColour);
	auto& fcspr = ECS::GetComponent<Sprite>(fillColour);
	auto& bcui = ECS::GetComponent<Transform>(backColour);
	auto& bcspr = ECS::GetComponent<Sprite>(backColour);
	auto& cameraH = ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera());
	auto& cameraV = ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera());
	int ghostscaptured = 0;
	if (MainEntities::Captured() >= 0) {
		ghostscaptured = MainEntities::Captured();
	}
	else
	{
		ghostscaptured = 0;
	}

	if (MainEntities::Captured() <= ghosts.size()) {
		ghostscaptured = MainEntities::Captured();

	}
	else
	{
		ghostscaptured = ghosts.size();
	}
	
	int ghostphoto = (bcspr.GetWidth() / (ghosts.size()));
	
	int fcoffset = ((bcspr.GetWidth()/2) - (ghostscaptured*(ghostphoto/2)));
	//(ghostscaptured * ghostphoto) - (((ghosts.size() / 2) + 1) * ghostphoto) - (ghostscaptured * (ghostphoto/2));
					//((ghostscaptured * ghostphoto) - (((ghosts.size()/2)+1) * ghostphoto)) - (((ghostscaptured) * ghostphoto)/2);
	


	fcspr.SetWidth((ghostscaptured) * ghostphoto);
	bcspr.SetWidth(ghosts.size() * ghostphoto);
	fcui.SetPositionX((cameraH.GetCam()->GetPosition().x + 60) - fcoffset);
	fcui.SetPositionY(cameraV.GetCam()->GetPosition().y - 72);

	bcui.SetPositionX(cameraH.GetCam()->GetPosition().x + 60);
	bcui.SetPositionY(cameraV.GetCam()->GetPosition().y - 72);

	int index = 0;
	for (int i : ghosts) {
		//st position
		auto& ghostFilter = ECS::GetComponent<Transform>(i);
		ghostFilter.SetPositionY(cameraV.GetCam()->GetPosition().y - 72);
		ghostFilter.SetPositionX((cameraH.GetCam()->GetPosition().x + 60) + ((bcspr.GetWidth()/2) - (ghostphoto/2) - (index*(ghostphoto))));
		index++;

	}

}




