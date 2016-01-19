CONTENTS
========

* Installation
* Views Integration
* Taxonomy Integration

Introduction
============
The Twitter Tags module provides additional functionality to the Twitter module
by analyzing Tweets and tracking hashtag data. This allows users to generate
customized lists of Tweets using Views and this additional data.

Installation
============
Install the module per normal Drupal module installation procedure. Note that
the Twitter module has a dependency on oauth_common which is now included in
the OAuth project.

If you've already got Tweets saved in your site, go to
admin/config/services/twitter/tags and initiate the batch process to parse and
save tag data. This process may be run at any time to refresh the hashtag data
saved for all Tweets.

Views Integration
=================
There is no literal dependency on the Views module and the Twitter Tags module
will parse Tweets and store hashtag data as they're saved, but there will be no
noticeable functionality to a user of the site.

Taxonomy Integration
====================
If your site is running the Drupal core Taxonomy module, additional options are
available in the Views configuration. There you will be able to relate terms to
Tweets giving you the ability to use existing filters, contextual arguments,
etc.
