#!/bin/bash

find -L source | grep "[.]hpp$\|[.]h$" | sed ':a;N;$!ba;s/\n/ /g'
