<?php

/**
 * @file
 * Access plugin defining HTTP request header based rules.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("HTTP Request Header"),
  'description' => t('Control access by request header values.'),
  'callback' => 'ctools_header_plugins_request_header_ctools_access_check',
  'settings form' => 'ctools_header_plugins_request_header_ctools_access_settings',
  'summary' => 'ctools_header_plugins_request_header_ctools_access_summary',
  'required context' => new ctools_context_required(t('Header'), 'request_header'),
);

/**
 * Settings form for the 'request header' access plugin
 */
function ctools_header_plugins_request_header_ctools_access_settings($form, &$form_state, $conf) {
  $context = ctools_get_context("request_header");
  if ($function = ctools_plugin_get_function($context, 'convert list')) {
    $converters = (array) $function();

    $form['settings']['header_element'] = array(
      "#type" => "select",
      "#title" => t("Header element"),
      "#options" => $converters,
      "#default_value" => $conf['header_element']
    );

    $form['settings']['header_element_value'] = array(
      "#type" => "textfield",
      "#title" => t("Header element value pattern"),
      "#default_value" => $conf['header_element_value'],
      "#description" => t("Use !regex_link to define a pattern that should be matched for selected header element, i.e. /Mozilla/", array('!regex_link' => l(t('regular expression'), 'http://www.php.net/manual/en/pcre.pattern.php', array('external' => TRUE))))
    );

    return $form;
  }
}

/**
 * Check for access.
 */
function ctools_header_plugins_request_header_ctools_access_check($conf, $context) {
  if (preg_match($conf['header_element_value'], $context->data->$conf['header_element'])) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Provide a summary description based upon the header values.
 */
function ctools_header_plugins_request_header_ctools_access_summary($conf, $context) {
  return t("Matches the pattern on !element header element.", array("!element" => $conf["header_element"]));
}