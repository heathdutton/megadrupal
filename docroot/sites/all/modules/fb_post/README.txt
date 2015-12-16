---DESCRIPTION---

This module allows registered users to add posts to their Facebook feeds when they create new content or make a comment
on your site.
It was designed to work as simple as possible for users with the minimum setup required, and it works similar to Twitter
Post sub-module from Twitter module package if you are familiar with it. All that user needs to do is to add connection
to his/her Facebook account in the Drupal account edit page. Connection is persistent, but there is a limitation that it
stays valid for 60 days (as of writing), and user must reconnect his/her account every 60 days (this limitation is set
by Facebook itself), so keep that in mind.


---INSTALLATION AND CONFIGURATION---

1. Install the module as usual.
2. Create Facebook application, which will be used for posting to users' feeds on their behalf.
   If you don't know how to do this, go to https://developers.facebook.com/ page.
3. Go to admin/config/fb-post and enter your Facebook application credentials.
4. Go to admin/people/permissions and under 'Facebook Post' section set permissions as desired.
5. Visit content type edit page such as admin/structure/types/manage/page for each content type where you want to enable
   the feature of posting to Facebook, and under 'Facebook Post' tab choose your settings.
6. If you go to your user edit page you'll see 'Facebook Account Connection' tab their where you can manage your
   connection.