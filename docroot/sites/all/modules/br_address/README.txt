
DESCRIPTION
===========

This module implements a format plugin to Address Field module, commonly used
with Drupal Commerce. This plugin enable a specific form for brazilian
addresses, according to recomendations of the brazilian postal service company,
Correios.

Additional features

- Autocomplete for city names based on a list of all brazilian cities
- Atomatic address filling by CEP querying to Correios site
  (http://m.correios.com.br)

Known issues

- When installed, the module creates a database table and inserts the names of
  all brazilian cities (more than 5000), what may takes a long time. It doesn't
  use the Batch API yet, so you can have problems with the execution time.

USAGE
=====

To use it in Drupal Commerce, after install the module, go to "Store ->
Customer profiles -> Profile types -> Billing (or Shipping) Information ->
Manage Fields" (admin/commerce/customer-profiles/types/billing/fields).
Then, edit the Address field. In "Format handlers" fieldset, uncheck "Address
form (country-specific)" and check "Brazilian address".
