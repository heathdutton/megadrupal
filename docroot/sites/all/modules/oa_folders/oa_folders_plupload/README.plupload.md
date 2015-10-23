Integration with oa_plupload app
================================

To integrate with the OpenAtrium multi upload app:

1) Download and enable the oa_plupload app.

2) Change the field widget to "Media multiselect" for folder files in
   admin/structure/types/manage/oa-folder/fields/field_oa_media/widget-type

Alternatively you can run the drush make file which will download the app and
patch it to use features_override to set the field widget:

$ drush make --no-core profiles/openatrium/modules/oa_folders/oa_folders_plupload/oa_folders_plupload.make .
 
