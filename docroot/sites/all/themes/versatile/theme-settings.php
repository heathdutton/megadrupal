<?php
/**
 * Implements hook_form_FORM_ID_alter().
 */
function versatile_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['versatile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Versatile settings'),
  );
  
  $form['versatile']['versatile_header_outside'] = array(
    '#type' => 'checkbox',
    '#title' => t('Place header outside page wrapper'),
    '#default_value' => theme_get_setting('versatile_header_outside'),
    '#description' => t('Makes the header element be placed outside of the page wrapper.'),
  );

  $form['versatile']['versatile_footer_outside'] = array(
    '#type' => 'checkbox',
    '#title' => t('Place footer outside page wrapper'),
    '#default_value' => theme_get_setting('versatile_footer_outside'),
    '#description' => t('Makes the footer element be placed outside of the page wrapper.'),
  );
  
  $form['versatile']['versatile_responsive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use responsive layout'),
    '#default_value' => theme_get_setting('versatile_responsive'),
    '#description' => t('Use responsive layout that adapts based on viewport width. Please note that you have to use the <em>Versatile Site Template</em> for this settings to take effect.'),
  );
}