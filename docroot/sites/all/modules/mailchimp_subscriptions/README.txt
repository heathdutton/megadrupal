Overview
=====================

This module allows anonymous visitors to subscribe to new content on your site of specific authors. The notifications are email sent by MailChimp and you can configure which users (by role) and which content-types apply. You can also customize the mail body using tokens available for the specific nodes.

Please note that this is still a sandbox project and may not be production ready.

Features
=====================

This module provides a block with a simple form which should be placed on a full node. By submitting his name and email on this page, the user will automatically be added to a MailChimp list and group. Each author which can be subscribed to this way will be automatically created as a MailChimp group in one specific list.

In the current version, an email (campaign) is immediately created and sent when a node is published. The publication_date module is used to assure that a mailing for a newly published node is sent only once.

People can also easily manage their "subscriptions" through the MailChimp interface. No database tables are created with this module, and their are no permissions associated with this module. 

Why do it this way? Because when anonymous visitors subscribe to new content via email, you should handle that like newsletter subscription. This means you need to account for double opt-in and easily allow people to unsubscribe. There are no Drupal modules which do a good job in all this, and you should use specialized tools like MailChimp to take care of the heavy lifting.

Requirements
=====================

- a MailChimp account (http://www.mailchimp.com)
- mailchimp (http://drupal.org/project/mailchimp)
- publication_data (http://drupal.org/project/publication_data)
- token (http://drupal.org/project/token)

The main MailChimp module is only used for the API keys and PHP API Wrapper 1.3 class library. 

Install & Test
=====================

1. Enable the module and make sure you have a valid MailChimp API key submitted in the main mailchimp module config screen. 
2. Then, create a new list in your MailChimp account and copy the list-ID. 
3. Enter this ID and all other required field in the modules' config screen at admin/config/services/mailchimp/subscriptions.
4. After submitting, check that the groups are actualy created in the list in your MailChimp account.
5. Place the block provided by this module (called "Mailchimp subscribe to user") on a full node of one of the content-types you chose.
6. Visit the node. If the author and content-type match, you should see a form.
7. Enter your name and email
8. A confirmation email is sent by MailChimp. Confirm.
9. Create a new node and publish it, where the author and type match the one you subscribed to in step 6.
10. You should see a status message which says how many emails will be sent by MailChimp.
11. Check your mail! 
12. Check MailChimp to confirm that the campaign is created and sent!
13. Iep! Eat a banana to celebrate!

Be aware that a MailChimp group is created in the list for **each** user with the role you check. This is not necessarily a bad thing, but might provide the user with a very long list of checkboxes in their "manage preferences" screen on MailChimp.


Support
=====================

Please use the issue queue for any requests or bug reports.

Related projects
=====================

I'm not aware of any modules which provide this functionality for **anonymous** visitors. There are however these modules who can provide somewhat similar functionality. 

There is a comparison of notification module in below link, but they are sometimes for comments only and often only for authenticated users.

https://groups.drupal.org/node/15928

Wishlist
=====================

If you agree with one of these wishes below, please post them as a feature request in the issue queue!

- no need to try creating the groupings and folder eacht time -> split settings in subtab.
- create grouping when a new user is created with chosen role
- make name optional or configurable
- better validation of block placement
- make block also work on user/profile pages
- delete a campaign after X days (Mailchimp has a limit og 32.000 campaigns)
- maybe make mailchimp module optional
- allow for aggregation of new nodes in time before sending
- support for MailChimp templates

Credits
=====================

This module is written and maintained by Albert Skibinski (@askibinski) and sponsored by Merge.
https://drupal.org/user/248999
http://www.merge.nl

