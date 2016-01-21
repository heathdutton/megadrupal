<?php
/**
 * @file
 * Admin settings for Lyris Simple.
 */

/**
 *  Settings form.
 */
function lyris_simple_settings_form($form, &$form_state) {
  $protocol = !empty($_SERVER["HTTPS"]) ? 'https://' : 'http://';
  $sub_vals = t('The following replacement values may be used: %email, %name, %list');
  $unsub_vals = t('The following replacement values may be used: %email, %list');

  $form['tabs'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 10,
  );

  $form['lyris_simple_host'] = array(
    '#type' => 'urlfield',
    '#title' => t('Lyris Host'),
    '#description' => t('Provide the location of your Lyris host instance.  Ex. "http://yoursite.sparklist.com"'),
    '#default_value' => variable_get('lyris_simple_host', ''),
  );

  $form['lyris_simple_list_name'] = array(
    '#type' => 'textfield',
    '#title' => t('List Name'),
    '#description' => t('Provide the Lyris list name to (un)subscribe the user to.'),
    '#element_validate' => array('lyris_simple_element_validate_list_name'),
    '#default_value' => variable_get('lyris_simple_list_name', ''),
  );

  // Subscribe/Unsubscribe settings from block configuration.
  $form += lyris_simple_block_configure('subscribe');
  $form += lyris_simple_block_configure('unsubscribe');
  $form['subscribe']['#tree'] = FALSE;
  $form['unsubscribe']['#tree'] = FALSE;

  // Flood Control
  $form['flood'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flood Control'),
    '#description' => t('Limit users\' submissions per form over a given time period.  Helps to prevent spamming.'),
    '#group' => 'tabs',
  );
  $form['flood']['lyris_simple_flood_threshold'] = array(
    '#type' => 'numberfield',
    '#title' => t('Threshold'),
    '#description' => t('Set to 0 for no limit.'),
    '#min' => 0,
    '#size' => 5,
    '#field_suffix' => t('submissions'),
    '#default_value' => variable_get('lyris_simple_flood_threshold', LYRIS_SIMPLE_FLOOD_DEFAULT_THRESHOLD),
  );
  $form['flood']['lyris_simple_flood_window'] = array(
    '#type' => 'select',
    '#title' => t('Timeframe'),
    '#field_prefix' => t('per'),
    '#field_suffix' => t('per form.'),
    '#options' => array(
      60 => t('Minute'),
      60 * 60 => t('Hour'),
      60 * 60 * 24 => t('Day'),
      60 * 60 * 24 * 7 => t('Week'),
      60 * 60 * 24 * 30 => t('Month'),
      60 * 60 * 24 * 365 => t('Year'),
    ),
    '#default_value' => variable_get('lyris_simple_flood_window', LYRIS_SIMPLE_FLOOD_DEFAULT_WINDOW),
  );
  $default = variable_get('lyris_simple_flood_message', array());
  $form['flood']['lyris_simple_flood_message'] = array(
    '#type' => 'text_format',
    '#title' => t('Flood Message'),
    '#description' => t('Display this message in place of the form if the user is over the flood limit.'),
    '#rows' => 3,
    '#default_value' => isset($default['value']) ? $default['value'] : '',
    '#format' => isset($default['format']) ? $default['format'] : NULL,
  );

  return system_settings_form($form);
}

