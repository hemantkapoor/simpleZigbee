file(GLOB_RECURSE sourcesCpp      zigbeeComms/*.cpp zigbeeManager/*.cpp observer/*.cpp)
file(GLOB_RECURSE sourcesH        zigbeeComms/*.h zigbeeManager/*.h observer/*.h)
target_sources(SimpleZigbee
    PRIVATE
        ${sourcesCpp}
    PUBLIC
        ${sourcesH}
)

target_compile_options(SimpleZigbee PUBLIC -std=c++1y -Wall -Wfloat-conversion)
