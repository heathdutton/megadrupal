README.txt
==========

A lightweight module for profiling memory usage. Memory usage on each path will
be logged to the watchdog.

It's better to use this with syslog than dblog, as with Database Logging the
writes to the db can be expensive.

Analysis of output
------------------

Useful one-liner to analyse output from memory profiler in drupal-watchdog.log:

grep '|memory profiler|' drupal-watchdog.log | grep -o '[0-9\.]* MB - .*' | awk '{ mem=$1; url=$4; gsub("/[0-9]+" , "/[num]", url); gsub("=[0-9]+" , "=[num]", url);  gsub("apachesolr_search/.*", "apachesolr_search/[keywords]", url); gsub("autocomplete/.*", "autocomplete/[keywords]", url); if (min[url]>mem || min[url]=="") min[url]=mem; if (max[url]<mem) max[url]=mem; tot[url]+=mem; count[url]++; urls[url]=url; } END { print "Path Count Max(MB) Min(MB) Avg(MB)"; for (url in urls) { if (count[url]>2) print url " " count[url] " " max[url] " " min[url] " " sprintf("%.2f", tot[url]/count[url]) } }' | column -t

Sample output:

Path                                                     Count  Max(MB) Min(MB) Avg(MB)
admin/content/node                                       9      59      37.25   54.03
sites/default/files/imagecache/product/abc_1234.jpg      3      14      14      14.00
user/[num]/addresses                                     3      58.25   57.25   57.83
admin/build/modules/list                                 9      115     66.25   73.97
user/login                                               32     102.5   57      59.12
user/password                                            91     104     34.5    56.21
user/[num]/order/[num]                                   79     106.25  35.75   60.07
search/apachesolr_search/[keywords]                      3757   110.5   16.75   47.74
user/[num]/edit                                          48     59.5    20.25   47.61
node/[num]/edit                                          68     113.75  28.25   60.82
node/[num]                                               64024  128     25.75   65.14

To sort the output by a column, add | sort -k[column num]n to the end. For example: 

grep [..snip..] | column -t | sort -k3n

...would sort by Max.

