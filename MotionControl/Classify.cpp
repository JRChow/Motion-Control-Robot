#include <iostream>
#include <string>
#include "Leap.h"
#include <stdio.h>
#include <curl/curl.h>
#include <cstdlib>
#include <unistd.h>
#include <chrono> // for sleep
#include <thread> // for sleep

using namespace Leap;

#define FWD 2
#define BWD 1
#define LT 4
#define RT 5
#define STD 3
#define AUTO 6
#define CONT 7
#define SLEEP_TIME 588 // milliseconds

// Movement Listener
class MovementListener : public Listener {
	public:
		virtual void onConnect(const Controller&);
		virtual void onFrame(const Controller&);
		virtual void move(int id);
	private:
		const static double FWD_PARAM;
		const static double BWD_PARAM;
		const static double LT_PARAM;
		const static double RT_PARAM; 
		virtual void displayCmd(int id);
};

const double MovementListener::FWD_PARAM = -0.3;
const double MovementListener::BWD_PARAM = 0.8;
const double MovementListener::LT_PARAM = 0.7;
const double MovementListener::RT_PARAM = -0.8;

void MovementListener::displayCmd(int id) {
	switch (id) {
		case FWD:
		std::cout << "Forward" << std::endl;
		break;

		case BWD:
		std::cout << "Backward" << std::endl;
		break;

		case LT:
		std::cout << "Left" << std::endl;
		break;

		case RT:
		std::cout << "Right" << std::endl;
		break;

		case STD:
		std::cout << "Stand" << std::endl;
		break;

		case AUTO:
		std::cout << "Autonomous" << std::endl;
		break;

		case CONT:
		std::cout << "Continuous" << std::endl;
		break;
	}
}

void MovementListener::move(int id) {
	MovementListener::displayCmd(id);
	std::string url = "http://192.168.4.245:8080/?action=command&dest=1&plugin=0&group=1001&value=0&id=" + std::to_string(id);
	CURL *curl;
  	CURLcode res;
	curl = curl_easy_init();
  	
  	if(curl) {
    	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    	/* example.com is redirected, so we tell libcurl to follow redirection */ 
   		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
    	/* Perform the request, res will get the return code */ 
    	res = curl_easy_perform(curl);
    	/* Check for errors */ 
    	if(res != CURLE_OK)
    		fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));
 
 		/* always cleanup */ 
    	curl_easy_cleanup(curl);
  	}
}

void MovementListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void MovementListener::onFrame(const Controller& controller) {
//	std::cout << "Frame available" << std::endl;
	const Frame frame = controller.frame();
	HandList hands = frame.hands();
	Hand firstHand = hands[0];
	double firstHandPitch = firstHand.direction().pitch();
	double firstHandRoll = firstHand.palmNormal().roll();
	
	// std::cout << "hands: " << hands.count()
	//        << ", fingers: " << frame.fingers().count()
	//        << ", gestures: " << frame.gestures().count()
	//        << ", pitch: " << firstHandPitch
	//        << ", roll: " << firstHandRoll;	      

	if (hands.count() > 0 && firstHandPitch <= FWD_PARAM)
	{
		MovementListener::move(FWD);
	} else if (hands.count() > 0 && firstHandPitch >= BWD_PARAM) 
	{
		MovementListener::move(BWD);
	} 
	else {
		if (hands.count() > 0 && firstHandRoll >= LT_PARAM)
		{
			MovementListener::move(LT);
		} else if (hands.count() > 0 && firstHandRoll <= RT_PARAM)
		{
			MovementListener::move(RT);
		}
	}

	MovementListener::move(STD);
	std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
	
	std::cout << std::endl;	
}

int main(int argc, char* argv[])
{
	MovementListener listener;
	Controller controller;

	listener.move(STD);
	controller.addListener(listener);

	// Keep the process running until Enter is pressed.
	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();
	listener.move(STD);

	// Remove the sample listener when done.
	controller.removeListener(listener);

	return 0;
}
