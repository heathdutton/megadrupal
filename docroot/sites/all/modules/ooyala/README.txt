Description
-----------
This module implements a new field type that allows videos to be uploaded
directly to the Ooyala video hosting platform. This module requires an Ooyala
account (http://www.ooyala.com/). Free trials of Ooyala are also available at
http://www.ooyala.com/free_trial.

The Ooyala module provides tight integration with the Ooyala platform including:

- Uploading directly to Ooyala from the node form with the ooyala_upload module
- Automatic publishing of content when processing is completed by Ooyala
- Automatic pulling of thumbnails from Ooyala based on Backlot settings
- Bulk import of existing Ooyala videos from Backlot

Recommended modules
-------------------
Views (display lists of videos as a player)


Installation
------------
After enabling the module and signing up for an Ooyala account, you'll need to
set some options both in Drupal and in the Ooyala Backlot.

Sign into http://backlot.ooyala.com and visit the "Account" tab. Under the
sub-tab for "Developers", find the "API Key" and "API Secret". Copy and paste
these values into the Ooyala settings in your Drupal site at
Administer >> Configuration >> Media >> Ooyala settings
(admin/config/media/ooyala/settings). Save the Drupal settings.

Now go back to Backlot, and fill in the "API Ping URL" with the URL of your
website, with "ooyala/ping" as the path, i.e.
http://www.example.com/ooyala/ping

The ping URL allows Ooyala to ping your site when video processing is finished.
The Ooyala module will auto-retrieve thumbnails for videos when receiving a ping
and (optionally) publish the associated node that contains a video. If needed,
multiple URLs may be specified in backlot by separating by commas (such as for
development and production environments).

Now, add Ooyala fields to your content types. If you want to upload videos
from Drupal to the Ooyala backlot, follow the README in the Ooyala Uploader
module to add support for the Ooyala Web Uploader to your site.

About Ooyala chapter markers (bundled module)
----------------------------------------------
The Ooyala chapter markers module implements a new field type that may
collaborate with an Ooyala video field provided by on the same content type. It
allows users to specify chapter titles and times in long video files. When
displayed, users can click the chapter title and the Ooyala player will jump to
the chapter time.
