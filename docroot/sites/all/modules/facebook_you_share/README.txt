Facebook You Share
------------------------

Description
-----------

A module containing helper functions for Drupal developers and administrators who want the ability to customize the post
they want to share on Facebook. This module provides you a simple admin interface to customize the post on facebook.
Here are the sections of the facebook post which can be customized.
- Title - The title to be shown on FB post.
- Link - The link attached to this post.
- Picture - If available, a link to the picture included with this post.
- Caption - The caption of the link (appears below the link name).
- Description - A description of the link (appears below the link caption).

Attached is an image to show the visual representaton of the post.(facebook_you_share.png)

Configurations
--------------

To share your site content on Facebook in customized way you first need to
create an app for your site on http://developers.facebook.com/

To do this,
1. Go to Administer -> Configuration -> Facebook You Share
2. Fill all details, including Facebook App ID and Facebook Redirect URL.
3. Type of content to share.
 3.1 Select the type of content you want to share on facebook.
4. Location for Facebook You Share button
 4.1 Select the location where you want to show Facebook You Share button.
4. Check the 'Enable checkbox' if you want the userpoints to be integrated with your share. For this, 
   make sure that the Userpoints module and Userpoints Node and Comments module are installed and enabled. 
5. Click on Save Configuration.

In the Advanced Settings tab, you will find the content type specific sharing settings. So, you can have different 
sharing settings for every content type.

Note: Make sure that your Facebook Redirect URL is same as Site URL in Facebook App settings.

Once you have saved all configurations,
go to the content of the content types you've selected in configurations and
you will find a button of Facebook You Share.
Click on Facebook You Share button to share your customised content on Facebook.

Compatibility Notes
-------------------
- Modules that use AHAH may have incompatibility with the query log and other footer info. Consider setting $GLOBALS['devel_shutdown'] = FALSE in order to avoid issues.
- Modules that use AJAX should idenify their response as Content-type: text/javascript. The easiest way to do that is run your reply through drupal_json().

Issues
------

If you have any concerns or find any issue with this module please don't hesitate to add them in the issue queue.

Maintainers
-----------
The Facebook You Share was originally developed by:
Swarad Mokal

Current maintainers:
Swarad Mokal - swarad07
Ayush Jain - ayushjn
Yogesh S. Chaugule
