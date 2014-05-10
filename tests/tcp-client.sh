#!/bin/bash

sleep 1
for (( i = 0; i < 10; ++i )); do
  printf "%s\n" "$i" | netcat localhost 6002
done
