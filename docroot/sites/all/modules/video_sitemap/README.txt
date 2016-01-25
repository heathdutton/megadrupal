Introduction
-------
The Video Sitemap module. Provides possibility to create video sitemap.
Currently is limited to videos uploaded only to nodes. Videos added to block or other entity
types are not indexed. More information on video sitemaps can be found at
https://developers.google.com/webmasters/videosearch/sitemaps

Requirements
------------
This module requires the following modules:
1) File Entity (https://www.drupal.org/project/file_entity)

Install
-------
Simply install Video Sitemap like you would any other module.

1) Copy the video_sitemap folder to the modules folder in your
installation.
2) Enable the module using Administer -> Modules (/admin/build/modules).

Configuration
-------------
Configuration page link is /admin/config/search/video-sitemap.
1) Set minimum lifetime of sitemap used on cron.
2) You can exclude file mimetype from indexing in case if some types are not
available through http, as required by Google, or simply if you don't need it to be
in the sitemap.

Use
-------
1) Open /admin/config/search/video-sitemap/rebuild
2) Rebuild video sitemap.
3) Video sitemap will be available at /sitemap-video.xml
