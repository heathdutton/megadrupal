=====================================
========== Intro ====================
=====================================

Simple Crop module provides image widget with cropping behavior.

=====================================
========== Installation =============
=====================================

Go to /admin/modules page and enable the Simple Crop module.

=====================================
========== Configuration ============
=====================================

1. To add crop behavior for you image you need to change current image widget
   to SimpleCrop widget. To do this, go to "Manage fields" page of your entity
   (the Field UI module should be activated) and click on current image widget.
    Then choose "SimpleCrop" in widget type list and save changes.

2. To crop the image of the selected area - you need to add an "Apply SimpleCrop"
   effect to each image style which displays the cropped image. Go to
   Administration » Configuration » Media » Image styles and click on the "edit"
   link for the styles that needs to be cropped.

   After adding "Apply SimpleCrop" effect, you'll notice that the this effect
   will always become the first effect in the list. This is because cropping
   should always be done first, otherwise the result will be unpredictable.

3. When image styles are configured, you need to go to "Manage display" page
   and select configured image style in field format settings.
