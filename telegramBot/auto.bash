#!/bin/bash
i=1;
for i in {1..3}; do
  ./a.out &
  sleep 60
done;
