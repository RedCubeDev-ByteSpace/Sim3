cmake_minimum_required(VERSION 4.2)
project(sim3 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

link_libraries(raylib m json-c)
add_executable(sim3 main.c
        GUI/Grid.h
        GUI/Grid.c
        GUI/GUI.h
        GUI/Input.c
        GUI/Input.h
        GUI/GUI.c
        GUI/Drawables.h
        GUI/Drawables.c
        GUI/Drawables/Wire.h
        GUI/Drawables/Wire.c
        GUI/Bench.h
        GUI/Bench.c
        Sim/Components/Connection.c
        Sim/Components/Connection.h
        Sim/Components/ConnectionPoint.c
        Sim/Components/ConnectionPoint.h
        Sim/Components/ConnectionPointList.c
        Sim/Components/WireList.c
        Sim/SimulationSpace.c
        Sim/SimulationSpace.h
        GUI/Drawables/FixedContact.c
        GUI/Drawables/FixedContact.h
        Sim/Components/FixedContact.c
        Sim/Components/FixedContact.h
        Sim/Components/Chip.c
        Sim/Components/Chip.h
        Sim/SimulationResources.c
        Sim/SimulationResources.h
        GUI/Drawables/Chip.c
        GUI/Drawables/Chip.h
        Sim/SimLists/SimspaceLists.h
        Sim/SimLists/SimspaceLists.c
        GUI/Drawables/WireBranchingPoint.c
        GUI/Drawables/WireBranchingPoint.h
        GUI/Drawables/LED.c
        GUI/Drawables/LED.h
        Sim/Components/LED.c
        Sim/Components/LED.h)
