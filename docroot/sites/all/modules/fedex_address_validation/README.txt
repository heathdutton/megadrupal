In addition to the normal Drupal installation steps you will need to:

1. Signup for a FedEx developer account.
2. Apply for a test key at https://www.fedex.com/us/developer/web-services/process.html?tab=tab2.
3. Call FedEx (877-339-2774 press 2 then press 3) with your test account and
meter and request that then enable Address Validation on your test account.

This module has no User Interface, so you must set the following variables:

* fedex_address_validation_environment - must be either "test" or "live".
* Live credentials are made up of the following 4 values:
 * fedex_address_validation_live_key
 * fedex_address_validation_live_password
 * fedex_address_validation_live_account
 * fedex_address_validation_live_meter
* Test credentials are made up of the following 4 values:
 * fedex_address_validation_test_key
 * fedex_address_validation_test_password
 * fedex_address_validation_test_account
 * fedex_address_validation_test_meter

You can also change the following variables:

* fedex_address_validation_street_accuracy - must be one of the FedEx
values, defaults to 'LOOSE.
* fedex_address_validation_directional_accuracy - must be one of the FedEx
values, defaults to 'LOOSE'.
* fedex_address_validation_max_concurrent - set to control the number of
concurrent service calls, defaults to 10.
