For more information about this module please visit http://drupal.org/project/twitter_db

-
Important: Uninstall any previous version of Twitter DB before you install this one.
-

Contents of this file
---------------------

 * About Drupal
 * About Twitter DB module
 * How to use
 

*** About Drupal
---------------------------------------

Drupal is an open source content management platform supporting a variety of
websites ranging from personal weblogs to large community-driven websites. For
more information, see the Drupal website at http://drupal.org/, and join the
Drupal community at http://drupal.org/community.

Legal information about Drupal:
 * Know your rights when using Drupal:
   See LICENSE.txt in the same directory as this document.
 * Learn about the Drupal trademark and logo policy:
   http://drupal.com/trademark


*** About Twitter DB module
---------------------------------------

This module downloads all tweets from a specific Twitter account (or hashtag) and
saves them in the database. The block display get the tweets from database and not
directly from Twitter.
That avoid problems with Twitter rate limit that returns following message if
over 150 requests is done per hour:
"Rate limit exceeded. Clients may not make more than 150 requests per hour."


*** How to use
---------------------------------------

  Note: Twitter has changed its API and requires the users to create an applications
        before the API resources can be accessed.

  1. Create an application on Twitter: https://dev.twitter.com/apps/new
  2. After you have created the application, generate an access token.
  3. Go to admin/config/media/twitter-db.
  4. Input follow information from your Twitter application:
          * Access Token
          * Access Token secret
          * Consumer key
          * Consumer secret
  5. Set if you want import an user timeline or a hashtag.
  6. Other settings are available and explained under each field.
  7. Once configured you can start by clicking the Update twitter table link on
     the top of the page to get the first feed of your twitter feed onto the database.
  8. You will now find a block called "Twitter DB: Last tweets" on "admin/structure/block"
     that you can place where ever you want in your theme.
  9. (Optional) If you want to add some fancy jQuery rotation, just install the
     module jQuery Plugin (http://drupal.org/project/jquery_plugin) and enable
     the feature on Twitter DB config page.

 
---------------------------------------
by Dhavyd Vanderlei - http://www.dhavyd.com