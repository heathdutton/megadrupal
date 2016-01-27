
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Adding More Services


INTRODUCTION
------------

Current Maintainer: Morbus Iff <morbus@disobey.com>
Original Maintainer: Adam Kalsey <http://kalsey.com>

Activity Stream builds a lifestream, a "River of You", by aggregating all your
social activities in one place. Whether it's bookmarks on Del.icio.us, posts
from Twitter or your blog, edits to wikis or enjoyed music and movies, anything
you create can be gathered into one easy-to-read stream. Every item becomes a
full Drupal node, allowing them to be searched, promoted, commented upon, and
managed just like any other piece of content within Drupal.

Activity Stream 3.x also attempts to archive these activities by retaining
a copy of the raw data received. This allows you to recreate your activity
items, even if the remote site is no longer listing that item, if you didn't
use all the data the first time around (GPS, user-agent strings, keywords,
etc.), or if the service doesn't exist any more. If you uninstall the module,
the imported content is kept, though all module configuration is deleted.

Developers can build modules to add more third-party integrations, using a
dead-simple API that is half Activity Stream and half Drupal core. Both theme
and module developers can tweak the activity item display from the defaults.

Services supported in Activity Stream core include: Twitter and any RSS or
Atom feed. Activity Stream 7.x-3.1 is scheduled to add dozens more by
providing a Feed Service Builder.


INSTALLATION
------------

 1. Copy the activitystream/ directory to your sites/SITENAME/modules directory.

 2. Enable the module and configure it at user/#/edit/activity-stream.

 3. View the site-wide activity stream at /activity-stream.

 4. View the per-user activity stream at /user/#/activity-stream.

 5. See "Adding more services" for how to integrate another service.


ADDING MORE SERVICES
--------------------

If the service you'd like to add offers RSS feeds, first consider if you can
merely add it to the Feed URLs input provided by the Activity Stream Feed
module at user/#/edit/activity-stream. If that is satisfactory, you're done.
If it isn't (because the service doesn't provide RSS, you want to tweak the
display, etc.), follow the steps below.

To begin adding more services:

 1. You'll need to create or use a custom module to implement the API. Further
    information about module development and the Drupal APIs is available in
    the "Develop for Drupal" docs at http://drupal.org/documentation/develop.
    At the minimum, read:

     * Module file names and locations: http://drupal.org/node/1074362
     * Telling Drupal about your module: http://drupal.org/node/1075072
     * Implementing your first hook: http://drupal.org/node/1095546

 2. Read about the Activity Stream API in activitystream.api.php.

