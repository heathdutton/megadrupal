<?php
function localefix_admin_settings() {
  $form['localefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Locale Name'),
    '#description' => t('Full locale name such as "zh_TW.UTF-8", or "en_US.UTF-8".'),
    '#default_value' => variable_get('localefix', "zh_TW.UTF-8"),
  );
  $form['localefix_type'] = array(
    '#type' => 'select',
    '#title' => t('Locale Type'),
    '#options' => array(
      LC_ALL => 'LC_ALL',
      LC_COLLATE => 'LC_COLLATE',
      LC_CTYPE => 'LC_CTYPE',
      LC_MONETARY => 'LC_MONETARY',
      LC_NUMERIC => 'LC_NUMERIC', 
      LC_TIME => 'LC_TIME', 
      LC_MESSAGES => 'LC_MESSAGES',
    ),
    '#description' => t('Locale type to fix.'),
    '#default_value' => variable_get('localefix_type', LC_CTYPE),
  );
  return system_settings_form($form);
}
