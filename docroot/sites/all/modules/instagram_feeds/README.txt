DESCRIPTION
-----------

This is a Feature module. It allows you to create Instagram feeds for your site
that will download images from Instagram using filtering by hashtags and / or
usernames.

You can use default module presets, so the absolutely ready for use features
will be available:
    * Creating unlimited number of Instagram Feeds (that will be available as
      blocks) with custom settings for each one (custom node type)
    * Ready Views Displays for management of Instagram Feeds and Instagram
      images via administrative interface
    * Downloading new Instagram images by cron
    * Deleting old images by cron (expiration time sets up at Instagram Feeds
      settings page)
    * Inserting Instagram Feeds into any filterable text fields (body,
      long text, long text with summary) using special button in WYSIWYG editor,
      or into any other place as a block
    * Custom permissions for creating new Instagram Feeds, managing images and
      inserting Feeds via WYSIWYG editor
    * Adding Instagram users in the black list
    * Hiding images that flagged by users as inappropriate until moderator's
      check.

Or you can use the basic module functionality to create your own applications.

INSTALLATION
------------

Follow the instructions in the provided INSTALL.txt file.

CONFIGURATION
-------------

There are several settings that can be configured in the following places:

  Administration > Modules (admin/modules)
    Enable or disable the basic module and its submodules. (default: disabled)

  Administration > Configuration > Web services > Instagram Settings
  (admin/config/services/instagram)
    After Instagram Feeds module installation please fill out the Instagram
    Client ID, Client Secret and generate Access Token. These options are
    required to have access to Instagram. Also it is recommended to set images
    expiration time (1 month by default).

  Administration > People > Permissions (admin/people/permissions)
    Under Instagram Feeds module:
    Insert Instagram Feeds: Enable access to insert Instagram Feed via WYSIWYG
    editors. (default: disabled)
    Under Node module:
    Instagram Feed: Create new content, Edit own content, Edit any content,
    Delete own content, Delete any content. (default: disabled)
    Under Taxonomy module:
    Edit terms in Instagram Users: Enable access to add Instagram users into the
    black list. (default: disabled)

  Administration > Content > Instagram (admin/content/instagram)
    This page becomes available after enabling the Instagram Feeds Moderation
    module. Allows to view and manage all Instagram Feeds.

  Administration > Content > Instagram > Instagram Media Items
  (admin/content/instagram/media)
    This page becomes available after enabling the Instagram Feeds Moderation
    module. Allows to view and manage all Instagram images and videos.

CONTACT
-------

Current maintainers:
* Vasiliy Zaytsev (Vasiliy Zaytsev) - https://drupal.org/user/2443606

This project has been sponsored by:
* Blink Reaction
  Blink Reaction is a leader in Enterprise Drupal Development, delivering
  robust, high-performance websites for dynamic companies. Through expertise in
  seamless integration, module customization, and application development, Blink
  creates scalable and flexible web solutions that provide the best in dynamic
  user experience. Specialties: Custom Drupal Development, Enterprise Drupal
  Development, Fortune 500 Clients, End to End Solutions.
* Whole Foods Market
  America’s Healthiest Grocery Store™
  Who are we? Well, we seek out the finest natural and organic foods available,
  maintain the strictest quality standards in the industry, and have an
  unshakeable commitment to sustainable agriculture. Add to that the excitement
  and fun we bring to shopping for groceries, and you start to get a sense of
  what we’re all about. Oh yeah, we’re a mission-driven company too.

MORE INFORMATION
----------------

For more information, visit the project page:
http://drupal.org/project/instagram_feeds
