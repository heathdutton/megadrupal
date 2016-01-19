Social Timeline module:
------------------------
Maintainers:
  Rob Lee (http://drupal.org/user/893454)
Requires - Drupal 7


Overview:
--------
The Social Timeline module lets you retrieve status/posts/videos/images
from different social networks in a timeline format from the newest to
the oldest.


Features:
---------
* Get status/posts/videos/ images from differents accounts in the same
  social network
* Twitter, Facebook Page, Youtube, Delicious, Flickr, Dribbble, Digg,
  Pinterest, Tumblr, Instagram, Google+, Lastfm
* Retrieve Youtube videos using search keywords.
* Retrieve tweets using a hashtag.
* Different display styles.
* Limit the number of Feeds to retrieve.
* Add multiple custom feeds
* Show/Hide Social Icons.
* Social Filter Support.
* Cross Browser Support.
* Simple to Customize.
* Full Documentation.
* Demo examples included.


Installation:
-------------
1. Download and unpack the Social Timeline module directory in your
   modules folder (this will usually be "sites/all/modules/").
2. You need to purchase the jQuery Social Timeline plugin ($6, I did not create
   or sell it):
   http://codecanyon.net/item/jquery-social-timeline/2390758?sso?WT.ac=cate...
3. Unpack it to sites/all/libraries/social-timeline.
4. Go to "Administer" -> "Modules" and enable the module.


Configuration:
--------------
Go to "Configuration" -> "Social Timeline" to find
all the configuration options.

You will want to setup the configuration before placing the Social-
Timeline block


Set Twitter Credentials:
------------------------
1. Add a new Twitter application.
2. Fill in Name, Description, Website, and Callback URL
   (don't leave any blank) with anything you want.
3. Agree to rules, fill out captcha, and submit your application.
4. Click the button "Create my access token" and then go to the OAuth tab.
5. Copy the Consumer key, Consumer secret, Access token and Access token secret
   into the files in the libraries folder: twitter_oauth/user_timeline.php
   and twitter_oauth/search.php.


Usage:
------
The Social Timeline module has two parts to it, the configuration page
and the block that is provided on the Structure -> Blocks page. Once
you configure the module on the configuration page: Configuaration -> Social-
Timeline, you can then go to the Blocks page and place the jQuery Social-
Timeline block in a region of your choosing.
