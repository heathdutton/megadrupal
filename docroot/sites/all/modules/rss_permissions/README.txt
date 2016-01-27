
Description
-----------

RSS permissions module adds role-based permission settings to blog, 
taxonomy, aggregator, and main site's RSS feeds.

Permission settings currently available are:

- access site RSS feed
    Main site RSS feed, which can be found at rss.xml

- access taxonomy RSS feeds
    Taxonomy RSS feeds, which can be found at taxonomy/term/%/%/feed, if 
    Taxonomy module is enabled. This applies to all taxonomy feeds. You 
    cannot set permissions for individual category feeds.

- access user blog RSS feeds
    Individual user RSS feeds, which can be found at blog/%/feed, if Blog 
    module is enabled. This applies to all user blogs. You cannot set 
    permissions for individual blog feeds.

- access main blog RSS feed
    Main blog RSS feed, which can be found at blog/feed, if Blog module 
    is enabled.

- access aggregator RSS feeds
    All aggregator feeds, if Aggregator module is enabled. These can be 
    found at aggregator/rss, aggregator/opml, and aggregator/rss/%.

Installation
------------

Read http://drupal.org/getting-started/install-contrib on how to download
and install Drupal modules. The typical location for the module's files is
sites/all/modules/rss_permissions in the Drupal directory.

Once the module is enabled, you can set the permissions at:
  Administer › User management > Permissions


Authors
-------

 * Anastasia Zarubina <http://drupal.org/user/274293> - original author
