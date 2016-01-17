<?php

/**
 * @file
 * URL plugin.
 *
 * This plugin allows to wrap incoming data into an url.
 *
 * It only works with link_field fields.
 */

/**
 * Callback 'value' for URL plugin.
 */
function opac_plugin_url($record, $mapping) {
  $field_type = $mapping['field_type'];
  $remotefield = $mapping['mapped_with'];
  $function = "_opac_plugin_url_format_$field_type";
  $truncate_function = '_opac_plugin_url_truncate_' . $field_type . '_title';

  $return = array('und' => array());
  if (isset($record[$remotefield])) {
    foreach ($record[$remotefield] as $value) {
      $url = _opac_plugin_url_replace_token($mapping['url_string'], $value);
      $title = _opac_plugin_url_replace_token($mapping['url_title'], $value);
      $return['und'][] = call_user_func(
        $function,
        $url,
        call_user_func($truncate_function, $mapping, $title)
      );
    }
  }
  return $return;
}

/**
 * Callback 'available' for URL plugin.
 */
function opac_plugin_url_available($server, $field) {
  if (in_array($field['type'], _opac_plugin_url_allowed_field_types())) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Callback 'form' for URL plugin.
 */
function opac_plugin_url_form(&$form_state, $field_settings, $mapping) {
  $field_type = $field_settings['type'];
  $information = "Here you have to set an url and title that will produce a link field. You have access to a !value token wich is the value in the ILS field. If there is several values, make sure the node field is multivaluable.";
  $form['url_rules'] = array(
    '#type' => 'fieldset',
    '#title' => check_plain(t("Url plugin form settings")),
  );
  $form['url_rules']['informations'] = array(
    '#markup' => t($information),
  );
  $form['field_type'] = array('#type' => 'hidden', '#value' => $field_type);
  $url_string = isset($mapping['mapped'][0]['url_string']) ? $mapping['mapped'][0]['url_string'] : 'search/node/!value';
  $form['url_rules']['url_string'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#description' => t("Enter the url for this field. '!value' is the value in remote field. I.e. 'http://foo.com?q=!value' for external url or 'foo/!value' for local url."),
    '#default_value' => $url_string,
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $url_title = isset($mapping['mapped'][0]['url_title']) ? $mapping['mapped'][0]['url_title'] : '!value';
  $form['url_rules']['url_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t("Enter the title for this field. !value' is the value in remote field. I.e. 'My !value'."),
    '#default_value' => $url_title,
    '#size' => 15,
    '#maxlength' => 128,
  );
  return $form;
}

/**
 * Return allowed field types for URL plugin.
 */
function _opac_plugin_url_allowed_field_types() {
  return array('link_field');
}

/**
 * Return field content for 'link_field' type.
 */
function _opac_plugin_url_format_link_field($url, $title) {
  return array(
    'url' => $url,
    'title' => $title,
    'html' => TRUE,
  );
}

/**
 * Replace token '!value' with $value in $string.
 */
function _opac_plugin_url_replace_token($string, $value) {
  return preg_replace("/!value/", $value, $string);
}

/**
 * Truncate title string according to the field setting.
 */
function _opac_plugin_url_truncate_link_field_title($mapping, $value) {
  $field_info = field_info_field($mapping['node_field_name']);
  $max_length = isset($field_info['settings']['title_maxlength']) ? $field_info['settings']['title_maxlength'] : '';
  $title = $max_length ? drupal_substr($value, 0, $max_length) : $value;
  return $title;
}
