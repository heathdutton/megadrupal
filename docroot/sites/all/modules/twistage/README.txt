TWISTAGE INTEGRATION
--------------------

About Twistage (http://www.twistage.com)
---------------
Twistage is the leader of digital media management, delivering web, mobile, and connected TV solutions. For more go to the site http://www.twistage.com.


Purpose
-------
This module will allow your Drupal site to relatively seemlessly integrate with Twistage, a white label video hosting and delivery service (http://www.twistage.com). The "twistage" module features an administration page to create "profiles" (which mirror Twistage's "sites"). For each profile, it obtains information on videos stored with Twistage and keeps this locally in the Drupal database. It also allows you to set "preroll" videos (typically used to show ads). Once videos are known locally they can be shown easily in pages using a number of theme functions. The Twistage Integration module provides profiles either as blocks or standalone pages, the latter includes a simple jQuery-based video selector (using theme_table).

I have implemented in a very big site ukabbalah.com.

This package also includes the Twistage Publishing module, which provides a CCK field type to handle the uploading of Twistage videos in the node form. Users enter metadata (title, description, tags) for each video they wish to upload, and then after node submission they will be prompted for a file using Twistage's embedded publishing wizard. There is no limit on the number of videos per node.

Installation
------------
Install this module as you would any Drupal module. Unpack the .tar.gz file into the modules directory and enable in /admin/build/modules.

Setup
-----
Create profiles at /admin/settings/twistage. You will most likely want to do this while looking at your Twistage admin area, since some of the settings will need to be obtained from there (such as the site license key). Once you've created profiles and downloaded the video list from Twistage, you can enable videos and set a play order. Depending on whether you chose 'page' or 'block' for your profile, you can test it out by going to the page path you chose or enabling the block in the block administration form.

In order for your site to receive "pings" updating the video list, you will need to set a hook username and password, and then change your Twistage site settings (in console.twistage.com) to reflect the URL you see in the profile form.

If you need to embed the Twistage content into a node, you can either use the Twistage Publishing module to handle node/video associations, or you can just add a one-line theme function to a node and use the PHP code filter. An example:

      <?php
        print theme('twistage_page', array('my_profile_name'));
      ?>
      
To use the Twistage Publishing module, make sure you set the site publishing username in the profile settings, then create a new CCK field. Choose the profile you want to publish to in the field settings. To custom-theme videos uploaded in this manner you might do:

      <?php
        $video = twistage_publish_get_video_by_token($node->field_my_video[0]['token']);
        $preroll = twistage_get_preroll_video($video->pid);
        print theme('twistage_video', array($video, $preroll));
      ?>
      
Known Issues
------------
The "Add More" link in the node form for Twistage Publishing causes a series of Javascript errors when one or more existing videos are being displayed above it. For now, the workaround is to either add them at all at once, or add one at a time when editing a node with existing videos.

Planned Improvements
--------------------
The "Twistage Syndication" module from the Drupal 5.x version has been shelved for now, but it may reappear in the future (when I have more time to work on it).

I may also consider adding a way to edit the metadata of an existing video in Twistage Publishing. For now this data is read-only (although it updates when it receives remote pings from Twistage).

Caption implementation (may be with dotSub), youtube video implementation, Audio only, download video/audio functionality will be added soon.

A more detailed document describing the API for this module is also forthcoming.

Author & Maintainer
-------------------
Debraj Naik,  OSSCUBE SOLUTIONS
debraj@osscube.com
johnson.matthew.david@gmail.com
xmattus on drupal.org
