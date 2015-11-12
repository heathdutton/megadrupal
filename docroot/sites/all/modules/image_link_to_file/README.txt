
-- SUMMARY --

The Image link to file module is a formatter for image fields that creates a
hyperlink from the image to a file field in the same entity.

This simple module solves a typical use case in which, when clicking on a
preview image (e.g. JPG or PNG file), a PDF file specified as a file field in
the same node should be opened.

To use the formatter, click on the Manage Display tab of a content type that
contains at least an image field and a file field. Then, in the Format column
of the image field, select "Image linked to file". To specify the format
settings, click on the button with the gear to reveal the "Image style"
selector, the "Image link" selector and the "Open image in a new window or tab"
checkbox.

Any of the available image style options can be selected for the image and any
of the file fields can be selected for the linked file. Optionally, the linked
file can be made to open in a new window or tab, rather than in the same window
or tab.

After your selection, click the Update button and then click the Save button.


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.


-- CONFIGURATION --

None.


-- FAQ --

Q: Must a file link be specified in any case? What happens if no link is
   specified?

A: A link to a file field is optional. If no link is specified, the image will
   not be linked to anything.


Q: What happens if the file field linked to the image is empty?

A: If the file field linked to the image is empty, the image will not be linked
   to anything.


Q: What if there is more than one file field in the node?

A: A selector in the settings form (accessible via the button with the gear)
   allows you to specify as link destination any of the file field in the node.


Q: What if there are multiple file values in the same file field specified as
   link destination?

A: The first file is linked to the image.


Q: Can I control whether the linked file opens in a new window or in a new tab?

A: This depends on the settings of your browser. This module can control
   whether the file opens in the same window/tab or in a new window/tab, but
   has no control over the choice of window vs. tab.


-- CONTACT --

Developer and current maintainer:
* Alex B - https://drupal.org/user/1815724
