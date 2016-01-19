* Description
This module allows you to connect your site to the FIA-NET API to add a layer
of verifiations to your orders.
FIA-NET is giving you the possibility to use two services:
- Receive and Pay
- SAC (Système d'Analyse des Commandes)

So far, this module only implements the SAC method.


* Configuration
Before using this module you have to set up the merchant information.
To configure that, go to Store >> Configuration >>
FIA-NET settings (admin/commerce/fianet).

- On this configuration page, fill the merchant ID, login and password
to connect to FIA-NET backoffice.

- You can determine how many items should be sent to the API every time the
cronjob is run.

- You can set up the API mode to use, wether Test mode if you are running
your site on a development environment or on a preproduction stage.

* Process
Commerce FIA-NET is relying on a rule, the module is creating a custom action,
this rule can be overriden, the default comportment is that when the order
reach the "completed" state, the order is queued in the FIA-NET queue.
This queue is sent to FIA-NET when the cron is run. The number of processed
items is defined in the configuratio page.
