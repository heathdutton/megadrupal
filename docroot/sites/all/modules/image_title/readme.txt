This module helps you to set an image as the title for nearly any page using a simple file attach form.
Just install module by putting into modules folder and installing.
While creating node you will see an additional fieldset option called "Title image" where you can upload an image to replace the title.
In block admin you will find a Title Image Upload block - place this on all pages except node/* to add images to non-node pages.
This will expose a form called "Title image" in the region where you placed the block.
 
By default all images go in to an img-title folder in the default files directory for Drupal, but you can change this directory name in admin:
admin/settings/image-title

To delete the title image, mark the checkbox and save.


 Still in to do list...
   Node type configuration for type based setting - Done by dksdev01
   More theming options.
   and what else you will need ...
 
 
The $node->title text is included inside a <span> tag in the title variable, when a title image is available, which is being passed up also.
This is merely for search engine optimisation reasons - so Google can still index your H1 text, as opposed to it only being an image tag. 
to display only the image on screen but allow the node title to be indexed by search engines still.

The title image upload form is also presented as a template variable, $image_title_form, available to all page.tpl.php files if you want to place it in a template.


 if you have any problem with this module contact dksdeveloper at gmail dot com
 
Alternate -- explained here http://drupal.org/node/221854
Alternative approach for Drupal 5.x can be found here -- http://drupal.org/node/221854
