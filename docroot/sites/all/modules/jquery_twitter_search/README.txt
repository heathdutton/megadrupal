jQuery Twitter Search

With this module you can create as many jQuery Twitter Search blocks as you like to your website. You can give each block it’s own search term. The block will then show a stream of relevant Tweets.

Each block is highly configurable in the UI and custom CSS styling.

This module requires:

  The jquery.twitter.search.js plugin.
  The twitteroauth library
  You own Twitter application
  libraries

Installation:

  Install this module.
  Install libraries.
  Download the jquery.twitter.search.js plugin. Create the folder sites/all/libraries/jquery_twitter_search and add jquery.twitter.search.js to the newly created folder
  Download the twitteroauth library and copy it to the libraries folder so the path looks like libraries/twitteroauth/twitteroauth/twitteroauth.php
  Go to https://dev.twitter.com/apps and create a new application. Follow the given instructions. You will need the information provided at the dashboard later
  In your site go to admin/config/services/jquery-twitter-search
  At the Oauth configuration tab you can enter the required information about your Twitter application.
  Optionaly ad Twitter search configuration
  Now add your own block in the jQuery Twitter Search blocks list and configure it to your needs. Check http://jquery.malsup.com/twitter/ for more information
  Enable your block in admin/structure/block.

NOTES : you have to update jquery_twitter_search.js to make it work with OAuth and Drupal. Also as of version 3.0 this module require twitteroauth library.