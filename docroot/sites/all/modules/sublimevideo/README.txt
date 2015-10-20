Integration SublimeVideo Player sublimevideo.net in Drupal.

In the module the SublimeVideo player is used as a field formatter on a File field.

Installation and using
======================
1. Upload and enable the module SublimeVideo.

2. Register at https://my.sublimevideo.net/signup, choose a plan and enter
   the domain of the site where you want to integrate SublimeVideo.
   (For a testing purpose you can use a player on localhost. By default
   SublimeVideo works on localhost and 127.0.0.1. If you want to use
   a more specific development domain, please edit your site settings.
   http://docs.sublimevideo.net/site-settings)

3. Go to https://my.sublimevideo.net/sites and click to the button "Embed Code".
   Get a site's token from the popup window (it will appeared bellow of the window).
   Than go to Administration > Configuration > Media > SublimeVideo
   (admin/config/media/sublimevideo) and past the site's token into
   the field "Site's token".

4. For the content type you'd like select "Manage display" to configure and
   select "SublimeVideo" as the formatter on the relevant File field.

