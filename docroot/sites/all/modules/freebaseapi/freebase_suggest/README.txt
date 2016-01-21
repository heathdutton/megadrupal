This small plugin just attaches the Google Freebase autocomplete external 
library to Drupal form fields.

It is configured per-field in the field widget settings form.

Enter the *type* of thing this field is expected to hold, and the Freebase
lookup and info box will be used to provide suggestions.

This uses only client-side javascript libraries to talk directly to 
Freebase, and doesn't connect any deeper into Drupal-side logic.
For greater semantic coupling or Drupal-side control, look at freebase_data.

This module is not related directly to the Freebase API project, but is here
as a related experiment.

@see https://developers.google.com/freebase/v1/search-widget