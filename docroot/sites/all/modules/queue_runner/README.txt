Queue Runner Module
-------------------

This module executes items in the Queue Runner queue, typically using a drush
command.

The main goal of the Queue Runner module is that it runs for x time and
executes as many tasks as possible within the given timeframe, with a
specific interval between tasks. This is effectively a sort of daemon that
continiously executes tasks in the queue.

By using the unix cron command to run this once per minute, it will effectively
continuously execute tasks that are added to the queue. It waits until the last
item has finished and starts the whole queue processing code again for another
minute. File locking is used to maintain exclusive execution per unix cron job.

Queue Runner module started from the Drupal 7 Queue API, but it needs to go
beyond the basic back-end agnostic behavior provided by the interface. It also
derived inspiration from the Advanced Queue module, among others:

http://drupal.org/project/advancedqueue

Architecture
------------

In the core Queue API, each "named" queue has a single associated worker
function, and each "named" queue may run in parallel or in arbitrary order.

Queue Runner basically has one single queue, and effectively a single wrapper
function that runs for each item. By inspecting the data for the item we can
determine the functions to call for its work, the function to call if it needs
to retry, and the function to call to finalize the result (store, log, etc.)

The way this module tracks complex tasks is via a mechanism called a
"collector".

To create a collector, add an item in the queue with a worker function that
creates the collector entry in the {queue_runner_collector} table, and it then
creates one or more new child tasks in the queue that are associated via the
{queue_runner_rel_collector} table.

As each child task finishes, its finalize function loads up the collector and,
for example, adds its result to the blob field in the collector table, or to a
log file.

When writing results to a log file, the final log message can link to the log
file in some way, potentially using a private file path and/or a callback that
displays the log message in some relatively pretty form (maybe with paging and
searching). Further add-ons might push the log file to s3 over time and just
keep a signed URL in the local log.

There are three basic types of collectors and example of each are implemented
in the module: parallel, serial and bounded.

- A "parallel" collector is an item that creates all of its child tasks at once
and does not care about the specific order in which they run. Any task can be
retried or run in parallel to others.
- A "serial" collector creates a single child task at a time so that the order
of execution is fully controlled.
- A "bounded" collector is somewhere between the 2 - you can specify a number of
subtasks to maintain in the queue, and those tasks can run somewhat in parallel.
As subtasks complete, new ones will be queued to maintain the required number.

If your module understakes especially complex behavior or needs to do added
processing of results, it's likely that you will want to copy the relevant
"finalize" callback and modify it.  See also the module test cases for examples
of how to "nest" collectors in a hierarchy.

