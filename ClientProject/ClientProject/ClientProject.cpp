// ClientProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CognitiveVRAnalytics.h"
#include "curl.h"

//for testing delay
#include <chrono>
#include <thread>

#if defined(_MSC_VER)
#include "gtest.h"
#else
#include "include/gtest/gtest.h"
#endif

std::string temp;

std::string TESTINGCUSTOMER = "altimagegames59340-unitywanderdemo-test";
long TestDelay = 2;

size_t handle(char* buf, size_t size, size_t nmemb, void* up)
{
	//std::cout << "handle handle handle";

	for (int c = 0; c < size*nmemb; c++)
	{
		temp.push_back(buf[c]);
	}
	return size * nmemb;
}

void DoWebStuff(std::string url, std::string content, cognitive::WebResponse response)
{
	//std::cout << "<<<<curl url sent\n";
	//std::cout << url + "\n";

	CURL* curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	//std::cout << curl_version();

	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		//verbose output
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		/* Now specify the POST data */
		if (content.size() > 0)
		{
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());
		}

		//if (response != NULL)
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &handle);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);

		//call response
		if (response != NULL)
			response(temp);

		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		else
		{

		}

		temp.clear();
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
}

//===========================TESTS
//----------------------INITIALIZATION

TEST(Initialization, MultipleStartSessions) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	bool first = cog.StartSession();
	EXPECT_EQ(first, true);
	bool second = cog.StartSession();
	EXPECT_EQ(second, false);
	bool third = cog.StartSession();
	EXPECT_EQ(third, false);
}

TEST(DISABLED_Initialization, MultipleStartEndSessions) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	bool first = cog.StartSession();
	EXPECT_EQ(first, true);
	cog.EndSession();
	bool second = cog.StartSession();
	EXPECT_EQ(second, true);
	cog.EndSession();
	bool third = cog.StartSession();
	EXPECT_EQ(third, true);
	cog.EndSession();
}

TEST(DISABLED_Initialization, SessionFullStartEnd) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));
	
	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	//settings. //TODO try all the fields in the settings container
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	std::vector<float> pos = { 0,0,0 };
	cog.StartSession();
	cog.transaction->BeginEndPosition("testing1", pos);
	cog.EndSession();
}

TEST(DISABLED_Initialization, SessionStart) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
}

TEST(DISABLED_Initialization, Initialization) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	std::vector<float> pos = { 0,0,0 };
	cog.transaction->BeginEndPosition("testing1", pos);
	cog.StartSession();
}

TEST(DISABLED_Initialization, SessionEnd) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.EndSession();
}

TEST(DISABLED_Initialization, SessionStartEnd) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	cog.EndSession();
}

//----------------------SET USER

TEST(DISABLED_UserSettings, UserPreSession) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cognitive::nlohmann::json user = cognitive::nlohmann::json();
	user["age"] = 21;
	user["location"] = "vancouver";
	cog.SetUserId("john");
	cog.SetUserProperties(user);
	cog.UpdateUserState();
	cog.StartSession();
}

TEST(DISABLED_UserSettings, UserPostSession) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cognitive::nlohmann::json user = cognitive::nlohmann::json();
	user["age"] = 21;
	user["location"] = "vancouver";
	cog.StartSession();
	cog.SetUserId("john");
	cog.SetUserProperties(user);
	cog.UpdateUserState();
}

TEST(DISABLED_UserSettings, UserNullPreSession) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.SetUserId("");
	cog.SetUserProperties(cognitive::nlohmann::json());
	cog.UpdateUserState();
	cog.StartSession();
}

TEST(DISABLED_UserSettings, UserNullPostSession) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	cog.SetUserId("");
	cog.SetUserProperties(cognitive::nlohmann::json());
	cog.UpdateUserState();
}

//----------------------SET DEVICE

TEST(DISABLED_DeviceSettings, DevicePreSession) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.SetDeviceId("7741345684915735");
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICEMEMORY, 128);
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICEOS, "chrome os 16.9f");
	cog.UpdateDeviceState();
	cog.StartSession();
}

TEST(DISABLED_DeviceSettings, DevicePostSession) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	cog.SetDeviceId("7741345684915735");
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICEMEMORY, 128);
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICEOS, "chrome os 16.9f");
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICEGPU, "GeForce GTX 970");
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICECPU, "i7-4770 CPU @ 3.40GHz");
	cog.UpdateDeviceState();
}

TEST(DISABLED_DeviceSettings, DeviceNullPreSession) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.SetDeviceId("");
	cog.UpdateDeviceState();
	cog.StartSession();
}

