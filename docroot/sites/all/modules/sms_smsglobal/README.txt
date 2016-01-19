SMS Global
----------
This module allows the use of SMSGlobal (https://www.smsglobal.com/) as a
gateway in the SMS Framework.

INSTALL
-------
1. Download the SMSGlobal PHP REST API Client into
'sites/all/libraries/smsglobal' (or similar).

2. Enable the module, see the Getting Started guide for more information:
https://drupal.org/getting-started/install-contrib/modules

3. You can now setup the SMSGlobal gateway at admin/smsframework/gateways.

Incoming SMS Callback
---------------------
If you want the sms framework to know about replies to your SMS from
SMSGlobal, you add the incoming callback to your SMSGlobal dashboard.

Incoming callback URL: /sms/smsglobal/incoming
SMSGlobal settings page: https://mxt.smsglobal.com/settings

By default, this URL is not IP restricted. This can be enabled on the gateway
settings page.

In order to use the incoming callback, you need to have purchased a dedicated
number. The 'shared numbers' shared pool is not supported at this point.

Credits
-------
Chinthaka Godawita
Sponsored by Demonz Media (http://www.demonzmedia.com/)
