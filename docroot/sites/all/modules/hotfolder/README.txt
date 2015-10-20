
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation

INTRODUCTION
------------

Current Maintainer: Bastlynn <bastlynn@gmail.com>

Hotfolder is designed to assist an installation with automated data processing. It monitors a folder 
on the server during cron, matching any new files found in that folder to matching configurations, and 
queues the files to be processed according to the configured settings.

Hotfolder only provides the monitoring system and an API with which to create processes customized for 
your needs. An example module example_hotfolder has been provided to get you started with your own 
development.

With hotfolder you could use a FTP service to drive data into your website, manipulate files, or automate 
just about any process not already handled by a specific module. Always test automated processes before 
putting them live.

INSTALLATION
------------

1. Copy this hotfolder/ directory to your sites/SITENAME/modules directory. 

2. Enable the module and then configure individual watch configurations.

3. Configure user permissions at admin/user/permissions.

USE
---

Hotfolders by itself doesn't do anything, it just provides a way to do work. That said, some things
are common to all installations.

Users start with watch configurations. On these configurations you set the folder to watch, the file
name to watch for, and the action to execute. Cron takes over after that, checking for folders on each
cron execution and setting up jobs according to the watch configurations.

Watch folders are relative to the Drupal root.

You create a watch configuration by creating a content node of type 'Watch Configuration' 
(node/add/watch-configuration).

Views are provided to allow you to keep track of your configurations and logs.

Jobs detected as files come into the folder are processed using the Drupal Queue system. More details on
that can be found here: https://api.drupal.org/api/drupal/modules!system!system.queue.inc/group/queue/7