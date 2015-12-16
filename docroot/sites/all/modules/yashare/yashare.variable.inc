<?php
/**
 * @file
 * Variable module integration for the Yandex.Share module.
 */

/**
 * Implements hook_variable_info().
 */
function yashare_variable_info($options = array()) {
  $variables['yashare_block_title'] = array(
    'type' => 'string',
    'title' => t('Yandex.Share block title', array(), $options),
    'description' => t('Title of the Yandex.Share block.', array(), $options),
    'default' => '',
    'localize' => TRUE,
  );
  $variables['yashare_popup_title'] = array(
    'type' => 'string',
    'title' => t('Yandex.Share popup title', array(), $options),
    'description' => t('Title of the Yandex.Share popup.', array(), $options),
    'default' => '',
    'localize' => TRUE,
  );
  return $variables;
}
