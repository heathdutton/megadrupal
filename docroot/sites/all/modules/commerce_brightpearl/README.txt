Commerce Brightpearl

In this file:
- Install instructions
- Configuration instructions
- un-install notes.


Install
==============================================================

The install includes two parts

1. Install the Drupal module through the Brightpearl app store - this will
   generate a signed Key.
2. On your Drupal commerce site Install the Brightpearl module - you will need
   the signed key.

Install the Drupal module through the Brightpearl app store
--------------------------------------------------------------
1. Log into your Brightpearl account
2. Go to the app store and add the "Drupal Commerce" app.
3. the app install will send you to a registration page where you will need to
   provide details
4. You will receive a signed key emailed to you, normally within one working
   day (this is not automated yet).


On your Drupal commerce site install the Brightpearl module
--------------------------------------------------------------
1. Download the Commerce Brightpearl module
  (drupal.org/project/commerce_brightpearl)
   and place in your contributed modules folder
2. Go to the modules page of your site and enable the
   Commerce Brightpearl module

* You should also install commerce stock to get the most out of the module
* If you don't need stock control than steps 3 & 4 can be skipped

3. Download Commerce stock (drupal.org/project/commerce_stock)
   and place in your contributed modules folder.
4. Go to the modules page of your site and enable
   Commerce Stock, Commerce Stock API,
   Commerce Simple Stock & Commerce Simple Stock Rules modules.


Configure
==============================================================
Before you configure the module it is worth noting that you should make sure
that your products have the same SKUs in both Drupal Commerce and the Brightpearl.

Configuration is divided into a number of tabs:
- Brightpearl Overview - Not fully implemented yet.
- Brightpearl Connection - Establish a connection to Brightpearl
- Brightpearl Settings - Configure Brightpearl settings like the channel to use.
- Brightpearl Orders - Order integration.
- Brightpearl Stock - Stock integration.
- Brightpearl Admin & Alerts - Provide an email for Alerts


Brightpearl Connection:
-----------------------
You will need three pieces of information:

* Your Account Code (from Brightpearl)
* The Brightpearl Datacentre (from Brightpearl)
* Token - you will receive a signed token by email after registration
After saving you should see an alert: "You are connected to Brightpearl .."

Brightpearl Settings:
---------------------
You will need to select the following from the dropdown:

* Tax code - The Brightpearl tax code to use.
* Channel - The Brightpearl channel to use.
* Warehouse - The Brightpearl warehouse to use.
* Check all available stock - options for "Selected Warehouse" or "All on Hand
  stock".
* Order Status - The Brightpearl Status for new orders created by the Drupal
  site.
* Bank account nominal code - The Brightpearl Bank account nominal code to use.
* Shipping nominal code - The Brightpearl shipping nominal code to use.

Brightpearl Orders:
-------------------
This will configure the order integration. Once enabled, orders will be processed
by cron.

* Completed order state - The state the Drupal order needs to get to in order to
  be exported to Brightpearl (default to Pending)
* Order integration state - Active/Suspended
* Integration state - Check the "Enable order integration" and submit. This will
  add a field to the Commerce Order.

Notes:
Once the integration has been enabled, newly created orders will be imported into
Brightpearl when they get to the specified state.


Brightpearl Stock:
-------------------
Before enabling the Brightpearl stock integration you should enable stock for
all your products so they can get updated by Brightpearl (see commerce stock for
details).

* Update Type:
    No updates - Stock updates are disabled
    Real time using a webhook - Recommended settings. Stock will be updated in
    close to real time.
    Full update on cron run - not recommended if large number of products.
* Batch size:
    Number of products to update on each cron run.
* Initialise stock level of all products:
    Click this button if you have chosen webhook. This will run a one time
    update of all your products using cron. Once updated changes to stock will
    occur each time a change is made on Brightpearl.



Uninstall notes
===============
The uninstall does not remove the field added to the order to prevent data loss.
you can use the Brightpearl Stock screen to remove the field before
un-installing.
