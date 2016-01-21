BluePay Level 3 Transaction Support
-----------------------------------

This module extends the existing transactions to add Level 3 Data. Using Level
3 transaction data can help you to better serve certain institutions and earn
better rates, especially on large transactions.

* Read more information on how [Level 3 Transactions can benefit you][l3transactions].
* Read relevant [developer documentation][dev-docs].
* [Sign up with BluePay][sign-up].


Technical Details
-----------------

**This module extends the base BluePay module and does not work with hosted 
forms.**

By using the hooks provided in the main BluePay module, this module adds the
necessary data to the request. It currently sends everything but the tax rate.
Review the API documentation for this module to see how you can extend this
module to suit your own business needs.


  [l3transactions]: https://www.bluepay.com/payment-processing/gateway/level-3/
  [dev-docs]: https://www.bluepay.com/sites/default/files/documentation/BluePay_bp20post/Bluepay20post.txt
  [sign-up]: https://www.bluepay.com/contact-us/get-started/

