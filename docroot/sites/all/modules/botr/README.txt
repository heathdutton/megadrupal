Bits on the Run module for Drupal
=================================

Description
-----------

This module allows you to easily upload and embed videos using the Bits on the
Run platform. The embedded video links can be signed, making it harder for
viewers to embed your content into their websites without permission.

Changelog
---------

1.2 - Added support for resumable uploads.

1.1 - Replaced the API key / secret settings with a login form.
      Added in-application signup.
      Added link to the dashboard in the widget.

1.0 - Added support for searching playlists.
      Small bugfixes in various parts of the module.
      Synchronized with new dashboard release.
      Improved formatting of the code.

0.6 - Upgraded the widget for Drupal 7. Drupal 6 is no longer supported in this version.

0.5 - Added option to disable embed code signing by setting timeout to 0.
      Made the timeout default 0.
      Small bugfixes to scripting, styling and plugin directory detection.

0.4 - Made bootstrapping code more flexible, module can now be put in all subdirs.
      Added 'target' and 'weight' options to content type options. 

0.3 - Implemented feedback from Drupal reviewer.
      Added reminder to enter API key and secret.

0.2 - Removed no-thumb icon.

0.1 - Initial release.


Installation
------------

- Move the directory that contains this file to the appropriate module directory
  (for example: sites/all/modules subdirectory of your Drupal installation).

- Make sure the PHP cURL library is installed. See
  http://php.net/manual/en/book.curl.php for more info, or ask your system
  administrator.

- Set $base_url in the settings file of your Drupal site (if you don't know
  which one, edit sites/default/settings.php). For example, if your site is
  reached via http://mysite.com/drupal, the line should be:
  $base_url = 'http://mysite.com/drupal';

- Go to the Modules section, scroll to the Video section and check the 'enabled'
  box for the Bits on the Run module. Save your configuration.

- Go to Configuration -> Media -> Bits on the Run and log in using your
  Bits on the Run credentials. If you do not have a Bits on the Run account yet,
  it is possible to sign up directly from inside your Drupal installation by
  clicking "sign up".

- Go to Configuration -> Content Authoring -> Text Formats -> Filtered HTML -> configure,
  check the 'Bits on the Run quicktag replacement' box under the
  'Enabled filters' header and save the configuration.
  
  Make sure that in the 'Filter processing order' table, the Bits on the Run
  quicktag placement filter is placed somewhere after the
  'Limit allowed HTML tags' filter, if it is present.

  Note that in this screen, you can configure this filter to your liking. Under
  the 'Filter settings' header, click Bits on the Run quicktag replacement.
  Now you can set the timeout for signed links, the content DNS mask and the
  default player.

- Go to People -> Permissions and allow authenticated users to use the
  Bits on the Run widget to add videos to posts. It is also possible to grant
  rights over the administration panel and to grant them access to your
  Bits on the Run dashboard.

- Note: the Bits on the Run widget is shown on the edit page of all node types
  by default, and inserts the video quicktags into the 'body' field. If you want
  to choose a different target you can change the settings for the node type.
  Go to Administer -> Content management -> Content types -> edit and modify the
  'target' and 'weight' settings under the 'Submission form settings' header.
  Leave the target empty if you want to hide the widget, or enter the name of
  the field to which the quicktags should be added.

Usage
-----

When you're creating or editing a node, you can use the new 'Bits on the
Run' section to embed videos in your post. Clicking a video will insert a
quicktag in the editor window. This tag will be replaced by a JavaScript embed
code when the page is rendered. 

PROTIP: if you want to override the default player for a given video, simply
append a dash and the corresponding player key to video key in the quicktag
For example: [bitsontherun MdkflPz7-35rdi1pO].

You can also upload new videos to your account with the new widget. Simply fill
out the title, choose a file and click the 'upload video' button.


Known Issues
------------

- The 'choose file' button does not work under Firefox 3.

- A rendering bug causes the video list in Internet Explorer 7 to overlap the
  upload button. It can be worked around by first using the search box.
