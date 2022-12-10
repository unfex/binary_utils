PHP_ARG_ENABLE(binary_utils,
    [Whether to enable the "binary_utils" extension],
    [--enable-binary_utils         Enable "binary_utils" extension support])

if test $PHP_BINARY_UTILS != "no"; then
    PHP_REQUIRE_CXX()
    PHP_SUBST(BINARY_UTILS_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, BINARY_UTILS_SHARED_LIBADD)
    PHP_NEW_EXTENSION(binary_utils, src/BinaryUtils.cpp, $ext_shared)
fi
