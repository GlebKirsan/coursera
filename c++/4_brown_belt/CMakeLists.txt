project(brown_belt)

include_directories(headers)
set(WEEK_PREFIX "${PREFIX}.${CONFIGURATION}/${PROJECT_NAME}")

add_subdirectory(1_week)
add_subdirectory(2_week)
add_subdirectory(3_week)