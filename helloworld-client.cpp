// helloworld-client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stormancer.h"
#include <string>
#include <iostream>
#include <thread>
#include <cstdlib>

std::string endpoint("http://api.stormancer.com/");
std::string accountId("ranjak");
std::string appName("tutorial-helloworld");

int main()
{
  auto configHandle = Stormancer::Configuration::create(endpoint, accountId, appName);
  Stormancer::Client* client = Stormancer::Client::createClient(configHandle);

  auto connection = client->getPublicScene("hello").then([](Stormancer::Result<Stormancer::Scene*>* sceneResult)
  {
    if (!sceneResult->success()) {
      throw std::exception(sceneResult->reason());
    }

    Stormancer::Scene* scene = sceneResult->get();
    auto routeAdded = scene->addRoute("msg", [] (Stormancer::Packetisp_ptr packet)
    {
      std::cout << packet->readObject<std::string>() << std::endl;
    });

    if (!routeAdded->success()) {
      throw std::exception(routeAdded->reason());
    }

    return scene->connect();
  });

  try {
    if (!connection.get()->success()) {
      std::cout << "Failed to connect, reason: " << connection.get()->reason() << std::endl;
    }
    else {
      std::cout << "Connection successful." << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(5));
    }
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  Stormancer::destroy(client);

  std::system("PAUSE");
  return 0;
}

