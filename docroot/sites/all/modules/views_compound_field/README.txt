Views compound fields
--------------------

This module provides two compound fields, which output content from other 
fields in the view.
In both cases, you add the field to your view (found in the 'Global' group),
then in the field's settings, select other fields you have already added to
the view.
If you want the fields you select to only be output in the compound field,
you need to set them to be excluded from display.

The two fields work as follows.

Collapsing field
----------------

This outputs only the first value it finds from the set of fields.

Example: you have a blog node type with both image and video fields. In your
blog view you want to show *one* piece of media, either the video if there is
one, or the image if there is no video.

Set up the fields thus:
  - video
  - image
  - collapsing field
  
and select the video and image fields in the collapsing field settings.
A blog with only a video will show that, with both will show the video, and
with only an image will show the image.

Gathering field
---------------

The outputs all the selected fields within one field.

Use this for advanced theming, such as floating an image + title alongside the remaining fields.