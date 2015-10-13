Tropo Rules
================================================================================
Tropo Rules integrates the Tropo Telephony API with Drupal's rules module,
allowing any registered event to trigger the delivery of an SMS message, an
automated phone call, and any other service that Tropo offers.

This module is currently still in its infancy so the feature set is still a bit
small.  Contributions are always welcome.


Installation
--------------------------------------------------------------------------------
1. Create an account at tropo.com.
2. Create an application through your new Tropo account.
3. Copy your outbound Tokens to the Setings form on your website under
   Configuration > Web Services > Tropo API.
4. You will need to add files to your Tropo application to handle the requests
   sent by your Drupal site.  Follow the instructions here to learn how to set
   them up. https://www.tropo.com/docs

Now you are ready to go! You should see new actions under the Tropo group when
you create your rules.
