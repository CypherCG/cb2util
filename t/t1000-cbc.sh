#!/bin/sh

test_description="Test CBC commands."

. ./test-lib.sh

out=out

test_expect_success "setup" "
    cd cbc &&
    trap 'rm -f $out' EXIT
"

for file in *.cbc; do
    prefix=${file%.*}

    test_expect_success "($file) extract cheats" "
        cb2util -t cbc $file >$out &&
        test_cmp $out $prefix.extract
    "

    test_expect_success "($file) extract and decrypt cheats" "
        cb2util -t cbc -d $file >$out &&
        test_cmp $out $prefix.decrypt
    "

    test_expect_success "($file) verify signature" "
        cb2util -t cbc -c $file >$out &&
        test_cmp $out $prefix.verify
    "
done

test_done
