Ubercart NAB Transact Hosted Payment Page
===============================================================================

An Ubercart payment method that accepts secure payment via an NAB Transact
Hosted Payment Page which can include your business branding. You will require
an appropriate merchant account with the NAB to accept payments with this
module.

http://www.nab.com.au/wps/wcm/connect/nab/nab/home/business/4/4/5/

When the client submits the Review Order page on your site, the finalised
order is posted to the NAB Transact Hosted Payment Page, which accepts the
client's financial details.

This module does not accept nor store the Credit Card details. This may keep
your website out of scope of Payment Card Industry Data Security Standards
(PCI DSS) as the card data is passed directly from the customer's browser to
the payment gateway. Please confirm this yourself or with an appropriately
qualified professional.

Details of the order, such as the list of products and thus final total, can
be altered by a mischievous user so all payments and the order should be
confirmed manually before shipping.

Access to the reply link URL is restricted by IP address; the status and
stability of the servers accessing this URL is unknown, and the protocol does
not contain any authentication or verification methods. Please inform the
author of any changes to the list of IP addresses. If the list of valid IP
addresses is cleared, then any user with "access content" permissions may fake
the reply link URL.

Your server must be live to the internet, and running on port 80 for the
reply_link_url to function and advance the order status.

For testing, configure the payment method to use the Test payment page Payment
Server, with either the valid Vendor Name or a test Vendor Name of NAB0010 and
the following CC details:
Typ: VISA
CC#: 4444333322221111
CVV: 123
EXP: Any date greater than today.

To simulate a successful transaction, the order total must be any sum ending
in 00, 08, 11, or 16 cents. All other values will simulate a declined
transaction.

For more information, please refer to the HostedPaymentPageIntegrationGuide.pdf
supplied to you by the NAB.


This module is based on the code in uc_nabhpp-5.x-1.0-beta1.tar.gz available
from the UC bounties forum.
http://www.ubercart.org/forum/bounties/4327/payment_gateway_ubercart#comment-22826

Adaption to UC3 D7 and improvements by:
Ted Cooper - http://drupal.org/user/784944
