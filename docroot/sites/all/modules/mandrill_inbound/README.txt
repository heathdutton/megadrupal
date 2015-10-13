-- SUMMARY --

This module provides integration with Mandrill Inbound email processing.

-- FEATURES --

The main module provides 'Incoming message is received' event for Rules module with related variables.

Also it has integration with some modules:
  * Privatemsg - Users will be able reply to private messages by email.
  * Subscriptions - Users will be able reply to comments by email.

-- REQUIREMENTS --

* Mandrill
* Rules

-- INSTALLATION --

1. Enable 'Mandrill Inbound' module.
2. Go to 'admin/config/services/mandrill/inbound' page and add email domain. (in.example.com)
3. Go to https://mandrillapp.com/inbound and add new Inbound Domain. (in.example.com)
4. Add route for domain:
  - Route: *@in.example.com
  - Webhook: http://example.com/mandrill/webhook/inbound

You are able to manage webhooks on follow page: https://www.mandrillapp.com/settings/webhooks/
