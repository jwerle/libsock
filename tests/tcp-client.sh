#!/bin/bash

for (( i = 0; i < 15; ++i )); do
  echo "$i" | nc localhost 8888
done
