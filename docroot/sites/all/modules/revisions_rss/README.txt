Drupal revisions_rss Overview
------------------------------------------------------------------------------
This module generates an RSS feed for node revisions. This allows users to 
subscribe to updates on individual nodes. This functionality is particularly 
useful for tracking changes to collaboratively developed content on auto-
revisioning node types.

This module allows administrators to customize which node types will have feeds 
generated for them, and which roles have permissions to view those feeds. The 
number of items per feed is taken from Drupal's internal RSS Publishing 
settings, but preferences about teasers and node full text are ignored; the 
revisions feed only contains node title, revision date and revision author.


Installation
------------------------------------------------------------------------------
  - Copy the module files to your modules directory.
  - Enable the module on the admin page (admin/modules)
  - The node types to generate feeds for can be controlled on the "Revisions RSS 
    publishing" tab of the "RSS publishing" page (admin/config/services/rss-publishing)


Credits / Contact
------------------------------------------------------------------------------
This module was created by Alex Jarvis (drupal@alexjarvis.net). It was
originally sponsored by the Great River Regional Library (http://www.griver.org).
