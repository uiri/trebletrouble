#!/bin/sh

if [ -d bin ]; then
    rm bin/*
    rmdir bin
fi

if [ -f src/ugfx/.build/libugfx.a ]; then
    rm src/ugfx/.build/libugfx.a
fi