TEST(DISABLED_DeviceSettings, DeviceNullPostSession) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);
	cog.StartSession();
	cog.SetDeviceId("");
	cog.UpdateDeviceState();
}

TEST(DISABLED_DeviceSettings, DeviceNullPreEnd) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);


	cog.SetDeviceId("7741345684915735");
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICEMEMORY, 128);
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICEOS, "chrome os 16.9f");
	cog.UpdateDeviceState();
	cog.EndSession();
}

//----------------------SET USER DEVICE
TEST(DISABLED_UserDeviceSettings, UserDevicePostSession) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();

	cog.SetDeviceId("7741345684915735");
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICEMEMORY, 128);
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICEOS, "chrome os 16.9f");
	cog.UpdateDeviceState();

	cognitive::nlohmann::json user = cognitive::nlohmann::json();
	user["age"] = 21;
	user["location"] = "vancouver";
	cog.SetUserId("john");
	cog.SetUserProperties(user);
	cog.UpdateUserState();

	cog.EndSession();
}

TEST(DISABLED_UserDeviceSettings, UserDevicePreSession) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cognitive::nlohmann::json user = cognitive::nlohmann::json();
	user["age"] = 21;
	user["location"] = "vancouver";
	cog.SetUserId("john");
	cog.SetUserProperties(user);
	cog.UpdateUserState();

	cog.SetDeviceId("7741345684915735");
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICEMEMORY, 128);
	cog.SetDeviceProperty(cognitive::EDeviceProperty::DEVICEOS, "chrome os 16.9f");
	cog.UpdateDeviceState();

	cog.StartSession();
	cog.EndSession();
}

//----------------------TRANSACTIONS

TEST(DISABLED_Transaction, PreSessionNoEnd) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	std::vector<float> pos = { 0,0,0 };
	cog.transaction->BeginEndPosition("testing1", pos);

	cog.StartSession();
}

TEST(DISABLED_Transaction, PreSessionEnd) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	std::vector<float> pos = { 0,0,0 };
	cog.transaction->BeginEndPosition("testing1", pos);

	cog.StartSession();
	cog.EndSession();
}

TEST(DISABLED_Transaction, PreSessionSend) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	std::vector<float> pos = { 0,0,0 };
	cog.transaction->BeginEndPosition("testing1", pos);

	cog.StartSession();
	cog.SendData();
}

TEST(DISABLED_Transaction, PreSessionPropsSend) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	std::vector<float> pos = { 0,0,0 };

	cognitive::nlohmann::json user = cognitive::nlohmann::json();
	user["age"] = 21;
	user["location"] = "vancouver";

	cog.transaction->BeginEndPosition("testing1", pos, user);

	cog.StartSession();
	cog.SendData();
}

TEST(DISABLED_Transaction, SessionEnd) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	std::vector<float> pos = { 0,0,0 };

	cognitive::nlohmann::json user = cognitive::nlohmann::json();
	user["age"] = 21;
	user["location"] = "vancouver";

	cog.StartSession();
	cog.transaction->BeginEndPosition("testing1", pos, user);
	cog.EndSession();
}

//----------------------TUNING

TEST(DISABLED_Tuning, TuningGetValue) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();

	auto snow_attitude = cog.tuning->GetValue("snow_attitude", "mellow", cognitive::EntityType::kEntityTypeDevice);
	EXPECT_EQ(snow_attitude, "ferocious");
	std::cout << snow_attitude << std::endl;

	auto blockPosition = cog.tuning->GetValue("vinegar_volume", 0, cognitive::EntityType::kEntityTypeDevice);
	EXPECT_EQ(blockPosition, 50);
	std::cout << blockPosition << std::endl;

	auto ExitPollActivated = cog.tuning->GetValue("ExitPollActivated", false, cognitive::EntityType::kEntityTypeDevice);
	EXPECT_EQ(ExitPollActivated, true);
	std::cout << ExitPollActivated << std::endl;

	auto pi = cog.tuning->GetValue("pi", (float)3.0, cognitive::EntityType::kEntityTypeDevice);
	EXPECT_FLOAT_EQ(pi, 3.1415927);
	std::cout << pi << std::endl;

	cog.EndSession();
}

TEST(DISABLED_Tuning, TuningGetValueNoSession) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	auto snow_attitude = cog.tuning->GetValue("snow_attitude", "mellow", cognitive::EntityType::kEntityTypeDevice);
	EXPECT_EQ(snow_attitude, "mellow");
	std::cout << snow_attitude << std::endl;
}

