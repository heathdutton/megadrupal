$Id$

This module provides a jQuery UI theme selector and uploader.

The user interface allows to:
    - import jQuery UI themes from various archive format
    - identify jQuery UI themes per user set name
    - delete jQuery UI themes
    - attach/detach jQuery UI themes to Drupal theme
    - preview jQuery UI themes in Drupal

Severals ways to fetch themes:
    - via an upload form (handles multiples themes at once)
    - from a user provided local filesystem location or external URL
    - automated import of default jQuery UI themes from 1.7, 1.8 versions

Supplied jQuery UI themes will be stored into a common directory.
When a jQuery UI theme is attached to a Drupal theme and it's active, 
CSS files goes throught the aggregation process.

It also provide a full Drush integration.

A block is provided to let's the end-user switching between jQuery UI
theme availables, depending on current active Drupal theme. If it's display
to anonymous users, performance may decrease due to sessions/cookies usage.
(Unable to serve up cached pages from a reverse proxy server, like Varnish).

This module is an helper module for integrators.
Integrators have to do the job for using it (create appropriate js, 
template, load the system library's widget).
@see jqueryui_theme_example()
@see jqueryui-theme-example.tpl.php

Another things to take note are : 
- detection of themes : jQuery UI themes must contain a "all.css" file. 
  Otherwise, the import process fail.
- A Drupal theme can be associate with only one jQuery UI theme.
