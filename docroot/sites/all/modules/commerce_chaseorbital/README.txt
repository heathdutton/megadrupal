CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Registering with Chase Orbital
 * Development notes
 * Sponsors


INTRODUCTION
------------

This module provides a Drupal Commerce credit card payment gateway method
for Chase Orbital API (Chase Paymentech CIM method) to purchase products
and also supports Card on File and Recurring payments.

Chase Paymentech is a leading credit card processing company.
It provides Orbital® Payment Gateway API to support online payment processing.

Chase Paymentech Site URL:
http://www.chasepaymentech.com/

Orbital® Payment Gateway enables online payment processing for the
most basic store fronts and also for highly integrated e-commerce systems.


REQUIREMENTS
------------
This module requires the following modules:
 * Drupal Commerce (https://www.drupal.org/project/commerce)
   and its dependencies. 
 * commerce, commerce_ui, commerce_payment, commerce_payment_ui, commerce_order

You must have a Virtual Terminal account with Chase Orbital
in order to use this module.
Please refer to Registering with Chase Orbital section on this.
 

RECOMMENDED MODULES
-------------------
If you need the features like Card on File and Recurring Payment support,
Please install the below recommended modules.

 * Commerce Card on File (Version 2 - 7.x-2.x):
   https://www.drupal.org/project/commerce_cardonfile
   When enabled, it will allow to store card authenticate information.

 * Commerce Recurring Framework (Version 2 - 7.x-2.x):
   https://www.drupal.org/project/commerce_recurring
   When enabled, it will allow to process recurring payments/orders.
 
 
INSTALLATION
------------

 * Install as usual, see http://drupal.org/node/895232 for further information.
 
 * Install this module and all its required dependencies as habitual.


CONFIGURATION
-------------

 * Configure the payment gateway settings in
   admin/commerce/config/payment-methods/manage/commerce_payment_chase_orbital
   by clicking the "edit" option in
   Enable payment method: Chase Paymentech Orbital - Credit Card
   under "Actions".

 * Please make sure that you have enabled "Payment UI" to access the
   above mentioned Payment Gateway method settings page.
   
 * Please enter the below details in the Payment Gateway method settings page.
 
   - Merchant ID

     Your Merchant ID is different from the Chase Orbital Gateway User Name.
  	 You need to login to your Chase Orbital account to obtain it.
	 Once you login, browse to your Account tab to find the Merchant ID.
	 If you are using a new Chase Orbital account,
	 you may still need to generate a Merchant ID.

   - Username

     Please provide your Chase Orbital Gateway User Name.

   - Password

     Please provide your Chase Orbital Gateway Password.

   - Terminal ID

     Please provide your Chase Orbital Terminal ID.

   - BIN No

     Please provide your Chase Orbital Gateway BIN.

   - Cards Types

     Please select the list of card types you want to support.

   - Card on File Check Box

     Please enable this Card on File check box, If you want to let customers
  	 opt-in to storing their card authenticate information on file for faster
	 checkout by using Chase Paymentech CIM (Customer Information Management).
   

REGISTERING WITH CHASE ORBITAL
------------------------------

 * You must have a Virtual Terminal account with Chase Orbital
   in order to use this module, as it depends on API credentials
   specific to their services.

 * If you need to register for an account, visit the page:
   https://www.chasepaymentech.com/virtual_terminal.html
   
 * Once you submit the Virtual Terminal application in the above link,
   you will receive an email which contains instructions for the
   document sign and business checking account confirmation process to proceed
   to the next step to obtain the Chase Orbital Virtual Terminal account.

 * Once you obtained the Virtual Terminal account, you will receive
   a Test Certificate from Chase Orbital where you can find the
   API details and Test Scenarios to be covered.
   
 * Once the site is ready to go live, then you need to submit the
   Test Certificate to Chase Orbital and obtain Live Certificate from them.


DEVELOPMENT NOTES
-----------------

 * This is the first Drupal Commerce Chase Orbital gateway module
   to contribute.

 * If you would like to contribute to this module by extending
   the CIM implementations or supporting additional 
   Chase Orbital payment methods(HPP, HPF, etc),
   feel free to post patches to the queue.

 * Chase Orbital developer guides can be found at:
   http://www.chasepaymentech.com/payment_gateway.html.


SPONSORS
--------

This project has been sponsored by:
 * DRUPAL GEEKS
   Specialized in consulting and planning of Drupal powered sites,
   development, theming, customization, and hosting to get you started.
   Visit http://www.drupalgeeks.com/ for more information.
