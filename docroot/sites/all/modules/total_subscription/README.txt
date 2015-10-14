README
======

Total Subscription provides functionality that allows the users to subscribe to
node pages, taxonomy terms, etc. The main feature which distinguished it from
other subscription modules is that it allows subscription for Anonymous along
with Authenticated users.

Total Subscription provides users the ability to:
1. Subscribe to a specific content type through the node page.
2. Subscribe to the related taxonomy terms associated with the node on the
node page.
3. Subscribe to a specific taxonomy term from the term page.
4. Unsubscribe from any previous subscription.
5. Add a separate ctools content type - "subscription", which could be
integrated with any panel page.
6. Add a "subscription" block to the any region.

Dependencies:
1. Views
2. Token

Email Trigger:
Email will be triggered thrice in the process -
1. During anonymous Subscription.
2. After Content Published.
3. During Unsubscription.
You can play around with hook_mail_alter along with t function for inserting
dynamic link.

Future Plans
1. Expecting feature requests from community.
2. Porting this module to Drupal 8.

Extra Feature
1. Bitly integration : If you wish to send the verification link in
compressed form (eg. http://bit.ly/1icB6B3), then you can go to the link
and create the account and provide the username and bitly api key.

2. HTML Mail : If you wish to send html mail then just add the key Confirmation
to the html mail configuration settings.
  - The key for verification mail for anonymous user subscription is
  	total_subscription_message.
  - The key for sending mail while content publication is
  	total_subscription_node_message.
3. Views Integration is also available. All data are exposed in views.
