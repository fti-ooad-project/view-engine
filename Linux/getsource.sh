#!/bin/bash

find -L source | grep "[.]cpp$\|[.]c$" | sed ':a;N;$!ba;s/\n/ /g'
