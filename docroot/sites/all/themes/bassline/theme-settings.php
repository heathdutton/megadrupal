<?php
/**
 * @file
 * Theme-settings.php.
 */

/*
 * Provides theme settings for Bassline.
 */

function bassline_form_system_theme_settings_alter(&$form, $form_state) {
  // Wrap global setting fieldsets in vertical tabs.
  $form['general'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<h2><small>' . t('Override Global Settings') . '</small></h2>',
    '#weight' => -8,
  );
  $form['theme_settings']['#group'] = 'general';
  $form['logo']['#group'] = 'general';
  $form['favicon']['#group'] = 'general';

  // Theme specific settings.
  $form['bassline'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<h2><small>' . t('Theme Settings') . '</small></h2>',
    '#weight' => -9,
  );

  // Components.
  $form['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb'),
    '#group' => 'bassline',
  );
  // Breadcrumbs.
  $form['breadcrumb']['bassline_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Breadcrumb'),
    '#default_value' => theme_get_setting('bassline_breadcrumb'),
    '#description'   => t("Select this option to enable the breadcrumb."),
  );
  $form['breadcrumb']['bassline_breadcrumb_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show current page title'),
    '#default_value' => theme_get_setting('bassline_breadcrumb_title'),
    '#description'   => t("When this option is enabled, display the current page title at the end of the breadcrumb."),
    '#states' => array(
      'invisible' => array(
        ':input[name="bassline_breadcrumb"]' => array('checked' => FALSE),
      ),
    ),
  );

}
