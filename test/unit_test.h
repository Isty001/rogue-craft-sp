#ifndef API_TEST_UNIT_TEST_H
#define API_TEST_UNIT_TEST_H


#include <string.h>
#include <minunit.h>
#include "../src/level/point.h"
#include "../src/util.h"


#define FIXTURE_DIR "./test/fixture"

#define TEST_NAME(name) puts("\x1B[33m[" name "] test case:\x1B[0m")


static inline void assert_string(char *expected, char *actual)
{
    char buff[1000];
    sprintf(buff, "Strings are not equal. Expected %s got %s", expected, actual);

    mu_assert(0 == strcmp(expected, actual), buff);
}

static inline void assert_point(Point point, int y, int x)
{
    mu_assert_int_eq(y, point.y);
    mu_assert_int_eq(x, point.x);
}

static inline void assert_range(Range a, Range b)
{
    mu_assert(a.from == b.from && a.to == b.to, "Ranges are not equal");
}


#endif
