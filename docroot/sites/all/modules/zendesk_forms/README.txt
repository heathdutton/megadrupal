Zendesk Forms.

This module can display forms to the drupal user and submit the entered data to
Zendesk. The forms can be used programmatically or just be embedded via the
provided block(s).
Uses the Zendesk API V2. Right now the only form available is one that
will create a ticket at Zendesk, but maybe more forms can come. Contributions
are very welcome :)

To install the module, simply enable it and go to http://yoursite.com/admin/config/services/zendesk-form
and enter your Zendesk credentials.

There are four fields in the form (name, e-mail, subject, and description), but
more fields can be added in hook_form_alter and the submit handler will
recognize fields prefixed with "zendesk_" and pack them in the API call to
Zendesk.

Here are some links to Zendesk API documentation:
General introduction
http://developer.zendesk.com/documentation/rest_api/introduction.html
Tickets
http://developer.zendesk.com/documentation/rest_api/tickets.html

Module is developed at <a href="http://drupal.org/node/1926900">Reload!</a>.