UC GoPay

GoPay payment gateway integration for Übercart.


REQUIREMENTS
---
Drupal 7.x [1] with Übercart 3.x [2]
PHP extensions: mcrypt [3], soap [4]

[1] http://drupal.org/project/drupal
[2] http://drupal.org/project/ubercart
[3] http://php.net/manual/en/book.mcrypt.php
[4] http://www.php.net/manual/en/book.soap.php


INSTALLATION
---
1. Register your e-shop at http://gopay.cz and wait until you obtain GoId,
   shared secret key and testing gateway access.
2. Download and extract this module to your modules directory.
3. Enable this module (requires uc_payment module from the Übercart pack).
4. Go to Administration -> Store ->  Configuration -> Payment methods ->
   GoPay Website Payments (admin/store/settings/payment/gopay).
5. Fill the GoId, Secret and enable the Payment channels you want to use.
6. OPTIONAL STEP: Fill the optional module registration. Thank you!
7. Set the GoPay server to Testing mode and try all payment
   methods to verify they are working as expected.
8. If the payment process works perfectly, set GoPay server to Production and
   go Live!


CHANGELOG
---
2011-12-07 7.x-1.0-alpha1
 - Initial version


CREDITS
---
Author: Vojtěch Kusý - http://vojtechkusy.cz
Developed for: Štěpán Korčák - http://stepankorcak.cz
