Havasu is a clean, bright theme for Drupal 7 that includes a built-in jQuery slideshow.

For a full description of the theme, visit the project page:
http://drupal.org/project/havasu

-- INSTALLATION --

Install as usual, see http://drupal.org/getting-started/install-contrib/themes for further information.

-- CONFIGURATION --

I. Place the search block in the correct regions

1) Go to Structure > Blocks
2) Place the 'Search Form' block in the Search region. (By default it is placed in the First Sidebar region.)

II. Customize the Havasu slideshow:

This theme features a slideshow on the homepage. Out of the box, the theme comes with some placeholder images and text that you'll likely want to replace. 
To do so:

1) Open the file "page--front.tpl.php" which is in the "templates" folder of the Havasu theme
2) Scroll down to line 44, which reads, "<!-- Slideshow -->". This is where the code for the slideshow starts.
3) The slides are listed individually below that code.
4) To swap out an image:
  a) Create your new image with the follow dimensions: 930 pixels wide by 300 pixels high
  b) Save it in the 'images' folder of the Havasu theme.
  c) In the page--front.tpl.php file, replace the last bit of this image line with the the title of your image:
     <img src="sites/all/themes/havasu/images/slide-image-1.png" width="960" height="300" alt="Slide 1">

     So if you have created a new image called "testimage.jpg", you would change the line to read as follows:
     <img src="sites/all/themes/havasu/images/testimage.jpg" width="960" height="300" alt="Test Image">

5) To change the descriptions and links, just edit the text in the page--front.tpl.php file below the corresponding slide image.

