********************************************************************
           VOCABULARY IMAGE  M O D U L E
********************************************************************
Original Author: Brajendra Singh
Current Maintainers: Brajendra Singh

********************************************************************
DESCRIPTION:

   This module gives you control over the images for vocabularies.
	This is a light weight module. This is developed in consideration with adding vocabulary images. In some cases your project( E-commerce site) needs to display list of vocabularies with their images.
	One of my project needs to same feature. I searched a lot but not found any solution. Then after all I decided to write their own module and also wants it to contribute. So it can provide some help to reach at appropriate solution for this type of listing.

********************************************************************
INSTALLATION:

1. Place the entire vocabulary_image directory into your Drupal modules/
   directory or the sites modules directory (eg site/default/modules)


2. Enable this module by navigating to:

     Administer > Build > Modules

***********************************************************************
USES:

you can use following hook and function for getting vocabulary images -
	- hook_taxonomy_vocabulary_load()
	- _get_vocab_image_path($vid)
