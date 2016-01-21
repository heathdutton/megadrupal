== Installation

1. - Install the Libraries module.
This is required by scald_gallery and scald_galleria.
Available at https://drupal.org/project/libraries.
Download, install and enable as usual. No configuration required.

2. - Install the Galleria Javascript library.
If your site doesn't have the directory sites/all/libraries, create it now.
Visit http://galleria.io/ and click the 'Free download' button. Save the zip file.
Extract the contents, then copy the galleria directory and all its contents
to sites/all/libraries. You should end up with path sites/all/libraries/galleria

3. - Update your jQuery
Newer versions of Galleria requires jQuery 1.7 or higher. The standard
Drupal 7 installation ships with an older version of jQuery.
To update it, download, install and enable module jquery_update
from https://drupal.org/project/jquery_update.
After enabling it, go to admin/config/development/jquery_update
and choose a later version of jQuery.

4. - Install the Scald Gallery module
Download and install as usual. You will see two new modules, scald_gallery
and scald_galleria. Enable them both.
Go to admin/structure/scald/gallery/contexts and below the preferred
representation choose the Galleria, also check player settings.

Now you should be able to create a new gallery and add images to it.
When you view the Gallery atom, they are displayed in the Galleria viewer.

Possible problems when viewing the Gallery atom:
- "Notice: Undefined index: data in drupal_process_attached()"
When you view the gallery, thumbnails of the gallery's images are shown,
but no viewer is displayed. The error message shown above is displayed.
You have copied the library to the wrong path. Check step 2 above.

- No error message, but no viewer either.
When you view the gallery, thumbnails of the gallery's images are shown,
but no viewer is displayed. Checking the browser's javascript console,
I found the error message
"Uncaught TypeError: Object [object Object] has no method 'on'"
This was a sign that the wrong jQuery version was installed. Check step 3 above.

The following steps is optional and could be use with the latest Scald dev or
the upcoming Scald 1.1 for multiple and drag and drop file upload.

- Download the plupload module.
- Download the plupload library, run 'ant' to build the library. You should have
  a sites/all/libraries/plupload/js folder after the build.
- In each atom type (e.g. for Images it is admin/structure/scald/image) select
  Plupload as the default upload type.

== Using

Gallery can be created using both modal dialog (the add button in the DnD
library) or at atom/add/gallery. However when creating a gallery in a modal
dialog, you can't create another atom in another modal dialog then add it to the
gallery.

The Galleria player support title and description for each gallery item. By
default, item title is the atom title, but it can be overriden locally in the
gallery.

Galleria supports many themes. In the basic theme, title and description can by
displayed by clicking the <i> button at the top left corner.
