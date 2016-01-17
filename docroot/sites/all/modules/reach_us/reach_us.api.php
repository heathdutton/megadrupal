<?php
/**
 * @file
 * Sample hooks demonstrating usage in Reach us.
 */

/**
 * To register one or more services, you have to do 3 things.
 *
 * Use hook_reach_us_add_services to register service.
 * Make sure you added required style to you css.
 * Optional but recommended, create a validate function for your service.
 */

/**
 * Define service(s) to reach us module.
 *
 * @return array
 *   An array of components keyed by machine name. The key will be used to
 *   create classes, it should be lowercase alphabets. If you use the existing
 *   key of main module, then the settings can be overridden
 *   - label: (required) , Human readable service name, same names uses for
 *     configuration form field labels.
 *   - linktext: (required) , Its default text appears in the Link text in
 *     front end, this can be override in admin settings
 *   - description: (required) Appears as field description for label in
 *     admin settings.
 *   - schema: (required) URL schema, use the token '!id' which will be
 *     configured as Service ID by admin. its value for 'href' attribute.
 *   - schemamsg: (Optional) URL schema, use the token '!id' which will
 *     be configured by Service ID admin. Use '!message' to pass the default
 *     message to apps, all apps wont support this, its value for 'href'
 *   - weight: (required) It helps to set the order of render, both in admin
 *     settings and in front end, can be override from admin settings.
 *   - valid: (Optional but recommended) Callback function to validate the
 *     input field, optional, skipping this will Skip the validation, get
 *     single argument which us string. Callback function should return false
 *     if no error, and return error message as string if entered data is wrong
 */
function hook_reach_us_add_services() {
  // Array key should be unique and alpha numeric.
  $services = array();
  // Label appear in admin configuration for.
  $services['yourapp']['label'] = 'My App name';
  // Linktext is Default link text, modifiable from admin back end.
  $services['yourapp']['linktext'] = 'SMS / Text us';
  // Appears as field description for label in admin end.
  $services['yourapp']['description'] = 'Enter your phone number to receive SMS from visitor.';
  // URL schema, use the token '!id' which will be configured.
  // As Service ID by admin. its value for 'href' attribute.
  $services['yourapp']['schema'] = 'mailto:!id';
  // URL schema use the token '!id' which can configured by Service ID admin.
  // Use '!message' to pass the default messages to apps.
  // All apps wont support this feature.
  $services['yourapp']['schemamsg'] = 'mailto:!id?subject=!message';
  // Default order in the form.
  $services['yourapp']['weight'] = 2;
  // Callback function to validate the input field, optional.
  // Skipping this will skip the validation.
  // Callback function should return false if no error.
  // Callback function should return string if entered data is wrong.
  $services['yourapp']['valid'] = '_reach_us_validate_yourapp';
  return $services;
}

/**
 * A sample validation function.
 */
function _reach_us_validate_yourapp($id) {
  // Return false of the id is valid.
  // Return error message as string if id is not valid.
}
