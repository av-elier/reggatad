#include "application.h"
#include "client.h"
#include "json.hpp"
namespace json = nlohmann;

#include <gtest/gtest.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

#include <memory>
#include <iostream>
#include <sstream>


class TestFixture: public testing::Test {
public:
	const int PORT = 9100;
	boost::thread _t;

	fs::path REPO_ROOT;

	TestFixture() :
		_t(&TestFixture::startReggataApp, this),
		REPO_ROOT("/home/vitvlkv/Pictures/") {
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}

	void startReggataApp() {
		Application app(PORT);
		app.openRepo(REPO_ROOT.string(), (REPO_ROOT/".reggata").string());
		app.start();
	}

	void SetUp() {
	}

	void TearDown() {
	}

	~TestFixture() {
		// cleanup any pending stuff, but no exceptions allowed
	}
};

TEST_F (TestFixture, UnitTest1) {
	Client c(PORT);
	std::cout << "sendMsg" << std::endl;
	json::json cmd = {
			{"id", "123"},
			{"cmd", "add_tags"},
			{"args", {
				{"file", (REPO_ROOT/"image.png").c_str()},
				{"tags", {"tag1", "tag2"}}
			}}
	};
	auto err = c.send(cmd.dump());
	ASSERT_EQ(nullptr, err) << "sendMsg failed, error: " << err;
	auto resp = c.recv();
	std::cout << "recvMsg resp: " << resp << std::endl;
}