TEST(DISABLED_Tuning, TuningGetInvalidValue) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	auto snow_attitude = cog.tuning->GetValue("snow_raditude", "mellow", cognitive::EntityType::kEntityTypeDevice);
	EXPECT_EQ(snow_attitude, "mellow");
	std::cout << snow_attitude << std::endl;
	cog.EndSession();
}

TEST(DISABLED_Tuning, TuningGetInvalidCast) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	auto snow_attitude = cog.tuning->GetValue("snow_attitude", 500, cognitive::EntityType::kEntityTypeDevice);
	EXPECT_EQ(snow_attitude, 500);
	std::cout << snow_attitude << std::endl;
	cog.EndSession();
}

TEST(DISABLED_Tuning, TuningGetInvalidCastBool) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	auto snow_attitude = cog.tuning->GetValue("snow_attitude", false, cognitive::EntityType::kEntityTypeDevice);
	EXPECT_EQ(snow_attitude, false);
	std::cout << snow_attitude << std::endl;
	cog.EndSession();
}

//----------------------SETTING SCENE KEYS FOR SCENE EXPLORER

TEST(DISABLED_Scenes, NoScenes) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();

	std::vector<float> pos = { 1,0,0 };
	cog.transaction->BeginEndPosition("testing1", pos);
	pos[0] = 2;
	cog.transaction->BeginEndPosition("testing2", pos);
	pos[0] = 3;
	cog.transaction->BeginEndPosition("testing3", pos);

	cog.SendData();
	cog.EndSession();
}

TEST(DISABLED_Scenes, InitScenes) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	std::map<std::string, std::string> scenes = std::map<std::string, std::string>();
	scenes["tutorial"] = "DELETE_ME_1";
	scenes["menu"] = "DELETE_ME_2";
	scenes["finalboss"] = "DELETE_ME_3";
	settings.sceneIds = scenes;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();

	std::vector<float> pos = { 1,0,0 };
	cog.transaction->BeginEndPosition("testing1", pos);
	pos[0] = 2;
	cog.transaction->BeginEndPosition("testing2", pos);
	pos[0] = 3;
	cog.transaction->BeginEndPosition("testing3", pos);

	cog.SendData();
	cog.EndSession();
}

TEST(DISABLED_Scenes, InitSetScenes) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	std::map<std::string, std::string> scenes = std::map<std::string, std::string>();
	scenes["tutorial"] = "DELETE_ME_1";
	scenes["menu"] = "DELETE_ME_2";
	scenes["finalboss"] = "DELETE_ME_3";
	settings.sceneIds = scenes;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	cog.SetScene("tutorial");

	std::vector<float> pos = { 1,0,0 };
	cog.transaction->BeginEndPosition("testing1", pos);
	pos[0] = 2;
	cog.transaction->BeginEndPosition("testing2", pos);
	pos[0] = 3;
	cog.transaction->BeginEndPosition("testing3", pos);

	cog.SendData();
	cog.EndSession();
}

TEST(DISABLED_Scenes, InitSetSceneSwitch) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	std::map<std::string, std::string> scenes = std::map<std::string, std::string>();
	scenes["tutorial"] = "DELETE_ME_1";
	scenes["menu"] = "DELETE_ME_2";
	scenes["finalboss"] = "DELETE_ME_3";
	settings.sceneIds = scenes;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	cog.SetScene("tutorial");

	std::vector<float> pos = { 1,0,0 };
	cog.transaction->BeginEndPosition("testing1", pos);
	cog.SetScene("menu");
	pos[0] = 2;
	cog.transaction->BeginEndPosition("testing2", pos);
	cog.SetScene("finalboss");
	pos[0] = 3;
	cog.transaction->BeginEndPosition("testing3", pos);

	cog.SendData();
	cog.EndSession();
}

TEST(DISABLED_Scenes, InitSetInvalidScene) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	std::map<std::string, std::string> scenes = std::map<std::string, std::string>();
	scenes["tutorial"] = "DELETE_ME_1";
	scenes["menu"] = "DELETE_ME_2";
	scenes["finalboss"] = "DELETE_ME_3";
	settings.sceneIds = scenes;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	cog.SetScene("non-existent");

	std::vector<float> pos = { 1,0,0 };
	cog.transaction->BeginEndPosition("testing1", pos);

	cog.SendData();
	cog.EndSession();
}

TEST(DISABLED_Scenes, InitSetInvalidNoScene) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	cog.SetScene("non-existent");

	std::vector<float> pos = { 1,0,0 };
	cog.transaction->BeginEndPosition("testing1", pos);

	cog.SendData();
	cog.EndSession();
}

//----------------------EXITPOLL

