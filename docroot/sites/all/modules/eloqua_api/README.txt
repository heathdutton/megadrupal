-- SUMMARY --

This module gives developers an easy, configurable way to integrate Drupal forms
with the Eloqua automated marketing platform.  If you do not already have or are
not considering an Eloqua subscription, this module is not for you.  If you are
not a developer or you do not have a developer to help you integrate Drupal with
Eloqua, this module is also probably not for you.


-- INSTALLATION --

* Enable the module
* Give appropriate roles the "administer eloqua api" permission.
* Configure the module at /admin/config/system/eloqua-api/settings


-- CONFIGURATION --

At a minimum, you will need to set the following on the aforementioned
configuration page:

* The URL to which Eloqua data will be sent via an HTTP POST request.
* Your Eloqua Site ID.
* You will need to enable at least one form.

See the "usage overview" section below for more details on enabled forms.
Additional details on the "push defined values" check are also explained there.

There are several optional settings that can be changed on the settings page:

* When "Log Eloqua posts" is checked, form submissions sent to Eloqua will be
  logged via watchdog. You can see these by filtering to messages of type
  "eloqua api" at /admin/reports/dblog
* When "Enable Eloqua tracking" is checked, Eloqua API will attempt to include
  your Eloqua tracking script on every page load. Note that this may not be
  necessary if you already include this on your site in another way. You should
  put this script in a location like: sites/all/libraries/elqNow/tracking.js
* When "Send Eloqua customer GUID" is checked, Eloqua API will attempt to append
  the customer GUID to the form submission. You will need to include your Eloqua
  tracking script (either by checking the box above or some other means) in
  order for this feature to work. This will not work for users with cookies or
  JavaScript disabled.


-- USAGE OVERVIEW --

This module provides a function eloqua_api_post($params) that will perform
an asynchronous HTTP POST request to the URL specified on the module's
configuration page.  If you'd like, you may simply use this function in your own
custom module to post values to Eloqua.

Additionally, this module allows developers the ability to make forms Eloqua
aware by adding a small submit handler to all forms marked "enabled" on the
module's configuration page.  Depending on the value of "manual field mapping,"
it will either use the standard $form_state['values'] array or the custom
$form_state['eloqua_values'] array for parameters sent to eloqua_api_post().

By default, this module is set to use eloqua_values and the only exposed form is
the user registration form, so nothing will happen.

Please use the following code samples to guide your Eloqua integration.


-- CODE EXAMPLES --

You'll likely want to create a custom module that depends on this module. Here
are a few snippets to help you get started. Everything you need can be
accomplished with a few simple form alters.

/**
 * Implements hook_form_alter().
 */
function MY_MODULE_form_alter(&$form, &$form_state, $form_id) {
  switch ($form_id) {
    // Expose the form you want to the Eloqua API Configuration Settings.
    case 'eloqua_api_admin_settings':
      $form['eloqua_api_forms']['eloqua_api_enabled_forms']['#options']['MY_FORM_ID'] = 'My form name';
      break;

    // Add a validation function to the form you exposed above.
    case 'MY_FORM_ID':
      $form['#validate'][] = 'MY_MODULE_eloqua_validate';
      break;
  }
}

/**
 * Validation handler to map Eloqua fields to be pushed.
 *
 * This is where you define what fields will be pushed to Eloqua.
 */
function MY_MODULE_eloqua_validate($form, &$form_state) {
  // Maybe you want to send the e-mail address.
  $fields['mail'] = $form_state['values']['mail'];

  // Additional fields here...
  $fields['foo'] = $form_state['values']['foo'][LANGUAGE_NONE][0]['value'];

  // Add more fields here...

  // Add your field mappings here so the Eloqua API module sees them.
  $form_state['eloqua_values'] += $fields;
}


-- HIDDEN FEATURES --

In your settings.php file, you may set...

$conf['eloqua_api_skip_post'] = TRUE;

...in order to prevent form submittals from posting to Eloqua in your dev and
staging server environments.
