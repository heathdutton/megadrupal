This feature will check for all necessary dependencies required to import ALL
information provided by the jsonc format of the YouTube gdata API, from any of
the video feeds (user favorites/uploads, all the YouTube standard video feeds
or a specific playlist) as supported and shown in the YouTube json responses:

http://code.google.com/apis/youtube/articles/view_youtube_jsonc_responses.html

The json responses tool can also be used for assisting you in getting the
right url when building your feeds.

The feature will install two new content types with all associated fields
(YouTube Importer and YouTube Video) and one Feeds Importer (YT Importer).

The feature has some dependencies:

    date
    features
    feeds
    feeds_jsonpath_parser
    feeds_tamper
    job_scheduler

To install the feature as per the Features module documentation:

1. Download the YouTube Importer Feature and extract it.
2. Put the folder in a suitable folder in your Drupal installation. A good
choice is sites/all/modules/custom/features, but any sub-folder to
sites/all/modules will do.
3. Go to the features administration page (admin/build/features), check
YouTube Importer Feature to activate, and save the settings. Done!

If you need to disable a feature at a future time, you just need to visit the
Feature administration page, deselect the appropriate checkbox and hit Save
settings.
	
To use, just simply create a new YouTube Importer node with the jsonc feed you
grabbed from the json responses tool. Periodic import/update is set to off by
default and YouTube only allow 50 items per feed. Create more nodes using the
start-index feature of gdata api to import more than 50.

