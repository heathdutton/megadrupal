
Ubercart Crowdfunding Feature module.

By Arlina E. Rhoton ("Arlina", http://drupal.org/user/1055344)


Description
===========
Defines an Ubercart product feature to turn any product into a
"crowdfunding style" donation product, in which users may donate
towards a goal before a dateline is reached.
Two options are available:
 1 - Disable donations if current date is after dateline
     OR if target amount is reached.
 2 - Disable donations if current date is after dateline
     (allows donating past target amount).
Defaults can be set at global or node type level, but can also
be added or edited for every product node.

Tips
====
Plays well with the following modules:
* Product attributes: allow customers to select different amounts to donate.
* Variable price: allows customers to enter their own amount to donate.

Installation
============
1. Enable module as usual.
2. Configure global default dateline:
   admin/store/settings/products/edit/features
3. Configure the defaults per node type (product class) in:
   admin/store/products/classes
   This will be used as default for all new nodes of that type.
4. Additionally, you can add or edit the crowdfunding feature for any product
   node through the Features tab on its Edit form.

Theming
=======
The following variables are made available on the $node object for 
easy theming:

* $node->uc_crowdfunding_sold (int)
The amount of items that this node has sold.
 
* $node->uc_crowdfunding_gross (float)
The gross income this node has generated.

* $node->uc_crowdfunding_revenue (float)
The revenue this node has generated.

* $node->uc_crowdfunding_target (float)
Target amount.

* $node->uc_crowdfunding_dateline (int)
Dateline as unix timestamp.
    
* $node->uc_crowdfunding_type (int)
Possible values:
    1 - Disable donations if current date is after dateline
        OR if target amount is reached.
    2 - Disable donations if current date is after dateline
        (allows donating past target amount).

* $node->uc_crowdfunding_enabled (bool)
Whether the node is available for sale.
 
* $node->uc_crowdfunding_days_remaining (int)
Remaining days until the closing deadline rounded to the largest integer.

* $node->uc_crowdfunding_goal_achieved (bool)
True if goal has been achieved.

Hooks
=====
* hook_uc_cf_product_report($nid, &$report)
This hook is used to alter the crowdfunding properties (sold, gross, income)
added to the node object.
Please see file uc_crowdfunding.api.php for details.
