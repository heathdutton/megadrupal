Imagepicker Views support includes some default views which are initially disabled.

Imagepicker Colorbox Gallery
This view provides a block showing a thumbnail which leads to a gallery
of images in a colorbox.
It requires the colorbox module so install that before enabling this view.
You should also enable 'colorbox-inline' in the colorbox admin configuration.

Imagepicker jCarousel Gallery
This view provides a block showing thumbnails in a jCarousel which when
clicked on shows the Full image in a colorbox.
It requires the colorbox module and the jcarousel module so install
them before enabling this view.
You should also enable 'colorbox-inline' in the colorbox admin configuration.

Imagepicker Slideshow Gallery
This view provides a block using galleria to display the images.
It requires the views_slideshow_galleria module and its dependencies
so install them before enabling this view.
You should also go into edit, select Block and go into Format: Slideshow | Settings
and then Save the view, this will avoid some notices.

To control which images to view you should use the Groups feature in Imagepicker
in combination with the Public feature. You can also select your images per user
by creating relationship Imagepicker: Image owner and filtering on user.
There are many different combinations possible, test them out on a clone, in Master
and then transfer the settings to a block, enable the block and test some more.

