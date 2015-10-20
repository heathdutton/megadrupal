
-- SUMMARY --

This module allows payments through the X-Pay - Carta Sì
Virtual pos (http://www.cartasi.it/gtwpages/common/index.jsp?id=OiRGdkfJWU)  
in Drupal Commerce
http://drupal.org/project/commerce

-- REQUIREMENTS --

Commerce: http://drupal.org/project/commerce


-- INSTALLATION --

* Install as usual, 
  see http://drupal.org/documentation/install/modules-themes/modules-7 
  for further information.

* Go to Store -> Conifgure -> Payment methods and press "enable" on the disabled 
  "X-Pay - Carta Sì" to enable the payment method

* To modify configuration options go to Store -> Conifgure -> Payment methods
  and press edit on "X-Pay - Carta Sì" row. Scroll down to "Actions" and
  press "edit"

-- CONFIGURATION OPTIONS --

Terminal ID 
The terminal id that you  got from Carta Sì. 
Leave the default (T04_000000000005) for testing purpose

Mac Key 
The Mac Key that you got from Carta Sì. 
Leave the default (AA88CCEWDKLSDJD3921ZZ) for testing purpose

Mode
Test for testing, normal for a Live site

Action Code 
You should leave that on VERI (At least the documents say so)

Language preference
The language used on X_Pay pages

Currency preference
The currency to use with X-Pay

Payment method title 
The text that appears in the payment page

Show credit card icons beside the payment method title. 
Thick the checkbox if you want to see the icons in the payment page

Order review submit button text 
The text on the submit button
