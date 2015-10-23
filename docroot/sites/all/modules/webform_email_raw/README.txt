Webform Email Raw
=================
This module addresses the use case where you need to send raw XML or some 
other text format in the message body that Drupal's default core would 
otherwise filter tags out of to generate plain text. It also allows override 
of the main mail message MIME type.

The functionality is configurable at the Webform node level under the Edit 
e-mail settings » E-mail template » Webform Email Raw settings, and is di
sabled by default.

*** WARNING! ***

Emailing raw, unfiltered (filter_xss) data from a Webform submission can be 
risky, so only use this module if you absolutely require this behavior and 
understand the risks. The ideal scenario is a machine receiving and processing 
the emails vs. going to an inbox that is opened by an interactive mail client.

