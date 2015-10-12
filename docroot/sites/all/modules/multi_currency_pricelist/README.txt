Multi-currency pricelist
Author: Martin Postma https://drupal.org/user/210402

Pricelist with a master and slave currencies. Updates itself to current Yahoo!
exchange rates. Optionally sample content can be turned on/off.

This is a feature module. It provides a setup of an automatically updating
multicurrency pricelist that can be easily adapted for your use case. You'll
find:
- a content type 'pricelist item'
- a View with the actual pricelist with a Tab for each currency
- some admin links at the bottom of the pricelist to put items in categories and
order them arbitrary.


*** Installation ***
Drush users:

Download: drush dl features features_extra nodequeue currency
currency_yahoo_finance money views views_php ctools

Enable: drush en features fe_nodequeue smartqueue currency
currency_yahoo_finance money views views_php ctools php views_ui

All other dependencies will be enabled automatically after confirmation.

Others download and enable manually (see list at the bottom).

Going into the still empty pricelist you'll find instructions how to proceed.

It is recommended to enable the provided submodule 'Multi-currency pricelist
demo content' and 'play' with it. On uninstall all demo content will be removed,
except for terms and nodes you have overriden with your own title. It is a good
starting point.


*** FAQ ***
<< After creating a pricelist item it does not show in the pricelist. >>

You should see a tab 'Nodequeue' besides the usual tabs 'View' and 'Edit'.
Assign a nodequeue there after creating a new item. If you don't see the
'Nodequeue' tab, assign a new or existing taxonomy term 'Pricelist section'
first.

<< Where to customize currencies for my use case? >>

/admin/structure/types/manage/pricelist-item/fields/field_price to select which
currencies should be available for your use case.

/admin/structure/views/view/pricelist/edit : To change the pricelist itself. You
have to go through all settings for each currency that contain currency codes
and change them for your use case, specifically:
- Display name
- FIELDS: Global PHP (Price)
- PAGE SETTINGS: Path and Menu
- HEADER: Global: Text area

<< Can I automatically add pricelist items to the pricelist on creation or after
assigning them to a pricelist section? >>

Yes, but it not included in this feature module. See
https://drupal.org/comment/7928329#comment-7928329 or
https://drupal.org/comment/7302256#comment-7302256. To make your life easier
install on long lists of items install the Administration Views module and add
the Bulk operation 'Add to nodequeues'.

<< What currency to use when creating a new pricelist item? >>

The currency you specify here, is the base currency that will not change based
on exchange conversion. Slave currencies are calculated multiplying the exchange
rate with the base currency.

<< How can I add a percentage to the converted price? >>

In a 'slave' currency View 'Pricelist' go to FIELDS: Global: PHP (Price) and
change the following line to add e.g. 2,5%:
$ret = number_format(round(bcmul($exchange_rate<<  * 1.025 >>, $amt, 2), 1), 2);

Then in 'HEADER: Global: Text area' add something like the following sentence:
'On payments with this currency a 2,5% commission applies, as already included
in the displayed prices.'

<< An item with price = 0 is displayed with the price 'on request'. >>

This is normal behaviour to avoid your business goes bankrupt if you forget to
fill in a price.

<< How often are the exchange rates updated? >>

Go to /admin/config/regional/currency-exchange to set how often exchange rates
should be retrieved.

<< How to remove the demo content? >>
Just disable << and uninstall >> the submodule 'multi_currency_pricelist_demo'.

<< I have special needs. Can I get assistance? >>
Feel free to contact the maintainer (https://drupal.org/user/210402/contact) to
set it up for your use case. For generic questions, open an issue tagged
'support request'. Try to solve it yourself first, comparing your settings with
those in the demo.


*** Dependencies ***

Project features (7.x-2.0)
Project features_extra (7.x-1.0-beta1)
Project nodequeue (7.x-2.0-beta1)
Project currency (7.x-2.1)
Project currency_yahoo_finance (7.x-1.1)
Project money (7.x-1.x-dev)
Project views (7.x-3.7)
Project views_php (7.x-1.x-dev)
Project ctools (7.x-1.3) needed for views

To enable.If prompted to enable dependencies, answer 'yes'.
features
fe_nodequeue (part of features_extra)
smartqueue (part of nodequeue)
currency
currency_yahoo_finance
money
views
views_php
ctools (needed for views)
php (part of core: PHP filter)
views_ui (part of views)
