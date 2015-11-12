The Textmarketer module provides integration between the Textmarketer SMS
service and the SMS Framework module, the module provides no interface of its
own but can be accessed through the interfaces provided by the SMS Framework.

The module now supports receiving incoming messages if you use the txtUs Plus
service, this should be set up in your Textmarketer account to use the API URL
provided on the gateway configuration page for Textmarketer
(admin/smsframework/gateways/textmarketer).

The module makes use of the PHP wrapper (v1.3) provided by Textmarketer
http://www.textmarketer.co.uk/pdfebooks/TMRestClient.zip

For this module to work the PHP wrapper provided by Textmarketer should be
placed so that the TMRestClient.php file is reachable using the following path:
sites/all/libraries/TMRestClient/TMRestClient.php
