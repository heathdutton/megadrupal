Introduction
============

Integration of the PostcodeAnywhere (www.postcodeanywhere.co.uk) service to Address Field module.

When user selects United Kingdom he also have possibility to find his address by postcode (using external service) and prepopulate his address details.


How to configure
================

After installation To make it works there are 2 steps:
1. Set the PostcodeAnywhere service credentials:
Administration -> Configuration -> Regional and language -> Postcodeanywhere settings (admin/config/regional/postcodeanywhere_addressfield).

2. Select the "PostcodeAnywhere" format handler in the addressfield instance.
ex. (Drupal commerce) Billing customer profile addressfield (admin/commerce/customer-profiles/types/billing/fields/commerce_customer_address).

How it works
============

There are 2 "widgets" available:

1. Search address button (used by default)
2 fields available:
 - "House number or name": optional field for the given postcode;
- "Postcode": required field.
According with the search it will return 2 kind of results:
 - Address filled form with the address for a valid "House number or name";
 - Address form with a selector for addresses of the postcode, for the postcode search only.

2. Autocomplete postcode search
The autocomplete will return a list of the addresses to have selection from, if the postcode is a valid UK postcode.
After the address selection beside the filled UK address form,
there will be both the postcode autocomplete and addresses selector available on the form.

