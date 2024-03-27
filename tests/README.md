# How to test

## Run test

Configure cmake with test option

```bash
cmake . -BUILD_TESTS=ON
```

Compile test

```bash
make
```

## Basic test file skeleton

```c
#include "unity.h"
#include "calc.h"

void setUp()
{

}

void tearDown()
{

}

int main(void)
{
    UNITY_BEGIN();

    /*
     * Add in between
     * RUN_TEST(func_name); 
     */

    UNITY_END();

    return 0;
}
```