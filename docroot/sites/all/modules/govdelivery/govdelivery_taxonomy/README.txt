INTRODUCTION
------------

Give your stakeholders flexibility in topic subscription. The
GovDelivery taxonomy integration module for Drupal automatically creates
subscription topics for every taxonomy term or content tag that you create
within your Drupal instance. Your website visitors will be able to subscribe
to the taxonomy terms and content tags. When a new page, story, or custom
content type is published with tags, all interested subscribers will be
automatically notified with a DCM Bulletin send.


CONFIGURATION
-------------

Once the module is installed and enabled, the user must go to the Configure
screen and enter relevant GovDelivery information to activate the module:

1. Web Services Administrator Username

2. Web Services Administrator Password

3. GovDelivery DCM Client Account Code

4. GovDelivery API URL (Without HTTPS://)

5. Drupal Instance Base URL (Without HTTPS://) (This is the URL to this Drupal instance without any subdirectories)

6. GovDelivery DCM Parent Category Code for Created Topics


OPERATING DETAILS
-----------------

There is no visible interface to this module.

Once activated, the GovDelivery taxonomy module creates a Topic
in the GovDelivery platform for every taxonomy term or vocabulary
created in Drupal. All Drupal terms and vocabularies are assigned
to a single Category within GovDelivery, specified in the configuration screen.
When the term is created in Drupal, the module uses the GovDelivery Create Topic API
to create the corresponding GD Topic and assign it to the specified category.


SUPPORT
-------

For help in configuring the module or for any other questions please
contact Govdelivery Customer Support by emailing help@govdelivery.com

If you are a government or transit organization and would like to learn
about GovDelivery and its products, please email:
info@govdelivery.com or navigate to www.govdelivery.com