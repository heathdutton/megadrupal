This module integrates the [KCFinder library](http://kcfinder.sunhater.com/) with the [CKEditor module](https://www.drupal.org/project/ckeditor).

__Note:__ You will need to download and install the [KCFinder library](http://kcfinder.sunhater.com/download) into your site's libraries folder (i.e.: sites/all/libraries).

## About KCFinder
[KCFinder](http://kcfinder.sunhater.com/) is free open-source replacement of CKFinder web file manager.
It can be integrated into FCKeditor, CKEditor, and TinyMCE WYSIWYG web editors (or your custom web applications)
to upload and manage images, flash movies, and other files that can be embedded in an editor's generated HTML content.

KCFinder is licenced GPLv2 & LGPLv2, as in "free" and "open source".


## KCFinder Module Compatibility

The [KCFinder](https://www.drupal.org/project/kcfinder) module __looks to be abandoned__ and __it doesn't integrate with the CKEditor module__, it instead, requires you to use the
WYSIWYG module. This is __my alternative__ that allows you __to use it directly with the CKEditor module__.

Because the integration class that comes with the KCFinder library has specific Drupal variables coded in,
__this module will conflict with the KFCinder module__.

## Demo
 * [See KCFinder's demonstration](http://kcfinder.sunhater.com/) on their main site.

## Installation

 * First download and extract the [KCFinder library from their GitHub repo](https://github.com/sunhater/kcfinder/releases) into your site's "libraries" folder.
 * Make sure the extracted library is named "kcfinder".
   * Example: sites/all/libraries/kcfinder
 * Download and enable this module like any other module.
 * Make sure your user has the newly created "access kcfinder" and "administer kcfinder" permissions.
 * Go to "admin/config/content/ckeditor_kcfinder" and set your "Upload path URL" _(i.e.: http://yourdomain.com/sites/default/files)_.
 * The "Browse Server" button should now be visible in the CKEditor for related images such as images.

## Troubleshooting
 * Make sure your Upload path is writable by the web user.
 * Most other specifics are validated/verified on the config page when you submit your settings.

