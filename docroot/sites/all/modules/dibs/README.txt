
DESCRIPTION
=====================
                
DIBS is the biggest payment gateway provider in Scandinavia.

This module makes it possible for other modules to integrate and 
receive payments through DIBS Internet, see 
http://www.dibspayment.com/products/dibs-internet/. 

The module it self does not "out-of-box" make it possible to receive
payments, but it offers an API so other modules easily can integrate
to DIBS via the PaymentWindow or FlexWindow. 

The DIBS API is not supported, and support is not planned.


INSTALLATION
=====================

1. Install both the DIBS and the DIBS Example modules. If you already
   has it installed, then please reinstall it!

2. Go to admin/dibs/settings. It is here possible to configure the 
   module for each hook_dibsapi implementation, actually in delta 
   level. If some configuration is missing on an implementation, will
   it fall back to the default implementation - the one on the DIBS 
   module (admin/dibs/settings/edit/dibs/default).

3. Go to admin/dibs/settings/edit/dibs/default and configure the 
   different settings.


TESTING THE MODULE
=====================
    
1. Go to payment/dibs/example/form which is the test form the 
   dibs_demo module provides.

2. Fill in a test amount and click next.

3. You are now redirected to the gateway

4. Use one of the test credit card numbers from 
   http://tech.dibs.dk/10-step-guide/10-step-guide/5-your-own-test/.

5. When you have paid should you be redirected back to 
   payment/dibs/accept/XXXXXXXXX and the hidden callback should have 
   updated the fields: payment_status, payment_transaction_id and 
   payment_type in the dibs_transactions table.
   

FUTURE DEVELOPMENT
=====================
    
1. Listing of transactions in administration including view of each 
   transaction.

2. Capture payments from the transactions view in the administration.

3. Ad better administration of order numbers. Right know is it quite 
   easy to mess them up.

4. Support for Ubercart (Seperate module).

5. Support for E-commerce (Seperate module).


DRUPAL 5
=====================
No support planned.


CONTRIBUTORS
=====================
Jens Beltofte Sørensen (beltofte@gmail.com)


SPONSORS
=====================
Propeople ApS (www.propeople.dk)
FDM A/S (www.fdm.dk)