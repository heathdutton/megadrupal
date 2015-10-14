
-- SUMMARY --

The FlexSlider Field Collection module provides a field formatter to render 
field collection entities in a FlexSlider.


-- REQUIREMENTS --

* Flex Slider - http://drupal.org/project/flexslider
* Field Collection - http://drupal.org/project/field_collection
* CTools - http://drupa.org/project/ctools


-- INSTALLATION --

* Install as usual, see https://drupal.org/node/895232 for further information.


-- CONFIGURATION --

* To use FlexSlider Field Collection in one of your content types, you need to 
  add a multi-value field collection field to the content type and set the 
  display formatter of that field to FlexSlider.

* There are four display options to configure on a per-field basis: 
  - the option set to use for the FlexSlider
  - the view mode of the field collection item to render as the FlexSlider slide
  - the image field to use for thumbnail navigation
  - the image style for thumbnail navigation

* The paging controls in the FlexSlider option set must be set to thumbnails 
  for the thumbnail image field and thumbnail image style display setting to 
  have any effect.
  

-- CONTACT --

Current maintainer:
* Genevieve Johnson (genjohnson) - http://drupal.org/user/1556426

FlexSlider Field Collection is mostly based on the FlexSlider Entityreference 
module by Bendik Brenne <https://drupal.org/user/1963442> and partially based 
on the Field Slideshow module by Jerome Danthinne 
<https://drupal.org/user/313766>.
