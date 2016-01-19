<?php
/**
 * @file * Admin include file. 
*/

/** 
 * Displays the form for the standard settings tab.
 * 
 * @return array 
 * A structured array for use with Forms API. 
*/ 
function intel_hubspot_admin_config($form, $form_state) {
  //$form = array_merge($form, intel_hubspot_admin_config_form($form, $form_state));
  $form = drupal_get_form('intel_hubspot_admin_config_form');
  //return system_settings_form($form);
  return $form;
}

function intel_hubspot_admin_config_page() {
  //$form = array_merge($form, intel_hubspot_admin_config_form($form, $form_state));
  $form = drupal_get_form('intel_hubspot_admin_config_form');
  //return system_settings_form($form);
  return render($form);
}

function intel_hubspot_admin_config_form($form, $form_state) {
  $form = array();

  $api_connection = 0;
  if (count($form_state['input']) == 0) { // hack to assure check and messages are only done once when form is submitted
    if ($HAPIKey = variable_get('intel_hubspot_api_key', '')) {
      intel_hubspot_include_library_file('class.settings.php');
      intel_hubspot_include_library_file('class.exception.php');
      $settings_api = new HubSpot_Settings($HAPIKey);
      $settings = $settings_api->get_settings();
      if (!empty($settings) && is_array($settings)) {
        foreach ($settings AS $i => $setting) {
          if ($setting->name == 'contactsEnabled') {
            if ($setting->value == 'true') {
              $api_connection = 1;
            }
          }
        }
      }
    }
    if ($api_connection) {
      drupal_set_message(t('HubSpot API connected.'));
    }
    else {
      $msg = t('HubSpot API not connected.');
      drupal_set_message($msg, 'error');
    }
  }
  
  $form['intel_hubspot_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#default_value' => variable_get('intel_hubspot_api_key', ''),
    '#description' => t('Enter your Api key from HubSpot.'),
  );  
  
  $form['settings']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update settings'),
  );
  
  return $form;
}

function intel_hubspot_admin_config_form_submit($form, $form_state) {
  $values = $form_state['values'];
  
  variable_set('intel_hubspot_api_key', $values['intel_hubspot_api_key']);
}

function intel_mailchimp_contact($visitor, $action = '') {
  $visitor_name = (!empty($visitor->name)) ? $visitor->name : '[No Name]';
  drupal_set_title(t('MailChimp contact: @title', array('@type' => $visitor_name, '@title' => $visitor_name)), PASS_THROUGH);
  
  $output = '';  
  
  if (!empty($_GET['debug'])) {
    dpm($visitor->getProperties());//
  }
  $ext_data = $visitor->ext_data;
  if (($action == 'resync')
      && (!empty($ext_data['hubspot']['utk']) || !empty($ext_data['hubspot']['lastsubmission_utk']))
    ) {
    intel_hubspot_intel_sync_visitordata($visitor);
    $visitor->load();
    $ext_data = $visitor->ext_data;
    drupal_set_message(t('HubSpot data resynced.'));
    drupal_goto('visitor/' . $visitor->vtk . '/hubspot');
  }
  elseif (empty($ext_data['hubspot']['profile_url']) 
      && (!empty($ext_data['hubspot']['utk']) || !empty($ext_data['hubspot']['lastsubmission_utk']))
    ) {
    intel_hubspot_intel_sync_visitordata($visitor);
    $visitor->load();
    $ext_data = $visitor->ext_data;
    drupal_set_message(t('HubSpot data resynced.'));
  }
  
  if (empty($ext_data['hubspot']['profile_url'])) {
    drupal_set_message('This contact has not been linked to a HubSpot contact.', 'error');
    $output = 'Unable to fetch contact from HubSpot.';
    return $output;
  }
  
  if ($action == 'launch') {
    //$script = "window.open('{$visitor->ext_data['hubspot']['profile_url']}', 'hubspot');";
    //drupal_add_js($script, array('type' => 'inline', 'scope' => 'header'));
    drupal_goto($ext_data['hubspot']['profile_url']);
  }

  $vars = array(
    'src' => $ext_data['hubspot']['profile_url'],
    'id' => 'hubspot-page',
  );
  //$output .= theme_intel_iframe($vars);
  
  //return $output;
  //drupal_add_js($script, array('type' => 'inline', 'scope' => 'header'));
  

$script = <<<EOD
jQuery('#hubspot-iframe').load(function() {
  alert('hi');
  this.style.height = this.contentWindow.document.body.offsetHeight + 'px';
  alert(this.style.height);
});
EOD;
  $output .= '<style>iframe #hubspot-iframe { visibility:hidden; }</style>';
  //drupal_add_js($script, array('type' => 'inline', 'scope' => 'header'));
  $output .= '<iframe src="' . $ext_data['hubspot']['profile_url'] . '" id="hubspot-iframe" scrolling="no" frameborder="0" style="width: 100%; height: 2500px;"></iframe>';
  drupal_add_js($script, array('type' => 'inline', 'scope' => 'header'));
  
  
  return $output;
}