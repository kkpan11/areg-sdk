# add service interface
addServiceInterface(21_generated res HelloUnblock)

# add projects
macro_example_declare_runnable(21_pubclient   21_generated
    pubclient/src/main.cpp
    pubclient/src/ServiceClient.cpp
)

macro_example_declare_runnable(21_pubservice   21_generated
    pubservice/src/main.cpp
    pubservice/src/ServiceComponent.cpp
)
