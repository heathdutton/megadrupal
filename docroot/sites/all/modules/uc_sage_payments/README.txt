Ubercart Sage Payments Solutions Gateway
----------------------------------------
* Requires uc_payment and uc_credit to be enabled and properly configured. In
the store payment settings, enter your Sage Payment Solutions Merchant ID and
key, and also make sure to enable the 'card owner' field on the checkout form.

* What to do if you encounter the following cURL error:

"SSL certificate problem, verify that the CA cert is OK."

This means the server doesn't have an up-to-date list of trusted Certificate
Authorities installed. There are a couple of options to fix it...

Option A - Do a web search for "update the root CA certificate bundle" and find
instructions for your system.

Option B - Get a copy of the specific CA certificate that's required and place
it where this module and cURL will find it.

For option B, here are detailed instructions (works with Firefox):

1) Visit the HTTPS URL of the gateway in your web browser to get a copy of the
CA certificate: https://gateway.sagepayments.net/

2) Click the padlock icon to bring up the security details for the page.

3) Click "View Certificate".

4) Click over to the "Details" tab.

5) Select the top level item in the Certificate Hierarchy, for example: "Builtin
Object Token:Go Daddy Class2 CA"

6) Click "Export..." and save the CA cert as a type X.509 Certificate (PEM)
file, for example: "GoDaddyClass2CA.crt"

7) Put a copy of this file in the 'Encryption key directory' used by Ubercart,
which is configured in the credit card payment settings at Administration »
Store » Configuration » Payment methods » Credit card settings.

8) Add a line to the settings.php for your Drupal installation defining a
'uc_sage_payments_ca_cert' variable with the name of this file, for example:
  $conf['uc_sage_payments_ca_cert'] = 'GoDaddyClass2CA.crt';

That's it! Please report any problems in the issue queue.
