QUICK START GUIDE
-----------------
install dependency timepicker module
https://www.drupal.org/sandbox/wouters_frederik/1679556


Enable the timepicker + asf module.

Add a advanced shedule field to your custom content type.

make sure your cron runs with the wished granularity.
(if you want to be able to publish any minute, run cron each minute).
use an advanced cron module for that. e.g. http://drupal.org/project/elysia_cron

Create a node , and select the publication
  start date/ end date / start time / end time
Select if the pubvlication is iterative
Iterative means the node is published on start hr
 each day and depublished on end hr eacht day, until a end is reached.
Iteration count or end date can cause to end the publication scheme.

ADVANCED HELP
-------------

the publication times are saved in a separate table (asf_schedule).
