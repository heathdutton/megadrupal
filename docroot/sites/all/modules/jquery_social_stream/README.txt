This module is a Drupal wrapper for jQuery social stream plugin by Designchemical
(http://www.designchemical.com/blog/index.php/premium-jquery-plugins/jquery-social-stream-plugin).
The module allows embedding streams as block, bean blocks, and panel panes. You can have multiple
streams per page.

Disclaimer: maintainers of this module are not affiliated with Designchemical.

Installation
------------

1. Install the module as normal Drupal module

2. Put jQuery social stream plugin in jquery-social-stream sub-directory in site libraries directory
   (e.g. sites/libraries/jquery-social-stream). NOTE: You DON'T need to rename any file (as opposite to 7.x-1.x).

3. If you are going to use search terms for Twitter stream, you have to hack the plugin (hopefully it will
   not be necessary in future versions of the plugin). Open jquery-social-stream/js/jquery.social.stream.min.js
   in your favorite editor, find "url=search&q=" (without quotes) and replace it with "url=search&query=" (again,
   without quotes).


Usage
---------

The module provides 3 options to embed a stream.

1. Block. Find "jQuery social stream" block in the list of blocks admin/structure/block, put it
   to a desired region and configure stream at block edit page.

2. Bean. Make sure https://www.drupal.org/project/bean module is installed on your site.
   Visit block/add/jquery-social-stream to create and configure a new stream bean block.

3. Pane. Select "jQuery social stream" pane in "Social stream" section and configure the stream
   at pane edit window.

Any option uses the same stream configuration form. To add a resource to a stream just fill in its username/ID.




