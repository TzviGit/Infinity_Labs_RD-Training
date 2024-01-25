#include "tests.hpp"

size_t TestsRun(int add)
{
    static size_t tests = 0;

    tests += add;

    return tests;
}

size_t TestsFailed(int add)
{
    static size_t failures = 0;

    failures += add;

    return failures;
}

void SummarizeTests(void)
{
    size_t tests = TestsRun(0);
    size_t failures = TestsFailed(0);

    printf("\n\n\n\n-------------------------------------------------------");

    if (0 == failures)
    {
        printf("\n\n%s%ld tests done succeessfully. %s", _G_, tests, _N_);
    }
    else
    {
        printf("\n\n%ld tests,%s %ld failures %s", tests, _R_, failures, _N_);
    }

    printf("\n\n");
}