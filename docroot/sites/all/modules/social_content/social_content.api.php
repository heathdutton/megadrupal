<?php

/**
 * @file
 * Hooks provided by Social Content module
 */

/**
 * Define social content classes.
 *
 * This hook enables modules to register their own social content classes which
 *   instances can be created from.
 *
 * All social content imports belong to an instance and that instance is from a
 *  class which is defined by this hook. You classes should extend the abstract
 *  SocialContent class. Refer to the class for documentation.
 *
 * $return $info
 *   An array of social content classes, in machine_name => class format.
 *    Machine name must be unique and not defined anywhere else. To override
 *    a specific class use hook_social_content_class_info_alter.
 */
function hook_social_content_class_info() {
  return array(
    'twitter' => 'SocialContentTwitter',
  );
}

/**
 * Alter available social content classes.
 *
 * Useful for overriding extending class implementation with your own.
 *
 * @param array $classes
 *   Array of social content classes, define in hook_social_content_class_info.
 */
function hook_social_content_class_info_alter(&$classes) {
  if (isset($classes['twitter'])) {
    // We only want tweets with images.
    $classes['twitter'] = 'SocialContentTwitterSH';
  }
}

/**
 * Attach fields to the settings form so that they can be stored.
 *
 * You have the original record so you can grab fields from it and attach it to
 *  the wrapper.
 *
 * @param string $type
 *   The type of form this is, either instance or global.
 * @param array $form
 *   The form to alter.
 * @param object $settings
 *   The settings array with the current values.
 */
function hook_social_content_settings_alter(&$type, &$form, &$settings) {
  $options = array(NULL => 'None') + mymodule_get_content_list();
  switch ($type) {
    case 'instance':
      $form['contest'] = array(
        '#type' => 'select',
        '#title' => 'Contest',
        '#options' => $options,
        '#default_value' => isset($settings['contest']) ? $settings['contest'] : NULL,
      );
      break;
  }
}

/**
 * Alter the entity before it's saved.
 *
 * You have the original record so you can grab fields from it and attach it to
 *  the wrapper.
 *
 * @param EntityMetadataWrapper $wrapper
 *   The EntityMetadataWrapper with the fields in the settings set.
 * @param object $context
 *   The original record from the social network.
 */
function hook_social_content_import_alter(&$wrapper, &$context) {
  $settings = $context['settings'];
  if (isset($settings['contest']) && !empty($settings['contest'])) {
    $wrapper->field_contest->set($settings['contest']);
    $wrapper->field_terms->set(array(1));
  }
}
