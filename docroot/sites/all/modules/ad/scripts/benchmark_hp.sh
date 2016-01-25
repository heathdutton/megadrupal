#!/bin/sh

ab -n 300 -c 10 -p benchmark_post_data -T 'application/x-www-form-urlencoded' http://ad.local/ad.php
