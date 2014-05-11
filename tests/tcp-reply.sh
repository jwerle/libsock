#!/bin/bash

sleep 1
echo -n reply | nc -w 1 -l 6003
