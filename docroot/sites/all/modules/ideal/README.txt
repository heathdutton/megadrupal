CONFIGURATION
-------------

- Create a new payment method at Administer > Configuration > Web services >
  Payment > Payment methods > Add payment method
  (admin/config/services/payment/method).
  Look up the necessary configuration values in ACQUIRERS.txt or with your
  acquirer.
  Create a private key and public certificate using the instructions in the
  Merchant Integration Guide that is provided by your acquirer, or contact
  http://mynameisbart.com.
- Configure cron to run at least every 24 hours, because transaction statuses
  may only be requested within 24 hours of transaction expiration.
