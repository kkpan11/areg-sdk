
# add service interface
addServiceInterface(19_generated res HelloWatchdog)

# add projects
macro_example_declare_runnable(19_pubclient   19_generated
    pubclient/src/main.cpp
    pubclient/src/ServiceClient.cpp
)

macro_example_declare_runnable(19_pubservice   19_generated
    pubservice/src/main.cpp
    pubservice/src/ServicingComponent.cpp
)
