<?php
/**
 * @file
 * Administrative settings for the Marketo Integration
 */


/**
 * Page callback for admin/config/marketo
 */
function marketo_settings_form() {
  $form = array();
  if (module_exists('marketo_munchkin')) {
    $form['marketo_soap'] = array(
      '#type' => 'radios',
      '#title' => t('Marketo API type'),
      '#description' => t('Specify the way you wish data to be submitted to Marketo.'),
      '#options' => array(
        0 => t('Munchkin API'),
        1 => t('SOAP API'),
      ),
      '#default_value' => variable_get('marketo_soap', 1),
    );
  }
  else {
    $form['munchkin'] = array(
      '#markup' => '<em>' . t('Enable the @m module to use the Marketo Munchkin API', array('@m' => 'marketo_munchkin')) . '</em>',
    );
  }
  $form['marketo_soap_endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Marketo SOAP API Endpoint'),
    '#description' => t('The SOAP API Endpoint for your Marketo Instance'),
    '#default_value' => variable_get('marketo_soap_endpoint', 'https://na-k.marketo.com/soap/mktows/1_6'),
  );
  $form['marketo_soap_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Marketo SOAP API Access Key'),
    '#description' => t('The SOAP API Access Key for your Marketo Instance'),
    '#default_value' => variable_get('marketo_soap_access_key', ''),
  );
  $form['marketo_soap_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Marketo SOAP API Secret Key'),
    '#description' => t('The SOAP API Secret Key for your Marketo Instance'),
    '#default_value' => variable_get('marketo_soap_secret_key', ''),
  );
  $form['marketo_soap_proxy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Proxy data to marketo asynchronously.'),
    '#description' => t('The Marketo SOAP API can have very long response times.  Select this option to cause data sent to Marketo to be relayed by an asynchronous service. The details of the SOAP transaction will still be logged, but the page load time for your users will be significantly improved.'),
    '#default_value' => variable_get('marketo_soap_proxy', FALSE),
  );

  $form['default_fields'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#title' => t('Default fields'),
    '#description' => t('Enter data which should be attached to every marketo submission as key-value pairs separated by a pipe (|) character. You may use tokens in the value, e.g !example. Note, if one of these fields is set by another module, it will not be overwritten.',
                        array('!example' => '<code>LeadSource|[current-page:url]</code>')),
  );
  $fields = variable_get('marketo_default_fields', array());
  $lines = array();
  foreach ($fields as $name => $value) {
    $lines[] = $name . '|' . $value;
  }
  $form['default_fields']['marketo_default_fields'] = array(
    '#type' => 'textarea',
    //'#title' => t('Default fields'),
    '#default_value' => implode("\n", $lines),
  );
  if (module_exists('token')) {
    $form['default_fields']['token_help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Available tokens'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 100,
    );
    $form['default_fields']['token_help']['tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('global'),
    );
  }
  $form['#validate'] = array('marketo_settings_form_validate');
  return system_settings_form($form);
}

function marketo_settings_form_validate($form, &$form_state) {
  $fields = array();
  $text = @trim($form_state['values']['marketo_default_fields']);
  if (!empty($text)) {
    $list = explode("\n", $text);
    foreach ($list as $line) {
      @list($name, $value) = explode('|', $line);
      $name = @trim($name);
      $value = @trim($value);
      if (empty($name) || empty($value)) {
        form_set_error('marketo_default_fields', t('You must specify default fields one per line as a name/value pair separated by a pipe (|) character.'));
        return;
      } 
      else {
			  $fields[$name] = trim($value);
      }
    }
  }
  $form_state['values']['marketo_default_fields'] = $fields;
}
