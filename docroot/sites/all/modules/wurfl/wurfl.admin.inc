<?php
/**
 * @file
 * Configuration file for the WURFL module
 */

function wurfl_check() {
  global $wurfl_manager;
  if (! ($wurfl_manager instanceof WURFL_WURFLManager))  {
    drupal_set_message(t('One or more problems were detected with your WURFL setup. Check the <a href="@status">status report</a> for more information.', array('@status' => url('admin/reports/status'))), 'error');
  }
}

function wurfl_admin() {

  wurfl_check();

  $form = array();

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('WURFL CONFIGURATION'),
    '#weight' => 5,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['options']['wurfl_cache_ttl'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache lifetime'),
    '#description' => t("Set the time to live for an element before it expires (seconds). Set 0 to never expires."),
    '#default_value' => variable_get('wurfl_cache_ttl', WURFL_CACHE_TTL),
    '#element_validate' => array('element_validate_integer_positive'),
    '#required' => TRUE,
  );

  $options = array('accuracy' => t('High Accuracy'), 'performance' => t('High Performance'));

  $form['options']['wurfl_match_mode'] = array(
    '#type' => 'select',
    '#title' => t('Match mode'),
    '#description' => t("Set the match mode for the API."),
    '#options' => $options,
    '#default_value' => variable_get('wurfl_match_mode', WURFL_DEFAULT_MATCH_MODE)
  );

  $form['options']['wurfl_allow_reload'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow reload'),
    '#description' => t("Update the persistence data automatically without any configuration by checking the modification time of the WURFL file."),
    '#default_value' => variable_get('wurfl_allow_reload', WURFL_DEFAULT_ALLOW_RELOAD)
  );

  return system_settings_form($form);
}

function wurfl_test_page() {

  wurfl_check();

  global $wurfl_manager;

  $output = '';

  if (! ($wurfl_manager instanceof WURFL_WURFLManager))  {
    return $output;
  }

  $output .= t('Access this page with your mobile device, or use the !switcher module to test different user agents', array('!switcher' => l( 'Firefox Agent Switcher extension', 'https://addons.mozilla.org/en-US/firefox/addon/user-agent-switcher/')));
  $requesting_device = wurfl_get_requestingDevice();
  $output .= '<h2>' . t('Device information') . '</h2>';
  $output .= t('This is a short collection of parameters that can be retrieved using WURFL');
  $output .= 'Requesting Browser User Agent: <br/><b> ' . $_SERVER["HTTP_USER_AGENT"] . '</b>';

  $output .= '<h2>product_info</h2>';
  $output .= '<ul>';
  $output .= '<li>Is Wireless Device: ' . $requesting_device->getCapability("is_wireless_device") . '</li>';
  $output .= '<li>ID: ' . $requesting_device->id  . '</li>';
  $output .= '<li>Brand Name: ' . $requesting_device->getCapability("brand_name")  . '</li>';
  $output .= '<li>Model Name: ' . $requesting_device->getCapability("model_name")  . '</li>';
  $output .= '</ul>';
  $output .= '<h2>display</h2>';
  $output .= '<ul>';
  $output .= '<li>Resolution Width: ' . $requesting_device->getCapability("resolution_width")  . '</li>';
  $output .= '<li>Resolution Height: ' . $requesting_device->getCapability("resolution_height")  . '</li>';
  $output .= '</ul>';
  $output .= '<h2>Ajax</h2>';
  $output .= '<ul>';
  $output .= '<li>Ajax/Javascript support: ' . $requesting_device->getCapability("ajax_support_javascript") . '</li>';
  $output .= '<li>Mobile Browser: ' . (bool) $requesting_device->getCapability("mobile_browser") . '</li>';
  $output .= '</ul>';
  $output .= '<h2>' . t('wml_ui') . '</h2>';
  $output .= '<ul>';
  $output .= '<li>Prefix for phonecall: ' . $requesting_device->getCapability("wml_make_phone_call_string") . '</li>';
  $output .= '</ul>';
  $output .= '<h2>' . t('xhtml_ui') . '</h2>';
  $output .= '<ul>';
  $output .= '<li>Prefix for phonecall: ' . $requesting_device->getCapability("wml_make_phone_call_string") . '</li>';
  $output .= '<li>Xhtml Preferred Markup: ' . $requesting_device->getCapability("preferred_markup")  . '</li>';
  $output .= '</ul>';
  return $output;
}
