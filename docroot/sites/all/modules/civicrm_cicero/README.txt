CiviCRM Cicero allows you to add election district information to your
CiviCRM contacts using Azavea's commercial Cicero API. You can map
fields provided by Cicero to your own custom data fields in CiviCRM and
limit contacts to update by CiviCRM groups. The information is updated
during cron runs, which can be scheduled by the you.

You have to start by getting an account with Azavea[0]. In order to use
their service, you have to pay for it. However, they offer a 1,000
credit free trial and, if you are a nonprofit organization, you can get
5,000 credits for free via TechSoup[1].

Once you have an account, you'll need to add a few Contact custom data
fields to your CiviCRM database. Depending on your needs, you might
create fields to hold a contact's city council district, national
representative district, school district or any other similar
information. Be sure to also create a "last updated by cicero" field
which will be auto-filled by the module.

Now it's time to configure the module. Using the CiviCRM menu system,
Click Administer -> System Settings -> Administer CiviCRM Cicero. Or,
you can find the same place via the Drupal Administer -> Configuration
menu.

Click the Cicero Account Settings tab to enter your Cicero API username
and login. In addition, on this page, you create your field mappings.
These mappings tell the module which custom data fields should hold your
Cicero data.

Once your fields are mapped, click the "Update Civi Contacts with Cicero
information" tab and choose the CiviCRM group that you would like
updated and the date/time you would like it to happen.

The actual sync'ing happens via the Drupal cron job to avoid http time
outs.

The module comes with two handy drush commands that are useful for
debugging.

  drush civicrm-cicero-show-contact --contact_id=N

Shows all available cicero data for the contact with contact_id N,
regardless of your mapping.

  drush civicrm-cicero-update-contact --contact_id=N

Updates the contact with contact_id N according to your saved mapping.

Limitations: nothing is perfect. Cicero only appears to cover the United
States. Also, if you are not using CiviCRM with Drupal 7 this won't work
(although we have done the heavy lifting, so re-implementing for
WordPress and Joomla should not be terribly difficult).

0. http://www.azavea.com/products/cicero/ 
1. http://home.techsoup.org/Stock/Pages/Product.aspx?cat=TechSoup%20Global%20Catalog&category=&id=G-46237
