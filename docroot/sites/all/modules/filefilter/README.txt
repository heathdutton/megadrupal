# File Filter module

## Notice
This module is still under development. Although it's proven to be working in
the most common use cases, it's still very rough around the edges and may not 
display your content properly in certain situations. Please 
[file a bug-report issue][1] if such a situation should occur.

[1]: http://drupal.org/node/add/project-issue/filefilter

## Installation
Download and enable the File Filter module, then enable the file filter in one
or more of your text formats at admin/config/content/formats.

## Usage
### Setting up your template
Once you've set up a file field (for example, `file_image`) for a certain
content type, you must make sure that:

1. the field is visible in the field-display settings for your desired content
type
2. the field is hidden at the template level.

To hide your field in your template, edit node.tpl.php in your theme (or the
specific template file for your content type) and add, at the top of the file,
within the PHP tags:

    hide($content['file_image']);

Of course you'll have to use the specific name of your field.

If you don't hide your field in the template, it will appear both in the
specified spot in the body and wherever the file is set to appear by the field's
display settings for your content type. On the other hand, if you hide the field
in the display settings, it will not appear at all.

### Filter syntax
The easiest way to use the file filter is by using this syntax in the node body,
in the spot where you'd like your file to be displayed.

    [image:1]

Note that you need to enter the field name without the initial `field_`. You can
also use progressive numbers (starting from 1) for the different values in
multiple-value fields.

As another example, if the field you set up is `field_coverimage`, and you want
to display the fourth value in the field, the correct syntax will be

    [coverimage:4]

Using the basic syntax, the file will be displayed according to the field's
display settings for the specific content type.

### Using image styles
In the case of image fields, it's possible to specify a different image style
from the default one provided in the field's display settings. If you want to
display the third `file_image` value using the `large` style, simply use this
syntax:

    [image:3|size:large]

If the specific image style isn't found, the image will be displayed in the
default style for that content type.

### More file filter attributes
Note that attributes can be entered in any order.

#### No title
While normally images are displayed with the title attribute (if present) used
as caption, you can choose not to display a caption by adding a `notitle`
attribute to the syntax:

    [image:3|size:large|notitle]

#### Alignment
The File Filter module comes with a few preset styles (that can be overridden in
your theme) to simplify image alignment. The `align` attribute defaults to
`left`, and can also take `center` and `right` as possible values:

    [image:3|size:large|align:right]

#### Extra classes
You can also specify custom classes using the `class` attribute. Multiple classes
must be separated with a space.