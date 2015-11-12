The Parature API for PHP implements Libraries hooks to tie the Parature API
Library (http://www.andrewmacrobert.com/parature-api-library) into Drupal. It
also provides an administrative form for setting default Parature credentials.

To use the Parature API in Drupal module development, load the Parature API
Library and create a new instance of the ParatureAPI class like so:

libraries_load('parature_api');
$p = new ParatureAPI();

Parature (http://www.parature.com) is a customer service system.
