This module integrates the Mapplic interactive map/floorplan plugin into Drupal.

It can be used for multi storey buildings, airports, shopping malls, department
stores, resorts, hospitals, harbors, campings or festivals, ski tracks,
sport stadiums, cruise ships or anything you can think of, your imagination
is the limit. It's also perfectly suitable for touchscreen kiosk maps.

Features:

Unlimited landmarks
 Any number of locations with unique pins and various actions
Unlimited floors
 Multiple floors are supported without any limitation
Unlimited floors
 Multiple floors are supported without any limitation
Deeplinking
 Every location can be referenced by it's own URL
Browser support
 Works great in every major browser (even Internet Explorer 8)
Responsive design
 Provides optimal experience across a wide range of devices
Touch optimised
 Touchscreen devices, like tablets and smartphones are also supported

This is a paid plugin and requires a license.
Plugin website: http://www.mapplic.com/plugin/

!!!!! IMPORTANT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Please make sure you purchase the jQuery version, currently for $18 (2015.07.20)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

If you need help in the installation, please read the INSTALL.txt

================================================
SETUP AFTER INSTALLATION
================================================

Once you have the module successfully enabled, notice the followings:
1) There is a new menu item available in the Configuration menu to finetune the
 module settings. Accessible also via the admin/config/services/mapplic URL.

2) There is a new Taxonomy called 'Mapplic landmark categories'
 This vocabulary is used to categorize the landmarks.

3) There is an another Taxonomy called 'Mapplic floors'
 By default you have a 'Basement' floor set up, in case you need a one level
 floorplan you are good to go with this one.

 Go and edit the 'Basement' term and you need to upload the map SVG file
 and smaller JPG version here to function. (You may find some SVG map examples
 in the package you downloaded for initial testing)

 In case you need multiple floors feel free to add more taxonomy terms.

3) You have a new content type named 'Mapplic landmark'
 Use this content type to upload all your landmarks on the map.

4) A new block is available in Structure / Blocks
  Use this block to add the interactive map to the desired page.

When you are adding the landmarks, most of the fields are quite straightforward
 however there are a few which may require further clarification.

Field: 'ID of the landmark in the SVG file'
The different shapes in the SVG files representing one landmark can have a
unique identifier. This can be added manually with a text editor or via your
favourite vector graphic suite program.

Field: 'X/Y position of the landmark on the Mapplic map'
This identifies the - center - coordinate of the landmark shape, if you turn on
'developer mode' in the Mapplic configuration section, you'll see coordinate
boxes appearing above your map to help you identify the coordinates.

================================================
HOW TO BUILD THE MAP
================================================

Check the doc folder of your downloaded package for more instructions.
Screencast how to create SVG files with Adobe Illustrator:
https://www.screenr.com/ZstN
