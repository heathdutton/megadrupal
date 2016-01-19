<?php

/**
 * @file
 * Hooks provided by the Sassy module.
 */

/**
 * This hook is not provided by the Sassy Substitutions module but listed here
 * as it can be used to provide tokens without the need for you to implement
 * the token info hook manually.
 *
 * You can simply set '#sassy' => TRUE on whatever Form API form element that
 * you want to use in your stylesheets to make it accesible via
 * @token('your-token', 'default-value').
 *
 * The name of the token is generated from the array key of the Form API form
 * element. In the example shown here the token would be 'background-color-token'
 * (dashes instead of underscores).
 */
function hook_form_system_theme_settings_alter(&$form, &$form_state) {
  // Adds a background color textfield to the theme settings form and exposes it
  // as a token in your stylesheets.
  $form['background_color_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#description' => t('This serves as a SASS / SCSS token for the background color of this website.'),
    '#default_value' => theme_get_setting('background_color_token'),
    // This line ('#sassy' => TRUE) exposes this theme setting as a token in
    // your stylesheets.
    '#sassy' => TRUE,
  );
}

/**
 * Use this hook to provide your own custom replacement tokens.
 *
 * The Sassy Substitutions token system supports the following properties:
 * - token: (required) The name of the token which is used as the first argument
 *   in the token syntax in your stylesheets to identify the token. This can
 *   be a simple string or a complex regular expression (useful if you want to
 *   replace different tokens with the same callback or string).
 * - label: (optional) The label (titel) of a token. It is not a required
 *   value but highly encouraged so other modules can identify a token.
 * - description: (optional) The description of a token. It is not a required
 *   value but highly encouraged so other modules can identify a token.
 * - string: (optional) The string that the token will get replaced with in
 *   your stylesheets. Required if 'callback' is not set.
 * - callback: (optional) The callback function that should be executed in order
 *   to retrieve the replacement value for the token. Required if 'string' is
 *   not set.
 * - preprocess: (optional) Wether or not the token should be preprocessed with
 *   preg_quote(). You have to set this to false if your token is a regular
 *   expression. Defaults to TRUE.
 * - cache: (optional) Wether or not the return value of the callback function
 *   should be cached. Defaults to TRUE.
 */
function hook_sassy_substitutions_token_info() {
  $info['my_custom_callback'] = array(
    'label' => t('This is just a custom token with a callback function'),
    'description' => t('This token will be replaced with the return value of its callback function.'),
    'token' => 'my-custom-callback',
    'callback' => 'my_custom_callback',
    // Do not cache the return value of this callback and execute it freshly for
    // every sinle occurance of this token.
    'cache' => FALSE,
  );
  $info['my_custom_string']  = array(
    'label' => t('This is just a custom token'),
    'description' => t('This token will be replaced with the string value defined here.'),
    'token' => 'my\-custom\-string',
    'string' => 'A simple string',
    // No need to preprocess this as the dashes have already been marked as
    // non-regex characters manually (this does not make much sense for a real
    // token and is just here to serve as an example).
    'preprocess' => FALSE,
  );
  return $info;
}

/**
 * Alter hook for the token info array. Use this hook to change or remove one
 * or multiple tokens.
 *
 * @param &$info
 *   An array containing all defined substitution tokens.
 */
function hook_sassy_substitutions_token_info_alter(&$info) {
  // Remove 'my_custom_callback'.
  unset($info['my_custom_callback']);
}

/**
 * Use this hook to provide your own custom replacement patterns.
 *
 * The Sassy Substitutions pattern system supports the following properties:
 * - pattern: (required) The pattern to look up in the processed stylesheets.
 *   This can be a simple string or a complex regular expression.
 * - label: (optional) The label (titel) of a pattern. It is not a required
 *   value but highly encouraged so other modules can identify a pattern.
 * - description: (optional) The description of a pattern. It is not a required
 *   value but highly encouraged so other modules can identify a pattern.
 * - string: (optional) The string that the pattern will get replaced with in
 *   your stylesheets. Required if 'callback' is not set.
 * - callback: (optional) The callback function that should be executed in order
 *   to retrieve the replacement value for the pattern. Required if 'string' is
 *   not set.
 * - iteration: (optional) Flag that defines the point in the parsing process
 *   at which a pattern should be applied. Can be SASSY_PRECOMPILE or
 *   SASSY_POSTCOMPILE and default to SASSY_PRECOMPILE.
 */
function hook_sassy_substitutions_pattern_info() {
  $info['my_custom_pattern'] = array(
    'label' => t('This is just a custom pattern with a callback function'),
    'description' => t('This pattern will be replaced with the return value of its callback function.'),
    // Match all media query definitions in all stylesheets.
    'pattern' => '@media\s*(.+)\s*\{',
    'callback' => 'my_pattern_replacement_callback',
    // Execute this pattern after the SASS / SCSS compiler has done its job.
    'iteration' => SASSY_POSTCOMPILE,
  );
  return $info;
}

/**
 * Alter hook for the pattern info array. Use this hook to change or remove one
 * or multiple patterns.
 *
 * @param &$info
 *   An array containing all defined substitution patterns.
 */
function hook_sassy_substitutions_pattern_info_alter(&$info) {
  // Remove 'my_custom_pattern'.
  unset($info['my_custom_pattern']);
}