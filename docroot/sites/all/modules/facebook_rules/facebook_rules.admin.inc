<?php

/**
 * Callback for hook_menu().
 */
function facebook_rules_settings_form($form, &$form_state) {
  $form['facebook_rules_markup'] = array(
    '#markup' => l('Read here', 'admin/help/facebook_rules') . t(' before changing default values.'),
  );
  $form['facebook_rules_renewal_days_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Renew user tokens after every: (days)'),
    '#default_value' => variable_get('facebook_rules_renewal_days_interval', '50'),
    '#description' => t('Set the period after which the user tokens will be renewed. <em><b>(Warning: Should not be more than 60)</b></em>'),
  );
  $form['facebook_rules_cron_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Set cron limit'),
    '#default_value' => variable_get('facebook_rules_cron_limit', '50'),
    '#description' => t('Set the limit of tokens to renew in a single cron run.'),
  );
  
  return system_settings_form($form);
}
