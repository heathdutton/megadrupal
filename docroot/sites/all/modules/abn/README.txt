## Australian Business Number (ABN)

ABN adds a new field type that stores and validates Australian Business Numbers.

This field also (optionally) pulls data from the Australian Business Register
for publicly available information on entered ABNs. Syncing is done as content
is saved and on cron runs to keep existing information fresh.

The data from the ABR is made available for display through formatters
compatible with Views, Display Suite, etc. in the same way as user entered data.

## Installation

In order to enable syncing data from the ABR you will need to lodge an
application for a GUID (Api key) with the ABR at
http://www.abr.business.gov.au/RegisterAgreement.aspx and then enter it in your
Drupal site at "admin/config/services/abn" (you will need the relevant
permissions to view this page).

Be aware that applications for GUIDs lodged with the ABR involve both accepting
the Terms and Conditions which notably include not "exposing" the ABR's API
directly to your users and a waiting period of roughly 2 working days.
