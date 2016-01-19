-- SUMMARY --

Chardin.js is a jQuery plugin that creates a simple overlay to display instructions on existent elements. It is inspired by the recent Gmail new composer tour 

-- INSTALLATION --

  1. Download the Chardin.js plugin (http://heelhook.github.io/chardin.js/) and extract the files under sites/all/libraries (so the result will be sites/all/libraries/chardin/chardinjs.min.js).
  2. Download and enable the module.

-- USAGE --

Currently there is limited integration into Drupal itself.  Once module is enabled add attributes to any element:

  data-toggle: Add the toggle ability to an anchor element
  data-intro: Text to show with the instructions
  data-position: (left, top, right, bottom), where to place the text with respect to the element

  <EXAMPLES>
  Create a toggle button:
		<a href="#" data-toggle="chardinjs" class="my-toggle-button-class" data-intro="This chardin button shows details about this view!" data-position="right">My Chardin.js Info Button</a>
	
	Add info to any element:
	  <img src="/path/to/my/cool/image.png" data-intro="This image was taken by me, is that not cool!?" data-position"bottom">
