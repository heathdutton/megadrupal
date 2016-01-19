-----------
Subs module
-----------
by Alex Weber

Short for "Subscriptions", this module enables "site subscriptions".

For more up-do-date and complete documentation check out the official
documentation pages in the handbook: http://drupal.org/node/1541248

--------
Overview
--------

This module aims at allowing users to subscribe to a site (or service). A subscription can have different types (bronze, silver, gold, etc) and can have different statuses (see below) .

Each subscription type can define an optional length for subscriptions and also a grace period after subscriptions expire.

This is not for subscribing to changes to content or taxonomies, check out Subscriptions or Notifications for that.

Subs implements a more generic type of subcription to an entire site or section, for example:

  * 1 month subscription to a video-tutorial website
  * 1 year gold subscription to hosting plan
  * 3 week premium subscription to a forum

----------------------
Implementation details
----------------------

  * Creates a "Subscription" entity
  * Enables exportable and fieldable "Subscription Type" bundles
  * Integrates with Devel, Drush, Features, Rules and Views
  * Provides add/edit/delete forms for Subscriptions
  * Allows viewing Subscriptions
  * Expires subscriptions automatically when they reach their end date

Subscriptions have the following default properties:

  * Start Date
  * End Date (optional)
  * Status (pending, active, grace period, expired, cancelled)
  * User ID (uid)

Subscription types have the following optional default properties:

  * Default Status
  * Subscription Length (optional)
  * Grace-period length (optional)


----
More
----

Subscriptions are smart; for example, if a user is deleted, his subscriptions are cancelled.

This module also offers concise and well-documented API functions to deal with all aspects of subscriptions.