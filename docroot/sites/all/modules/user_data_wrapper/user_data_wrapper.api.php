<?php
/**
 * @file
 * API documentation.
 *
 * Defining a new user_data property will allow you to use
 * your data in entity meta wrappers.
 *
 * for example a definition of:
 *
 * function hook_user_data_info() {
 *   return array(
 *     'your_module' => array(
 *       'name' => 'property_name',
 *       'label' => t('Property label'),
 *       'type' => 'array',
 *     ),
 *   );
 * }
 *
 * would be used like:
 *  $wrapper = entity_metadata_wrapper('user', $user);
 *  $wrapper->property_name->get();
 *  or
 *  $wrapper->property_name->set();
 */

/**
 * Implements hook_user_data_info().
 *
 * To use this API you first need to define the property of the user object, you
 * can then use it via the entity meta wrapper.
 * @code
 *    global $user;
 *    $user_wrapper = entity_metadata_wrapper('user', $user);
 *    $user_wrapper->property_one->set('Hi');
 *    $data = $user_wrapper->property_one->value();
 *
 *    $user_wrapper->property_two->set(array('statement' => 'Hi'));
 *    $data = $user_wrapper->property_two->value();
 * @endcode
 */
function hook_user_data_info() {
  return array(
    'property_one' => array(
      'label' => t('Property One'),
      'type' => 'text',
    ),
    'property_two' => array(
      'label' => t('Property Two'),
      'type' => 'array',
    ),
  );
}
