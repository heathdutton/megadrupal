
-- SUMMARY --

The Color Tagger allows nodes and other entities to be automatically tagged with
taxonomy terms based on the colors in an image on an image field in that entity.

In order to work, it requires a few fields be in place on the entity you want
to tag and the taxonomy terms.


-- REQUIREMENTS --

* PHP 5.3+ with GD Library
* Entity API module (https://www.drupal.org/project/entity)


-- INSTALLATION --

Color Tagger comes with a Color Tagger Example module that can be enabled to
quickly get you up and running. The example module creates a Picture content
type and a pre-populated Colors taxonomy vocabulary. These come with all fields
set up which Color Tagger needs to function.

If you want to get Color Tagger working on a different content type or you just
do not want to use the example module, follow these steps to get it working.

The taxonomy that you wish the Color Tagger to use needs to have a field added
to it for storing the hexadecimal color values that Color Tagger uses to
associate the pixels in the image with the taxonomy terms. This field can be a
simple textfield or a Color Field field. This field can support multiple values
if you wish to associate more than one hexadecimal code with a term.
The Color Tagger Example module uses two hexadecimal colors for most terms to
help color tagging accuracy.

The node or entity that you wish to automatically tag will need an Image field
and a Taxonomy Term Reference field. The Term Reference field should use the
Color Tagger Widget.

On the field edit page for your Term Reference field, choose the Image field you
 want Color Tagger to process and choose the field on the taxonomy term that
 stores the hexadecimal codes.

After creating the taxonomy terms with the associated hexadecimal values, when
the content is saved, the terms will be automatically applied based on the
colors in the uploaded image. If you need some help deciding what values to
choose for your terms, check out the defaults provided in the Color Tagger
Example module.

In order to provide an ability to manually override the selected terms, if the
Term Reference field has values entered on the content edit screen, Color Tagger
will not run. If you want the terms to be processed, make sure the autocomplete
field is blank when the content is saved.


-- CONFIGURATION --

* Configure settings at:
   Configuration > Media > Color Tagger > Color Tagger Settings

* Other settings can be configured on a field by field basis on the field
settings page for the taxonomy term reference field.

* Ignoring Colors:
  Colors can be set as ignored so that they will not be tagged.
  In order to do this, the colors must first be declared as taxonomy terms
  in the same vocabulary Color Tagger is using. This is because the processor
  can not ignore what it does not know exists. If you have colors you want
  ignored, they can be configured on the field edit page for the Term Reference
  field.

* Maximum Number of Colors and Threshold:
  On the Color Tagger settings page, you can choose a maximum number of colors
  that should be assigned to each entity. The default is 2.

  Sometimes you will have images that are comprised of almost entirely the same
  colors with only small amounts of other colors. Depending on your max number
  of colors setting, in these situations you might find the Color Tagger assigns
  colors that make up a very small percentage of the image. You can specify a
  percent threshold on the settings page so that colors that comprise a
  percentage below the threshold will not be tagged. By default, the threshold
  is set to 10%.
