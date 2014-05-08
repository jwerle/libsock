#!/bin/bash

for (( i = 0; i < 20; ++i )); do
  echo "$i" | nc localhost 8888
done
