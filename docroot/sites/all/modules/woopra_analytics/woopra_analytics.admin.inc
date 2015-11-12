<?php
/**
 * @file
 * Administration settings.
 */

function woopra_analytics_control_panel_form($form, &$form_state) {
  $form = array();

  $parse_url = parse_url(url('<front>', array('absolute' => TRUE)));

  $domain = !empty($parse_url['host']) ? preg_replace('~^(www\.)?~', '', $parse_url['host']) : $_SERVER['SERVER_NAME'];

  $form['woopra_analytics_domain'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Domain'),
    '#description'   => t('Used in woopra basic tracking.'),
    '#default_value' => variable_get('woopra_analytics_domain', $domain),
  );

  $form['woopra_analytics_switch'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Basic tracking'),
    '#description'   => t('Woopra Analytics basic tracking site.'),
    '#default_value' => variable_get('woopra_analytics_switch', 0),
  );

  return system_settings_form($form);
}
