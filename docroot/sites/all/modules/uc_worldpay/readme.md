# UC WorldPay

## Introduction

This module aims to integrate WorldPay's payment services with Ubercart.

The module currently integrates the redirection payment service.

## Installation

Follow the usual module installation procedure:

http://drupal.org/documentation/install/modules-themes/modules-7

## Settings

The settings for the UC WorldPay module are located at:

Administer -> Store administration -> Configuration -> Payment settings

Click on the "Payment methods" section and then expand the "WorldPay
Settings" fieldset to view the settings.

### Securing your connection to WorldPay

There are several options in part "Payment parameters" to add extra security.

__MD5 secret word__

This adds a signature to the most important payment parameters like price. You need to setup
the same value in your merchant interface. The signature is checked by WorldPay.

__Payment response password__

You can setup a payment response password in your merchant interface. WorldPay will then send
this password whenever a client is routed back to the store after payment. The uc_worldpay
module can use this to verify that the response is really from WorldPay.

### Advanced options

The following options do not need to be changed for most installations.

__Callback URL__

By default, you will setup an explicit payment response URL in your merchant interface. There are cases
(e.g. you need to make payments from multiple servers) where this must be dynamic. In this case this
module can send the payment response URL to WorldPay with each payment.  
You must enter "&lt;wpdisplay item=MC_callback&gt;" in the payment response URL field in your merchant
interface for this to work.

__Cart contents as descriptions__

This should be activated so that customers see what they pay for.

__Contact details__

This controls if contact details are shown and can be edited (on WorldPay page) for the credit card payment.

__Payment page language__

Sets the language of the WorldPay pages.

__Using multiple WorldPay configurations__

This module supports the usage of multiple WorldPay merchant accounts. By default only one
account profile is created (named "Default"). To create more accounts select
"WorldPay configuration" in Ubercart's store configuration.



## Testing

You can log in to the WorldPay Test Merchant interface at http://www.worldpay.com/support/bg/index.php?page=newlogin&c=WW

You can use the following credit card numbers to test the payment service:  
  
VISA: 4911830000000 and 4917610000000000  
AMEX: 370000200000000  
DINERS: 36700102000000  
JCB: 3528000700000000  
LASER: 6304900017740292441  
MAESTRO: 6759649826438453 (GB only)  
  
(Source: http://www.worldpay.com/deutsch/support/kb/bg/testandgolive/tgl.html)