TEST(DISABLED_ExitPoll, RequestSetNoInit) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.exitpoll->RequestQuestionSet("player_died");

	cog.StartSession();
}

TEST(DISABLED_ExitPoll, BasicRequest) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	cog.exitpoll->RequestQuestionSet("player_died");
	cog.EndSession();
}

TEST(DISABLED_ExitPoll, GetThenRequest) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);


	cog.StartSession();
	cog.exitpoll->GetQuestionSet();
	cog.exitpoll->RequestQuestionSet("player_died");
	cog.EndSession();
}

TEST(DISABLED_ExitPoll, RequestThenGet) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);


	cog.StartSession();
	cog.exitpoll->RequestQuestionSet("player_died");
	cog.exitpoll->GetQuestionSet();
	cog.EndSession();
}

TEST(DISABLED_ExitPoll, InvalidRequestThenGet) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	cog.exitpoll->RequestQuestionSet("question-does-not-exist");
	cog.exitpoll->GetQuestionSet();
	cog.EndSession();
}

TEST(DISABLED_ExitPoll, RequestThenGetAnswersJson) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	cog.exitpoll->RequestQuestionSet("player_died");
	cognitive::nlohmann::json questions = cog.exitpoll->GetQuestionSet();

	cog.exitpoll->AddAnswer(cognitive::FExitPollAnswer(cognitive::EQuestionType::HAPPYSAD, false));
	cog.exitpoll->AddAnswer(cognitive::FExitPollAnswer(cognitive::EQuestionType::MULTIPLE, 0));
	cog.exitpoll->AddAnswer(cognitive::FExitPollAnswer(cognitive::EQuestionType::SCALE, 1));
	cog.exitpoll->SendAllAnswers();
	cog.EndSession();
}

TEST(DISABLED_ExitPoll, RequestThenGetAnswersString) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	cog.exitpoll->RequestQuestionSet("player_died");
	std::string questionString = cog.exitpoll->GetQuestionSetString();

	cog.exitpoll->AddAnswer(cognitive::FExitPollAnswer(cognitive::EQuestionType::HAPPYSAD, false));
	cog.exitpoll->AddAnswer(cognitive::FExitPollAnswer(cognitive::EQuestionType::MULTIPLE, 0));
	cog.exitpoll->AddAnswer(cognitive::FExitPollAnswer(cognitive::EQuestionType::SCALE, 1));
	cog.exitpoll->SendAllAnswers();
	cog.EndSession();
}

//----------------------GAZE

TEST(DISABLED_Gaze, GazeThenInit) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	std::vector<float>pos = { 0,0,0 };
	std::vector<float>rot = { 0,0,0,1 };

	for (float i = 0; i < 10; ++i)
	{
		pos[1] = i;
		cog.gaze->RecordGaze(pos, rot);
	}
	
	cog.StartSession();
	cog.EndSession();
}

TEST(DISABLED_Gaze, GazeThenInitSetScene) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	std::map<std::string, std::string> scenes = std::map<std::string, std::string>();
	scenes["gazescene"] = "DELETE_ME_3";
	settings.sceneIds = scenes;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);


	std::vector<float>pos = { 0,0,0 };
	std::vector<float>rot = { 0,0,0,1 };

	for (float i = 0; i < 10; ++i)
	{
		pos[1] = i;
		cog.gaze->RecordGaze(pos, rot);
	}

	cog.StartSession();
	cog.SetScene("gazescene");
	cog.EndSession();
}

TEST(DISABLED_Gaze, InitThenGazeThenSetScene) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	std::map<std::string, std::string> scenes = std::map<std::string, std::string>();
	scenes["gazescene"] = "DELETE_ME_3";
	settings.sceneIds = scenes;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	std::vector<float>pos = { 0,0,0 };
	std::vector<float>rot = { 0,0,0,1 };

	for (float i = 0; i < 10; ++i)
	{
		pos[1] = i;
		cog.gaze->RecordGaze(pos, rot);
	}

	cog.SetScene("gazescene");
	cog.EndSession();
}

TEST(DISABLED_Gaze, InitThenGazeThenSendThenSetScene) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	std::map<std::string, std::string> scenes = std::map<std::string, std::string>();
	scenes["gazescene"] = "DELETE_ME_3";
	settings.sceneIds = scenes;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	std::vector<float>pos = { 0,0,0 };
	std::vector<float>rot = { 0,0,0,1 };

	for (float i = 0; i < 10; ++i)
	{
		pos[1] = i;
		cog.gaze->RecordGaze(pos, rot);
	}

	cog.SendData();
	cog.SetScene("gazescene");
	cog.EndSession();
}

//----------------------SENSORS

