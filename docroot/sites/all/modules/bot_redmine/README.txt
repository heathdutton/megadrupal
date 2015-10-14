This module is a plugin for the Bot module, https://www.drupal.org/project/bot,
allowing your bot to lookup Redmine issues.

To communicate with Redmine, this module relies on the Redmine REST API module,
https://www.drupal.org/project/redmine_rest_api, for the integration.

To get up and running, you need a working Bot install, and the Redmine REST API
needs to be configured to talk to your Redmine server.

With that done, you can enable this module, go to /admin/config/redmine/redmine,
and tick the 'Enable Redmine lookups from the IRC bot' checkbox. You'll need to
restart your bot for the change to be picked up.

With the module up and running, your bot will respond to any mention of Redmine
issue numbers in the form of r1234 or #1234 in IRC messages and reply with:

#1234 [Project] Issue Title - URL