//$Id:


-- SUMMARY --

LayoutStudio Extras is a companion module for the LayoutStudio starter 
theme (http://drupal.org/project/layoutstudio). It give themers the 
ability to:

    * Create a LayoutStudio sub-theme from the Appearance page.
    * Specify layout and layout dimensions changes based on path.
    * Adds drush support so developers can create LayoutStudio sub-themes 
      from the command line.
		* Add a site copyright block with automatic year spanning (ie. 2009-11).
    * Adds a site credit (either with text or image) block 
      (ie. Web Development by Acme Inc).


-- SUMMARY --

Please note that for the automated sub-theme creation, 
we are currently waiting for the following patch to make it 
in an official Drupal 7 release: http://drupal.org/node/1019834


-- DRUSH SYNTAX --

Basic syntax: drush layoutstudio [theme_name] [themes_directory]

For example:drush layoutstudio cooltheme

will create a new LayoutStudio sub-theme in the sites/all/themes 
directory. If you have a multi-site installation, add the themes 
directory argument:

drush layoutstudio cooltheme sites/examples.com/themes


-- CONTACT --

Current Maintainers:
* Rene Hache (rhache) - http://drupal.org/user/64478
* Ryan James (rjay) - http://drupal.org/user/363916
* Saren Calvert (Sarenc) - http://drupal.org/user/730816

This project was sponsored and maintained by: 

  * northStudio Inc. - http://www.northstudio.com
    northStudio specializes in designing and developing Drupal sites including
    custom web applications, e-commerce and search engine optimization.