# add service interface
addServiceInterface(21_generated examples/21_pubunblock/services/HelloUnblock.siml)

# add projects
macro_declare_executable(21_pubclient   21_generated
    pubclient/src/main.cpp
    pubclient/src/ServiceClient.cpp
)

macro_declare_executable(21_pubservice   21_generated
    pubservice/src/main.cpp
    pubservice/src/ServiceComponent.cpp
)
