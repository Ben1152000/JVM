package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "utilities",
    hdrs = ["utilities.h"],
)

cc_library(
    name = "class",
    srcs = ["class.cc"],
    hdrs = ["class.h"],
    deps = [":utilities"],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    deps = [
        ":utilities",
        ":class"
    ],
)

cc_library(
    name = "loader",
    srcs = ["loader.cc"],
    hdrs = ["loader.h"],
    deps = [
        ":utilities",
        ":class",
        ":parser",
    ],
)

cc_binary(
    name = "cli",
    srcs = ["cli.cc"],
    deps = [
        ":utilities",
        ":class",
        ":loader",
    ],
)

cc_test(
    name = "class_test",
    srcs = ["class_test.cc"],
    deps = [
        ":utilities",
        ":class",
        ":loader",
    ],
    data = ["//test:data"],
)