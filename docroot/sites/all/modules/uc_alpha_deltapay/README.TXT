Adds a payment method to Ubercart to accept credit card payments through Alpha
DeltaPay (Greek bank).

The original code for Drupal 6.x and Ubercart 2.x was originally created from
GiorkosK and may be found at http://www.ubercart.org/project/uc_alpha_deltapay.
The module for Drupal 7.x and Ubercart 3.x though is a complete rewrite of the
code.

Implementation follows the Alpha e-Pay v2 Merchant Integration Specifications
(HTML - v1.12).

Payment with instalments is also implemented as long as the Bank has approved
that feature for the vendor's account.

-- INSTALLATION --
Install as usual by placing the module folder at sites\all\modules.
Then navigate to admin/modules and enable the module.

-- CONFIGURATION--
Go to admin/store/settings/payment and enable the Alpha Bank - Delta Pay method.
After enabling, click settings or navigate to
admin/store/settings/payment/method/alphadeltapay to configure the module.
From the configuration page, you must fill-in the Vendor account number and the
maximum Number of instalments(if needed). You may also choose to display some
extra messages alongside the default ones upon success, failure or cancellation
of the order.

-- KNOWN ISSUES--
Most, if not all, Greek banks, authorize payments from only one URL. As a
result, if we have a multilingual website, this payment method may be used only
by one language and the administrator would have to set a proper condition to
hide it in other languages. To most properly understand this, read the following
examples:
(Greek - URL #1)
http://www.domain.com/el/cart/checkout/review
(English - URL #2)
http://www.domain.com/en/cart/checkout/review
In case anyone has anything to suggest upon this, feel free to fill an issue.

-- URLs to give to the bank --
GetGuids page: http://www.domain.com/cart/checkout/review
Payment page : http://www.domain.com/cart/checkout/review
Success page : http://www.domain.com/cart/alphadeltapay/complete
Failure page : http://www.domain.com/cart/alphadeltapay/complete
Cancel page  : http://www.domain.com/cart/alphadeltapay/complete

-- Demo Version --
In case you need it, the module also includes a demo page to act as the bank
and return some variables to the live system. Make sure you give the permission
only to trusted users though.

-- Sponsoring --
The original development was sponsored by Avalon Web and Media.
