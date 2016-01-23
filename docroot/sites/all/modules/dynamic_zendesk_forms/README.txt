Dynamic Zendesk Forms.
=======================

This is a module that helps to integrate zendesk and
your drupal site for rendering support forms and submitting
user input back to zendesk. This module dynamically pulls in form fields
and field rules directly from zendesk admin configuration. Without almost zero
coding we can set up a support form with fields of your choice.

This modules uses the rest apis exposed by zendesk 
for powering the support(contactus) form

The forms can be used programmatically or just be embedded via the
provided block(s).

Uses the Zendesk API V2.

Requirements
------------

Once the module is installed and enabled, browse to the Status Report
page (admin/reports/status) and confirm that the curl lib is installed.

Configuration Parameters
---------------------------------
Go to admin/tve/zendesk for configuring the zendesk.

Below are the details about the configurations

Zendesk API key-
Zendesk API Key. This will be used for making Zendesk api calls.

Zendesk username - 
The username of your Zendesk agent.

Zendesk API url  - 
Zendesk API url. The url is where you access your Zendesk backend.

Zendesk Ticket Form - 
Select the ticket form that need to be displayed in front end

Zendesk System to Custom Field Mapping Description - 
Select proper fields for the description,name and email fields

Configure the Minimum cache lifetime.

Zendesk will provide with separate account and url to access the backend. 
For ex:
https://xxxxx.zendesk.com/agent/. Then zendesk api url will be
https://xxxxx.zendesk.com/api/v2/

Usage:
-------------------------------
Once all the configurations are completed.
A drupal block will be created by name "Zendesk support form".

Alternatively you can call 
drupal_get_form('dynamic_zendesk_forms_create_ticket_form') 

which will return the form array so that 
we can display as per your requirements.

Please visit "/admin/structure/block" and enable the zendesk block.


Useful Additional Information
--------------------------------

Zendesk has ability to create (https://xxxx.zendesk.com/agent/) form and fields.
And we also have option to create multiple forms in zendesk
and attached has many fields has possible.
It has settings like to make it required and also
add conditional field settings in zendesk admin.

Basically this module will pull in the forms
and also form fields from zendesk including the label
and settings and will create a drupal form for the user.

Here are some links to Zendesk API documentation:
General introduction
http://developer.zendesk.com/documentation/rest_api/introduction.html
Tickets
http://developer.zendesk.com/documentation/rest_api/tickets.html
Conditional fields
https://xxxx.zendesk.com/agent/#/apps/conditional-fields
Create forms:
https://xxxx.zendesk.com/agent/#/admin/ticket_forms
Create fields:
https://xxxx.zendesk.com/agent/#/admin/ticket_fields

Additional Information:
In certain case when we are working on local,
"CURLOPT_SSL_VERIFYPEER" does not work properly, 
so temporarily you could disable it by adding 
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); 
in the _dynamic_zendesk_forms_perform_curl_request function.
