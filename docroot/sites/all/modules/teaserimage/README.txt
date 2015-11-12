INTRO

The module automatically generates thumbnail for teasers and RSS feeds. From images included
in node body (or imagefield which attached to node).

FEATURES

* Using Image Styles for thumbnail.
* Ability settings separately for each type of nodes. Or by default for any.
* Integration with Views.

USAGE

After intalling go to the settings page (admin/config/media/teaserimage) and
select the default settings. You need to have an Image Styles preset already
created (admin/config/media/image-styles).

Navigate to the admin page of the content type for which you want to activate
the Teaser Image feature (admin/structure/types/manage/YOUR-CONTENT-TYPE). You
can here override the default settings.

Next time you create a node of that type, a Teaser Image will be associated to
the node and may show in the teaser and RSS feed (depending on your settings).

OVER

This is fork of Teaser Thumbnail (drupal.org/project/teaserthumbnail)
originally developed by hunvreus (drupal.org/user/49057).

--------------------------------------------------------------------------------

Credits go to Dalay
(http://www.drupalka.ru/).
