<?php

/**
 * @file
 * Theme settings file for the WetKit Omega theme.
 */

require_once dirname(__FILE__) . '/template.php';

/**
 * Implements hook_form_FORM_alter().
 */
function wetkit_omega_form_system_theme_settings_alter(&$form, $form_state) {
  // You can use this hook to append your own theme settings to the theme
  // settings form for your subtheme. You should also take a look at the
  // 'extensions' concept in the Omega base theme.
  if (isset($form['omega']['layouts'])) {
    $form['omega']['layouts']['#prefix'] = '<div class="messages warning wxt-message">' . t('This has been purposely disabled by Drupal WxT as we use a panels based layout system.') . '</div>';
  }
}
