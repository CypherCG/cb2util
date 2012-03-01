#!/bin/sh

test_description="cb2util cheats: compile cheats"

. ./sharness.sh

for file in $TEST_DIRECTORY/cheats/*.txt; do
    basename=$(basename "$file")

    test_expect_success "$basename" "
        cb2util cheats -c $file out &&
        cb2util cheats out >out2 &&
        test_cmp $file out2
    "
done

test_done
