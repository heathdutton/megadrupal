Integrates Balanced Payments with Drupal Commerce.

Balanced Payments allows you to not only debit credit cards but bank accounts as well. 
Since the bank accounts need to be verified before they can be debited I have setup a 
system where a customer can checkout with a new bank account and when Balanced Payments 
has deposited the two small deposits needed to verify the account they can come back to 
your site and verify their bank account. Once they verify their account all pending 
transactions relating to that bank account are immediately processed.

After the user checks out with a new bank account that account is saved to file (account 
info is not saved locally). It acts very similary to the Commerce Card On File module, 
in fact that module can be used with the credit card side of this Balanced Payments module. 
I needed a custom way to save these bank accounts securely so that is built into this module. 
Users can manage their bank accounts through this module as well as verify and delete them.
You will need to download the balanced-php library and extract it into your sites/all/libraries 
folder. The folder should be named balanced-php-master.

https://github.com/balanced/balanced-php

I left it largely unstyled to leave as much room for customization as possible. 
In future releases I may spice it up.

Users can manage their bank accounts at /user/{uid}/bank-accounts.