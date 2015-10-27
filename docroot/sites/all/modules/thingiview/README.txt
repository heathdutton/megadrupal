
INSTALLATION:

As this module relies on the libraries module, you will be required to have one
of the following directories present;
(using 'xxxxx' as a placeholder for the current profile or site you're using)


sites/xxxxx/libraries
sites/all/libraries (recommended)
profiles/xxxxx/libraries

Within whichever libraries directory you choose to create;

cd /path/to/your/libraries/directory
git clone git@github.com:tbuser/thingiview.js.git

This will create a directory called 'thingiview.js' containing the relevant
javascript files for the Javascript 3D Model Viewer. Ensure you have the
following structure and files else you will run into problems;

/path/to/libraries/thingiview.js/javascripts/binaryReader.js
/path/to/libraries/thingiview.js/javascripts/plane.js
/path/to/libraries/thingiview.js/javascripts/stats.js
/path/to/libraries/thingiview.js/javascripts/thingiloader.js
/path/to/libraries/thingiview.js/javascripts/thingiview.js
/path/to/libraries/thingiview.js/javascripts/Three.js

USAGE:

This module creates a display formatter for the 'file' field type, see
hook_field_formatter_info() especially in "/modules/field/field.api.php".

In order to use it you will need to create a file upload field in your chosen
content type. For example, visit "/admin/structure/types/manage/article/fields"
and add a new field called "field_3d_test_upload" choose the field type 'file'
and save it. Change the "Allowed file extensions" to only accept 'stl' and save
these settings.

Now go to "/admin/structure/types/manage/article/display" and change the display
format for the field you have just created. You will see in the dropdown you can
choose "3D View" - select this and leave the settings as per defaults (you can
mess around with these later).

Lastly, create an article node and upload a known good stl file (I'd recommend
using one from the thingiview.js example folder). Once created the 3D viewer
should be active and displaying the 3D image.
