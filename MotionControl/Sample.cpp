#include <iostream>
#include <string.h>
#include "Leap.h"

using namespace Leap;

class SampleListener : public Listener {
	public:
		virtual void onConnect(const Controller&);
		virtual void onFrame(const Controller&);
};

void SampleListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onFrame(const Controller& controller) {
//	std::cout << "Frame available" << std::endl;
	const Frame frame = controller.frame();
	std::cout << "Frame id: " << frame.id()
		<< ", timestamp: " << frame.timestamp()
	       << ", hands: " << frame.hands().count()
	       << ", fingers: " << frame.fingers().count()
	       << ", tools: " << frame.tools().count()
	       << ", gestures: " << frame.gestures().count() << std::endl;	       
}

int main(int argc, char* argv[])
{
	SampleListener listener;
	Controller controller;

	controller.addListener(listener);

	// Keep the process running until Enter is pressed.
	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();

	// Remove the sample listener when done.
	controller.removeListener(listener);

	return 0;
}
