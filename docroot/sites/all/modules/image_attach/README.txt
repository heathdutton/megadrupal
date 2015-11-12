Image Attach
============

Image Attach enhances the functionality of entity reference fields that are used
to reference images as standalone nodes.

You should use Image Attach if:

* You want images that you attach to content to also be pieces of content in
  their own right, that can be viewed separately.
* You want reusable images (note that other ways of doing this exist).
* You want images that have meta-data, such as a title, description, author,
  and so on.
* You want to be able to change the file for an image, while keeping it attached
  to other content and keeping the rest of its data.

Image attach works by:

* enhancing the widget provided by Inline Entity Form module
* providing field formatters for the entityreference field that points to the
  image node

Requirements
------------

* Entity reference - http://drupal.org/project/entityreference
* Inline Entity Form - http://drupal.org/project/inline_entity_form

Setup
-----

You need the following structure:

* at least one node type that will function as an image. This needs to have an image field on it. In the rest of this, we'll refer to this as the 'image node type', but the name of the node type is immaterial. You can set up multiple node types to behave like this.
* another entity bundle which has an entityreference field that points to the image node type. This does not need to be a node. (The field may be configured
to point to more than one bundle, but they must all have a common image field.) We'll refer to this as the 'attaching entity type'.

Make the following configuration changes:

1. on attaching entity type's entityreference field that points to image nodes:
  1. enable the 'Image attach' setting
  2. select the image field that is on the image nodes, which should be used to
    display the image node in both the entityreference field widget and the
    formatter
  3. select the image style to use in the entityreference field widget
2. in the attaching entity type's display settings, select one of the two Image
  Attach formatters for the entityreference field:
  * 'Image attach basic': This allows setting whether to show the image node
    title, and which image style to use for the image.
  * 'Image attach view mode': This allows setting whether to show the image node
    title, and a view mode for the image node. You should then configure the
    image field on the image node type for the selected view mode. This
    formatter thus allows you to choose any formatter for the image field to
    show in the attaching entity type.

Further customization
---------------------

* To customize the way in which attached images are shown beyond the possibilities of the two included formatters, use the EVA module (https://drupal.org/project/eva) to show a View of the attached image nodes, and hide the actual field.
* To hide the node title on the inline entity form for attached images, use the Automatic Entity Label module (https://drupal.org/project/auto_entitylabel).
* Extra formatting options can be added with Field formatter settings module (https://www.drupal.org/project/field_formatter_settings).

Contrib module support
----------------------

* If using the Colorbox field formatter (https://www.drupal.org/project/colorbox) for the image field via the 'Image attach view mode' formatter, selecting the 'Per post gallery' option will group the images by attaching entities.

TODO
----

* There is no way to disable the creating of new image nodes in the inline entity form. See https://www.drupal.org/node/2056139.
