<?php
/**
 * Allow themes to alter the theme-specific settings form.
 *
 * With this hook, themes can alter the theme-specific settings form in any way
 * allowable by Drupal's Forms API, such as adding form elements, changing
 * default values and removing form elements. See the Forms API documentation on
 * api.drupal.org for detailed information.
 *
 * Note that the base theme's form alterations will be run before any sub-theme
 * alterations.
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function fusion_slate_form_system_theme_settings_alter(&$form, $form_state) {
  $form['fusion_slate_banner'] = array(
    '#type' => 'fieldset',
    '#weight' => 1,
    '#title' => t('Fusion Slate - Banner Styles Wrapper'),
    '#description' => t('Select the Banner Style'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['fusion_slate_banner']['fusion_slate_banner_style'] = array(
    '#type' => 'radios',
    '#title' => t('Fusion Slate - Banner Styles'),
    '#options' => array(
      'fusion_default' => t('None'),
      'banner-background-beachstones banner-background' => t('Beach stones'),
      'banner-background-citystreetlights banner-background' => t('City street lights'),
      'banner-background-greekheads banner-background' => t('Greek heads'),
      'banner-background-seascape banner-background' => t('Seascape'),
      'banner-background-windows banner-background' => t('Windows'),
    ),
    '#default_value' => theme_get_setting('fusion_slate_banner_style'),
  );

}
