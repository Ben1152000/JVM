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

cc_binary(
    name = "cli",
    srcs = ["cli.cc"],
    deps = [
        ":utilities",
        ":class",
    ],
)

filegroup(
    name = "classfiles",
    testonly = 1,
    srcs = glob(["test/*.class"]),
    visibility = ["//visibility:public"],
)

cc_test(
    name = "class_test",
    srcs = ["class_test.cc"],
    deps = [
        ":utilities",
        ":class",
    ],
    data = [":classfiles"],
)