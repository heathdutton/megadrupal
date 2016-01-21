-- SUMMARY --

This module provides Google Search Appliance's Query Suggestion functionality by
adding autocomplete functionality to the Google Search Appliance search form.


-- INSTALLATION --

Enable the module in the usual Drupal way. You may configure the module on the
main Google Search Appliance module configuration page at...

  * admin/config/search/google_appliance/settings

This module requires the Google Search Appliance module already be installed.

-- NOTES --

By default, the enter key will submit the autocomplete search form. To disable
this behavior, you can add a data-settings attribute to the form with a
JSON-encoded object. Set the property named submitEnter to false, i.e.
"{submitEnter: false}".

Example form alter with PHP:

    <?php
    $form['#attributes']['data-settings'] = drupal_json_encode(array(
      'submitEnter' => FALSE,
    ));
