DESCRIPTION
===========
KBC Paypage (https://www.kbc.be/PBL/CC028/Businesses/Running_your_business/Payments/Receiving_online_payments/Paypage) integration for the Drupal Commerce payment and checkout system.

INSTALLATION INSTRUCTIONS FOR KBC PAYPAGE
============================================

First you should make sure you have an KBC Paypage account, and are ready to configure it:

Configuring KBC Paypage
------------------
- Log in with your KBC Paypage account to the test or prod environment
  (depending on which you'd like to configure)
- Go to Configuration > Technical information
- Make sure the following settings are properly configured for this module to work:
  TAB "Global security parameters":
  * Hash algorithm: SHA-1
  * Character encoding: UTF-8

  TAB "Data and origin verification"
  * URL of the merchant page containing the payment form that will call the page:orderstandard.asp
    --> make sure to enter your correct domain here
  * SHA-IN Pass phrase (twice!)
    --> Enter a pass phrase here for security purposes, and remember it for later use
  * IP address of your server (for DirectLink - if appropriate)

  TAB "Transaction feedback"
  * No need to fill in Accepturl, Declineurl, Exceptionurl or Cancelurl. They will be handled automatically
  * Check the checkbox "I want to receive transaction feedback parameters on the redirection URLs.",
    unless you intend to use Direct HTTP server-to-server requests.
  * SHA-1-OUT Pass phrase
    --> Enter a pass phrase here for security purposes, and remember it for later use
  * Optionally configure the "Direct HTTP server-to-server request" settings,
    if you would like to have KBC Paypage contact your website directly for payment feedback, rather than providing the feedback on redirect.
    (This will avoid problems with users closing their browser after payment was received, but before the redirect back to your site occurred, and other special cases).
    --> Choose a timing setting that suits you best
    --> Set both post-payment URLs to http://<your_website_address>/commerce-kbcpaypage/callback
    --> Request method can be POST or GET
    You can do the same for the setting "HTTP request for status changes" if needed.
  * Dynamic e-Commerce parameters
    --> Define which parameters will be sent back on the transaction feedback. Add AMOUNT and CURRENCY to selected parameters.

Installing & configuring the KBC Paypage payment method in Drupal Commerce
---------------------------------------------------------------------
- Enable the module (Go to admin/modules and search in the Commerce (Contrib) fieldset).
- Go to Administration > Store > Configuration > Payment Methods
- Under "Disabled payment method rules", find the KBC Paypage payment method
- Click the 'enable' link
- Once "KBC Paypage" appears in the "Enabled payment method rules", click on it's name to configure
- In the table "Actions", find "Enable payment method: KBC Paypage" and click the link
- Under "Payment settings", you can configure the module:
  * KBC Paypage account: define whether to use the test or production account
  * PSPID: add your KBC Paypage PSPID (the username you use to login as merchant)
  * Currency code: select a currency
  * Language code: let KBC Paypage know in which language the payment screens should be presented
  * SHA-IN Pass phrase: enter the pass phrase you saved while configuring KBC Paypage
  * SHA-1-OUT Pass phrase: enter the pass phrase you saved while configuring KBC Paypage
- You can now process payments with KBC Paypage!

Dynamic templates
=================
Dynamic templates allow you to keep the look of your site on the payment pages.
To use them, create a page containing $$$PAYMENT ZONE$$$ where the content of
the payment page should be displayed. This can be a node page, a page generated
by a custom menu router element or even a static HTML page (note that all
references to images, css and other resources should be using absolute paths).
The absolute URL to this page must be passed as a parameter to
hook_commerce_kbcpage_data_alter() (see commerce_kbcpage.api.php). The template page
must be publicly reachable, it will not work on a local development or a
password-protected server.
