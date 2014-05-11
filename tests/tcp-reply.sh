#!/bin/bash

sleep 1
echo -n reply | timeout 1 nc -w 1 -l 6003
