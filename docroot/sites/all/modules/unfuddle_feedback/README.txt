
This module provides integration between the feedback module (http://drupal.org/project/feedback) and the ticketing system Unfuddle (http://unfuddle.com), paid account required.  It allows users to use feedback module to create Unfuddle tickets referencing specific paths on the site.  It relies on unfuddle_api module.

Configure the Unfuddle API settings at admin/settings/unfuddle_api.  Specify a project number on those settings.  You should create a user account in Unfuddle with only the permissions required to post tickets to your Unfuddle project for use with this module.

If you install browscap module (http://drupal.org/project/browscap) the tickets created will include the browser info for the user giving the feedback.
