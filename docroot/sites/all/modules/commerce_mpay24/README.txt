Commerce mPAY24
***************

Provides an mPAY24 integration for Drupal Commerce via the redirect method.


Installation:

 1. It is necessary to download the mpay24 php library on
    https://github.com/mPAY24/mpay24_php_api. This library is developed by
    mpay24.
 2. Place the library into your libraries folder (e.g. sites/all/libraries).
    The directory structure should look like libraries/mpay24/api/MPay24Api.php
 3. Configure a Commerce Payment Rule as with the other payment provider
    integration.
 4. Within the Rule, you need to specify the Merchant ID, the SOAP password and
    the 'mode' (live / test).
