#!/bin/bash
find -type f -name '*.log' -print0 | xargs -0 rm