--ABOUT--

The Histogram module allows you to generate a histogram for images in nodes.

It can create histograms for JPG, GIF or PNG images.

The module creates a new field type and widget called "histogram". Once attached
to a node, it will automatically create a histogram for the image field on the
node when the node is saved. Currently, it only supports one image field per
node per histogram field.

It can create stacked three color histograms as well as merged histograms. The
background color can be set to any hexadecimal value. These options can be
selected on an image by image basis.


--REQUIREMENTS--

images
field
file


--USAGE--

Next, on the manage fields page for the selected nodetype, add a new field with
the field type "Histogram."

If your node has multiple image fields, select which one you want to generate
a histogram with on the settings page.

The "Histogram Type" option determines whether your histogram will be three
separate channels stacked on top of each other or a single merged histogram.
On black and white images, a single channel histogram will be generated
regardless of the value of this option.

"Force Single Channel Histogram" allows you to generate a single channel
histogram by forcing the histogram generator to process the image as if it
were black and white. This is useful on monochrome images which are tinted
because they fail to automatically be seen as black and white by
the processor.

The background color field allows you to specify a hexadecimal background
color for your histograms. If left blank, a transparent background will be
generated. This is useful for situations where the histogram should appear
on a patterned background.

In order to reduce unnecessary wait times on node save, the histogram will only
be generated when the "Generate Histogram" radio button on the node edit form is
set to "Yes". After making any changes to the image or the histogram settings,
make sure you regenerate the histogram.


--Limitations and Future Plans--

Right now the histogram will only work with node entity types. I want to change
this in the future to work with all entity types.

Currently the module creates a png image of the histogram which is saved
alongside the original image file. In future versions, I plan to replace this
with a javascript graphing library that generates the histogram dynamically.


--CONTACT--

Matthew Messmer (machostache) drupal.org/user/2496364