TEST(DISABLED_Sensors, SenseThenInit) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	for (float i = 0; i < 10; ++i)
	{
		cog.sensor->RecordSensor("test-sensor",i);
	}

	cog.StartSession();
	cog.EndSession();
}

TEST(DISABLED_Sensors, SenseThenInitSetScene) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	std::map<std::string, std::string> scenes = std::map<std::string, std::string>();
	scenes["sensescene"] = "DELETE_ME_3";
	settings.sceneIds = scenes;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	for (float i = 0; i < 10; ++i)
	{
		cog.sensor->RecordSensor("test-sensor", i);
	}

	cog.StartSession();
	cog.SetScene("sensescene");
	cog.EndSession();
}

TEST(DISABLED_Sensors, InitThenGazeThenSetScene) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	std::map<std::string, std::string> scenes = std::map<std::string, std::string>();
	scenes["sensescene"] = "DELETE_ME_3";
	settings.sceneIds = scenes;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	for (float i = 0; i < 10; ++i)
	{
		cog.sensor->RecordSensor("test-sensor", i);
	}

	cog.SetScene("sensescene");
	cog.EndSession();
}

TEST(DISABLED_Sensors, InitThenGazeThenSendThenSetScene) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	std::map<std::string, std::string> scenes = std::map<std::string, std::string>();
	scenes["sensescene"] = "DELETE_ME_3";
	settings.sceneIds = scenes;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);

	cog.StartSession();
	for (float i = 0; i < 10; ++i)
	{
		cog.sensor->RecordSensor("test-sensor", i);
	}
	cog.SendData();
	cog.SetScene("sensescene");
	cog.EndSession();
}

//----------------------DYNAMICS

TEST(DISABLED_Dynamics, InitRegisterSend) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);
	cog.StartSession();

	int object1id = cog.dynamicobject->RegisterObject("object1", "lamp");
	int object2id = cog.dynamicobject->RegisterObject("object2", "lamp");

	std::vector<float>pos = { 0,0,0 };
	std::vector<float>rot = { 0,0,0,1 };

	pos = { 0,0,5 };
	cog.dynamicobject->Snapshot(object1id, pos, rot);
	pos = { 0,1,6 };
	cog.dynamicobject->Snapshot(object2id, pos, rot);

	pos = { 0,0,7 };
	cog.dynamicobject->Snapshot(object1id, pos, rot);
	pos = { 0,2,8 };
	cog.dynamicobject->Snapshot(object2id, pos, rot);

	pos = { 0,0,9 };
	cog.dynamicobject->Snapshot(object1id, pos, rot);
	pos = { 0,3,10 };
	cog.dynamicobject->Snapshot(object2id, pos, rot);

	cog.SendData();
	cog.EndSession();
}

TEST(DISABLED_Dynamics, InitRegisterSceneSend) {
	if (TestDelay > 0)
		std::this_thread::sleep_for(std::chrono::seconds(TestDelay));

	cognitive::CoreSettings settings;
	settings.webRequest = &DoWebStuff;
	settings.CustomerId = TESTINGCUSTOMER;

	

	std::map<std::string, std::string> scenes = std::map<std::string, std::string>();
	scenes["dynamicscene"] = "DELETE_ME_2";
	settings.sceneIds = scenes;
	auto cog = cognitive::CognitiveVRAnalyticsCore(settings);
	cog.StartSession();

	int object1id = cog.dynamicobject->RegisterObject("object1", "lamp");
	int object2id = cog.dynamicobject->RegisterObject("object2", "lamp");

	std::vector<float>pos = { 0,0,0 };
	std::vector<float>rot = { 0,0,0,1 };

	pos = { 0,0,5 };
	cog.dynamicobject->Snapshot(object1id, pos, rot);
	pos = { 0,1,6 };
	cog.dynamicobject->Snapshot(object2id, pos, rot);

	pos = { 0,0,7 };
	cog.dynamicobject->Snapshot(object1id, pos, rot);
	pos = { 0,2,8 };
	cog.dynamicobject->Snapshot(object2id, pos, rot);

	pos = { 0,0,9 };
	cog.dynamicobject->Snapshot(object1id, pos, rot);
	pos = { 0,3,10 };
	cog.dynamicobject->Snapshot(object2id, pos, rot);
	
	cog.SetScene("dynamicscene");
	cog.SendData();
	cog.EndSession();
}



int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

#if defined(_MSC_VER)
	RUN_ALL_TESTS();
	system("pause");
	return 0;
#else
	return RUN_ALL_TESTS();
#endif
	
	

	//should use only this in a build
	//return RUN_ALL_TESTS();
}