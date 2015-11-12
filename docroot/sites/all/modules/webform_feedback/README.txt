
Module: Webform Feedback
Author: Protitude <http://drupal.org/user/79388>


Description
===========
Adds a pop-up lightbox like feedback form. 
Inspired by http://www.feedbackify.com

Requirements
============

* Webform


Installation
============
* Copy the 'webform_feedback' module directory in to your Drupal
sites/all/modules directory as usual.
* Enable module (requires webform module).
* Create a webform that you want to use as a 
pop-up.
* Enable block under the webform settings 
(node/#/webform/configure) under advanced 
settings.
* Once enabled as a block. Set the block 
to show up somewhere in a context or under 
block settings.
	* You may want to style this block as this will be 
	what shows up for the small percentage of users 
	that don't have javascript.
* Go to webform_feedback settings 
(admin/config/content/webform-feedback).
	* Select your webform and hit save. This will hide 
	the block and add a button that will overlay the form.
