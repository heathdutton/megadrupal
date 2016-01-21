<?php

require_once dirname(__FILE__) . '/includes/fortytwo.inc';
require_once dirname(__FILE__) . '/includes/theme-settings-general.inc';

function fortytwo_form_system_theme_settings_alter(&$form, $form_state) {

  $form['fortytwo_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -10,
    '#prefix' => '<h3>' . t('Configuration') . '</h3>',
  );

  fortytwo_theme_settings_general($form, $form_state);

}