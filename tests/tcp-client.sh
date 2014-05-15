#!/bin/bash

NC="netcat"

if ! type -f netcat > /dev/null 2>&1; then
  if type -f nc > /dev/null 2>&1; then
    NC="nc"
  else
    exit -1
  fi
fi

sleep 1
for (( i = 0; i < 10; ++i )); do
  printf "%s\n" "$i" | "${NC}" localhost 6002
done
