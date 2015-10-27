<?php

/**
 * When implementing hook_token_info, we need to provide an array of types
 * and one of tokens (as each token belongs to a type). If your token belongs
 * to an existing type, you don't even need to return type information.
 *
 * Since we are declaring our tokens individually with the ctools plugins,
 * we need to specify which type the given token belongs to.
 *
 * This means that when we define a token plugin you have options. 1. If we
 * specify a type, the token will be of that type. Otherwise it will belong
 * to a general 'Info Plugins' type. 2. If you define multiple tokens that need
 * to be of the same type, you only have to provide the type information
 * for the first one. For all subsequent ones it will be enough to just specify
 * the machine_name of the type. 3. You can specify an already existing token
 * type such as 'node' if that makes sense.
 *
 * See below for more information.
 */

$plugin = array(
  // The first three keys related to the array you would normally construct
  // under the 'tokens' keyed array in hook_token_info(). However, to avoid conflict,
  // all the keys are preceded by the work 'token'.
  'token name' => 'Example token',
  'token description' => 'Example description',
  // (optional) 'type' under the token array of hook_token_info().
  'token type' => '',

  // The next 4 keys are optional (to a certain extent) and they represent the
  // data you would normally add into the 'types' keyed array in hook_token_info(),
  // except the 'type machine_name' which is what you would specify as the key of
  // the array defining your type. And like above, to avoid name conflicts, these
  // are preceeded by the word 'type'.

  // Leaving this key out would place your token into the default 'Info Plugins'
  // type.
  'type machine_name' => 'example_token_type',

  // If you have declared another token plugin which belongs to the 'example_token_type',
  // and has all this info, the values below are no longer required.
  'type name' => 'My type',
  'type description' => 'My type desc',
  'type needs-data' => '',

  // The 'callback' optional key allows you to specify the function which returns
  // the value of the token. It defaults to this construct: module_name_plugin_name_token().
  'callback' => 'info_plugins_example_token_token',
);

/**
 * Callback function that returns the value of the token.
 *
 * @param $data
 * @param $options
 * @return string
 */
function info_plugins_example_token_token($data, $options) {
  return '';
}