find_package(Catch2 CONFIG REQUIRED)

function(test filename)
    set(test_project_name test.${filename})
    add_executable(${test_project_name})

    target_sources(${test_project_name} PRIVATE ${filename}.cpp)

    target_compile_features(${test_project_name} PRIVATE cxx_std_20)

    target_link_libraries(${test_project_name} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
    target_link_libraries(${test_project_name} PRIVATE out::out)

    add_test(
            NAME ${test_project_name}
            COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${test_project_name})
endfunction()

## unit tests via catch (alphabetical order)
test("csv")
test("enum_to_string")
test("json")
test("json.object")
test("json.value_formatter")
test("sql.value_formatter")
