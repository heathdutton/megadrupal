<?php
/**
 * @file
 * API Documentation for RoyalSlider module.
 */

/**
 * Implements hook_royalslider_settings_alter().
 *
 * Alter the RoyalSlider settings before being added to the browser.
 *
 * @param array
 *   An array of settings to be added via drupal_add_js().
 * @param string
 *   The royalslider instance ID.
 * @param object
 *   The royalslider_optionset object used to build these settings.
 */
function hook_royalslider_settings_alter(&$settings, $id, $optionset) {
  if ($id = 'baz' && $optionset->name === 'foo') {
    $settings['bar'] = 'baz';
  }
}

/**
 * Implements hook_royalslider_skins_alter().
 *
 * Alter the available RoyalSlider skins.
 *
 * Used to add new custom skins or alter the existing ones.
 *
 * @param array
 *   An array of skin information, containing:
 *    - name: The human-friendly name of the skin.
 *    - class: The class name for this skin, as defined in the css file.
 */
function hook_royalslider_skins_alter(&$skins) {
  $skins['my-awesome-skin'] = array(
    'name' => t('My Awesome Skin'),
    'class' => 'rsAwesome',
    // Your CSS should live in the royalslider/skins/my-awesome-skin folder
    // and be named rs-my-awesome-skin.css.
  );
}

/**
 * Implements hook_royalslider_easing_alter().
 *
 * Alter the available jQuery Easing methods available to RoyalSlider.
 *
 * @param array
 *   An array of easing method names.
 */
function hook_royalslider_easing_alter(&$methods) {
  $methods[] = 'my-custom-method';
}

/**
 * By design, RoyalSlider should be entirely configurable from the web interface.
 * However some implementations may require to access the RoyalSlider library
 * directly by using royalslider_add().
 *
 * Here are some sample uses of royalslider_add().
 */

/**
 * This call will look for an HTML element with and id attribute of "my_image_list"
 * and initialize RoyalSlider on it using the option set named "default" and
 * will override the default option set's skin with the "Universal" skin.
 */
royalslider_add('my_image_list', 'default', 'universal');

/**
 * This call will look for an HTML element with and id attribute of "my_image_list"
 * and initialize RoyalSlider on it using the option set named "default".
 */
royalslider_add('my_image_list', 'default');

/**
 * You also have the option of skipping the option set parameter if you want
 * to run with the library defaults or plan on adding the settings array
 * into the page manually using drupal_add_js().
 */
royalslider_add('my_image_list');

/**
 * Finally, you can simply have Drupal include the library in the list of
 * javascript libraries. This method would assume you would take care of
 * initializing a RoyalSlider instance in your theme or custom javascript
 * file.
 *
 * Ex: $('#slider').royalslider();
 */
royalslider_add();
