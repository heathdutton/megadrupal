INTRODUCTION
------------

The GovDelivery TMS Integration module provides Drupal integration with the
GovDelivery Transactional Messaging Service (TMS). The module replaces the
backend SMTP library in your Drupal site with calls to the GovDelivery service, so all mail sent from your site uses TMS.

The module provides the following features:

1. Messages are always sent from pre-defined "from" addresses based on the TMS account settings.
2. Messages/Subscriptions can be sent immediately or queued. Messages/Subscriptions with errors are queued for resend.
3. An administrative interface for reviewing queued messages and sending individual messages in the queue.
4. An administrative interface for sending test messages/subscriptions.
5. Integration with the nagios module for monitoring the size of the outbound message queue.


CONFIGURATION
-------------------

Once the module is installed and enabled, the user must go to the Configure screen and enter relevant GovDelivery information to activate the module:

1. From Name (The name displayed in the From field of the received email. E.G. John Smith)
2. Server Address (The URL for the TMS instance. Include HTTPS://)
3. Authorization Token (For the TMS account. If you do not have one, please content your GovDelivery CSC)
4. Use TMS for Outbound Mail:  To begin sending messages via TMS , you must enable this setting.
5. Queue Mail for High Volume  "Messages that fail to send are queued regardless of this setting."
6. Cron job for Mail :  "Will automatically resend the messages queue on cron runs. Enable if Queuing is Enabled " 
7. Override the From Name defined above for Web forms and other modules.
8. Set Maximum Bins (Used with Queue Asynchronous Processing)
9. Set Cron Interval (in Seconds)


OPERATING DETAILS
-----------------

There is no visible interface to this module.

Once activated, all mail seeing originating on the Drupal instance will be routed through the GovDelivery TMS service.
Reporting metrics can be retrieved via an API call to the TMS service. Details of this will be shared when the TMS account is created and the authentication token delivered to the client.

SUPPORT
-------

For help in configuring the module or for any other questions please contact Govdelivery Customer Support by emailing help@govdelivery.com

If you are a government or transit organization and would like to learn about GovDelivery and its products including greater detail on TMS, please email: info@govdelivery.com or navigate to www.govdelivery.com