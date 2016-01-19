== What ==

This module provides event receipt generation for Drupal users who have paid event
records in CiviCRM.

It generates a node of type "civicrm_event_receipt" so admins can include a print function
using the Print module and have users print or create PDF versions of the receipt.

To avoid accumulation of receipt nodes that are left untouched, you can configure the module
to delete receipts older than X amount of time.
This is configurable at admin/config/civicrm/civicrm_event_receipts

== How ==

To access any user's list of receipts, go to user/<uid>/event-receipts.
To access the event receipts for the current user, go to user/<uid>/event-receipts.