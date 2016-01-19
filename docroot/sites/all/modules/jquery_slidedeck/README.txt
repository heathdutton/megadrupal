INTRODUCTION
============

  This module provide jquery slidedeck effect(like http://www.slidedeck.com )
for set of blocks. It provides admin interface to add more 
slidedeck widgets, for each slidedeck we can choose set of 
blocks as slide deck content.

REQUIREMENTS
============

  Slidedeck Module works with the Jquery slidedeck plugin.
Download the slidedeck Jquery plugin from http://www.slidedeck.com.
Unzip add place the file  slidedeck.jquery.lite.js in libraries folder.

File Path should be sites/all/libraries/slidedeck.jquery.lite.js 


INSTALLATION
============

1. Copy the slidedeck module directory to your modules directory
2. Download the slidedeck Jquery plugin from http://www.slidedeck.com,
   extract the zip and place the slidedeck.jquery.lite.js at folder
   sites/all/libraries.
3. Enable module thorugh modules admin interface (admin/modules).

USAGE
=====

1. Create new slidedeck widget block through 
   Admin > Structure > Slidedeck > Add Slidedeck

2. On the add slidedeck form, give title and choose 
   the slidedeck skin. The slidedeck contents are blocks,
   add what blocks you need to include as a slidedeck content. 
   If you don't find the block you require, 
   add block and select here.

3. Once slidedeck is created, it will create a block.

4. Visit Admin > Structure > Blocks 
   and add your slidedeck block to the region you wish.

CUSTOM SLIDEDECK SKINS
======================

This module is shipped with 3 different skins for slide deck. 
If you want to have new look for your slidedeck,
you can add your own skin by following below steps.

1. Creat your custom skin css file by copying the 
   skin css files from this module at path skins folder.

2. This module provides a hook called 
   hook_slidedeck_skin_alter. Implement this hook as below
   
   MODULE_slidedeck_skin_alter(&$skins) {
     $skins['skin_key'] = array(
      
      // this is the skin file path
      'path' => drupal_get_path('theme', 'your_theme') . 
                '/slidedeck_cust_skin.css', 

      /*
       *  This is the label which will appear 
       *  on the add slidedeck form
       */
      'value' => 'Blue and Black 700px'), 
     );
   }
3. Choose your custom skin for the slidedeck and save.
