-- SUMMARY --

This module provides the ability to create and manage group discussions in the
Drupal UI. Discussions can be participated in via Drupal or email.

-- FEATURES --
@todo

Integrations with other modules:
  * Flag - Users can "follow" discussions, prioritizing them in displays, if
the Flag module is enabled. (https://www.drupal.org/project/flag)

-- REQUIREMENTS --

* Features
* Mandrill
* Organic Groups

Recommended: Patch to organic groups 7.x-2.7 to display valid configuration on
node type edit pages: https://www.drupal.org/node/1674078#comment-6460240

-- INSTALLATION --
@todo
1. Ensure Mandrill module is enabled and configured for your Mandrill account.
    If you're having trouble configuring Mandrill with your API Key make sure
    you have the Mandrill PHP Library available. See the Mandrill project page
    for more details: https://www.drupal.org/project/mandrill.
2. Enable 'Mandrill Groups' module.
3. Go to 'admin/reports/status' and click on the 'Rebuild permissions' link.
4. Go to 'admin/config/services/mandrill/groups' and add an email domain. (in.example.com)
5. Go to https://mandrillapp.com/inbound and add an Inbound Domain. (in.example.com)
6. Add a route for the new domain:
  - Route: *@in.example.com
  - Webhook: http://example.com/mandrill/webhook/inbound

You can manage your Mandrill webhooks here: https://www.mandrillapp.com/settings/webhooks/
