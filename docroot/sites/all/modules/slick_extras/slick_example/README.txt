Slick Example Module
================================================================================
Provides samples for Optionsets, Image styles, Slick Views blocks and a few
supported alters.

Please do not use this module for your works, instead clone anything and use it
to learn how to make the most out of Slick module. This module will be updated
at times to reflect the best shot Slick can give, so it may not keep your
particular use.

The Slick example is just providing basic samples of the Slick usage:
- Several optionsets prefixed with "X" available at "admin/config/media/slick".
  You can clone what is needed, and make them disabled, or uninstalled later.

- Several view blocks available at "admin/structure/views".
  You can clone it to make it yours, and ajust anything accordingly.

- Several slick image styles at "admin/config/media/image-styles".
  You can re-create your own styles, and adjust the sample Views accordingly
  after cloning them.


REQUIREMENTS
--------------------------------------------------------------------------------
- field_image, as available with Standard install.
- field_images, must be created before seeing this example useful immediately.
- node/3 containing field_images.

See "admin/reports/fields" for the list of your fields.


MANAGE FIELDS
--------------------------------------------------------------------------------
To create the new field "Images":
  1) Go to: admin/structure/types
     Choose any "Manage fields" of your expected content type, for easy test
     I recommend Article where you have already a single Image. Basic page is
     fine too if you have large carousels at Basic pages. Or do both.
  2) Add new field: Images (without "field_" as Drupal prefixes it automatically)
  3) Select a field type: Image
  4) Save, and follow the next screen.
     Be sure to choose "Unlimited" for the "Number of values".

You can also do the similar steps with any fieldable entity:
  o admin/structure/field-collections
  o admin/structure/paragraphs
  o admin/config/people/accounts/fields
  o etc.

All requirements may be adjusted to your available instances, see below.

To have various slick displays, recommended to put both "field_image" and
"field_images" at the same content type. This allows building nested slick or
asNavFor at its very basic usage.

You can later use the pattern to build more complex nested slick with
video/audio via Media file fields or SCALD atom reference when using with Field
collection module.

Shortly, you have to add, or adjust the fields manually if you need to learn
from this example.


VIEWS COLLECTION PAGE
--------------------------------------------------------------------------------
Adjust the example references to images accordingly at the Views edit page.
 1) Go to: admin/structure/views
 2) Edit the Views Slick example before usage, adjust possible broken settings:
    admin/structure/views/view/slick_x/edit
    The first block depends on node ID 3 which is expected to have
    "field_images":
    admin/structure/views/view/slick_x/edit/block

    If you don't have such node ID, adjust the filter criteria to match your
    site node ID containing images.
    If you don't have "field_images", simply change the broken reference into
    yours.




Slick grid set to have at least 10 visible images per slide to a total of 40.
Be sure to have at least 12 visible images/ nodes with image, or so to see the
grid work which results in at least 2 visible slides.

See slick_example.module for more exploration on available hooks.

And don't forget to uninstall this module at production. This only serves as
examples, no real usage, nor intended for production. But it is safe to keep it
at production if you forget to uninstall though.
