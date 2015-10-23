CONTENTS OF THIS FILE
---------------------

 * Overview
 * Features
 * Requirements
 * Reccomended modules
 * Installation
 * Author

OVERVIEW
-----------

The Filefield Role Limit module is an extension of File field module and adds 
the capability to limit the max upload file size for each different user role.

FEATURES
-----------

The module creates new settings for each File field to define different file 
upload max size limit for each user role.
Different limit values can be provided for each user role and the module will 
automatically switch to default File field max upload size behavior if no 
settings per user role are provided.

REQUIREMENTS
------------
No addictional modules required, since File module is in Drupal 7 core.

RECOMMENDED MODULES
------------

The Filefield Role Limit module works also with Image field module, that is 
in Drupal 7 core too.
 
INSTALLATION
------------

1) Copy the FileField Role Limit folder to the modules folder in your 
   installation.

2) Enable FileField Role Limit modules using 
   Administration -> Modules(/admin/modules).

3) Create a new file field in a content type. 
   Visit Administration -> Structure -> Content types 
   (admin/structure/types), then click "manage fields" on the type you want to 
   add a file upload field. Select "File" as the field type and "File" as the 
   widget type to create a new field.

4) In new file field settings, open "Maximum upload size per file per role" 
   fieldset. Inside this fieldset you'll be able to set a default upload limit
   size for each user role. Set the value for the roles you need to.

5) Upload files on the node form for the content type you set up.

AUTHOR
------

Alberto Colella
http://drupal.org/user/460740

Contacs:
http://drupal.org/user/460740/contact
kongoji@gmail.com
