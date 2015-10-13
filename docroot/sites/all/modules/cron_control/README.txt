
Cron Control
============

Name: cron_control
Author: Markus Kalkbrenner | Cocomore AG
Drupal: 6.x
Sponsor: Cocomore AG - http://www.cocomore.com


About
=====

Using Cron Control, you'll be able to fine-tune Drupal's cron, with
special features for clustered environments.

Using the current version of Cron Control, you'll be able to turn on or off
individual cron jobs or define the server on which they should run.

For example, transcoding videos should not decrease your site's performance.
Using Cron Control, you can configure a server in a clustered environment, 
to be dedicated to doing that job, while others continue to serve your pages
without loss of performance.

You can also outsource resource-hungry cron jobs (like transcoding videos)
to a cloud, which doesn't need to be available all the time. These cron jobs
will run as soon as the cloud server resources are available, while your 
regular web servers continue to run "normal" cron jobs.


Installation
============

1. Place whole cron_control folder into your Drupal modules/
   directory, or better, sites/x/modules directory.

2. Enable the cron_control module by navigating to
     administer > modules

3. Run cron.php on every server

4. Fine-tune your cron settings at
     administer > settings > cron_control

It's recommended to run cron.php on every server, "round robin",
using an offset of a couple of minutes.

