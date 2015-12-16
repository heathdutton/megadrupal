<?php
/**
 * @file
 * Webform module sermepa component.
 */

use CommerceRedsys\Payment\Sermepa;

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_sermepa() {
  return array(
    'value' => '',
    'extra' => array(
      'private' => 1,
      'sermepa' => array(
        'transaction' => 'live',
      ),
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_sermepa() {
  return array(
    'webform_display_sermepa' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Generate the form for editing a component.
 */
function _webform_edit_sermepa($component) {
  $library = libraries_load('sermepa');

  $form = array();
  $form['extra']['sermepa'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sermepa Settings'),
  );

  if (user_access('use sermepa testing')) {
    $form['extra']['sermepa']['transaction'] = array(
      '#type' => 'select',
      '#title' => t('Mode of the transactions.'),
      '#options' => array(
        'live' => t('Live Transaction'), 
        'test' => t('Testing')
      ),
      '#default_value' => $component['extra']['sermepa']['transaction'],
    );
  }

  $components_values = $component['extra']['sermepa'];
  $fuc = empty($components_values['fuc']) ? '' : $components_values['fuc'];
  $merchant_name = empty($components_values['merchant_name']) ? '' : $components_values['merchant_name'];
  $product_description = empty($components_values['product_description']) ? '' : $components_values['product_description'];
  $terminal = empty($components_values['terminal']) ? '' : $components_values['terminal'];
  $key = empty($components_values['key']) ? '' : $components_values['key'];

  $language = empty($components_values['language']) ? key($language_options) : $components_values['language'];
  $currency = empty($components_values['currency']) ? key($currency_options) : $components_values['currency'];
  $encryption = empty($components_values['encryption']) ? key($currency_options) : $components_values['encryption'];
  $fixed_amount = empty($components_values['fixed_amount']) ? 0 : $components_values['fixed_amount'];
  $component_amount = empty($components_values['component_amount']) ? '' : $components_values['component_amount'];
  $component_validation = empty($components_values['component_validation']) ? '' : $components_values['component_validation'];
  $input_amount = empty($components_values['input_amount']) ? '' : $components_values['input_amount'];

  $url_ok = empty($components_values['url_ok']) ? '' : $components_values['url_ok'];
  $url_ko = empty($components_values['url_ko']) ? '' : $components_values['url_ko'];

  $form['extra']['sermepa']['fuc'] = array(
    '#type' => 'textfield',
    '#title' => t('Código FUC / Merchant Code'),
    '#size' => 80,
    '#maxlength' => Sermepa::getMerchantCodeMaxLength(),
    '#required' => TRUE,
    '#default_value' => $fuc,
  );
  $form['extra']['sermepa']['merchant_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant Name'),
    '#required' => TRUE,
    '#size' => 80,
    '#maxlength' => Sermepa::getMerchantNameMaxLength(),
    '#default_value' => $merchant_name,
  );
  $form['extra']['sermepa']['product_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Product description'),
    '#required' => TRUE,
    '#default_value' => $product_description,
  );
  $form['extra']['sermepa']['terminal'] = array(
    '#type' => 'textfield',
    '#title' => t('Terminal number'),
    '#size' => 5,
    '#maxlength' => Sermepa::getMerchantTerminalMaxLength(),
    '#required' => TRUE,
    '#default_value' => $terminal,
  );
  $form['extra']['sermepa']['key'] = array(
    '#type' => 'textfield',
    '#title' => t('SHA256 key'),
    '#size' => 80,
    '#maxlength' => Sermepa::getMerchantPasswordMaxLength(),
    '#required' => TRUE,
    '#default_value' => $key,
  );
  $form['extra']['sermepa']['language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#options' => Sermepa::getAvailableConsumerLanguages(),
    '#default_value' => $language,
  );
  $form['extra']['sermepa']['currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#options' => Sermepa::getAvailableCurrencies(),
    '#default_value' => $currency,
  );
  $form['extra']['sermepa']['url_ok'] = array(
    '#type' => 'textfield',
    '#title' => t('Url transaction Ok'),
    '#default_value' => $url_ok,
  );
  $form['extra']['sermepa']['url_ko'] = array(
    '#type' => 'textfield',
    '#title' => t('Url transaction Fail'),
    '#default_value' => $url_ko,
  );

  $form['extra']['sermepa']['fixed_amount'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enter a fixed price.'),
    '#description' => t('Price value input'),
    '#required' => FALSE,
    '#default_value' => $fixed_amount,
  );
  $node = node_load($component['nid']);
  $options = _webform_sermepa_filter_node_components($node, array('textfield'));
  $form['extra']['sermepa']['component_amount'] = array(
    '#type' => 'select',
    '#title' => t('Price value component'),
    '#options' => $options,
    '#description' => t('Choose the input text component with the price value.'),
    '#states' => array(
      "visible" => array(
        ":input[name='extra[sermepa][fixed_amount]']" => array("checked" => FALSE),
      ),
      "enabled" => array(
        ":input[name='extra[sermepa][fixed_amount]']" => array("checked" => FALSE),
      ),
    ),
    '#default_value' => $component_amount,
  );

  if (user_access('use sermepa php')) {
    $form['extra']['sermepa']['component_validation'] = array(
      '#type' => 'textarea',
      '#title' => t('Component validation'),
      '#description' => t('Enter PHP code to validate selected text component. This code will be added to $form["#validate"] array. <strong> Do not use &lt;?php ?&gt; delimiters.</strong><br /><em>$form</em> and <em>$form_state</em> arrays are availables. If the form is not valid you must invoke form_set_error() function.'),
      '#states' => array(
        "visible" => array(
          ":input[name='extra[sermepa][fixed_amount]']" => array("checked" => FALSE),
        ),
      ),
      '#default_value' => $component_validation,
    );
  }

  $form['extra']['sermepa']['input_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter the first amount value in cents of €'),
    '#description' => t('Price value'),
    '#required' => FALSE,
    '#size' => 12,
    '#states' => array(
      "visible" => array(
        ":input[name='extra[sermepa][fixed_amount]']" => array("checked" => TRUE),
      ),
      "enabled" => array(
        ":input[name='extra[sermepa][fixed_amount]']" => array("checked" => TRUE),
      ),
    ),
    '#default_value' => $input_amount,
  );

  return $form;
}

/**
 * Element validate handler.
 */
function _webform_edit_sermepa_validate($form, &$form_state) {
  $values = $form_state['values']['extra']['sermepa'];
  if (!empty($values['fixed_amount'])) {
    $amount = _webform_sermepa_format_amount($values['input_amount'], $values['currency']);
    form_set_value($form['extra']['sermepa']['input_amount'], $amount, $form_state);
  }
  if (!preg_match("/^[0-9]{9}$/", $values['fuc'])) {
    form_set_error('webform_sermepa', t('Please enter a valid commerce code'));
  }
  if (!preg_match("/^[0-9]{1,3}$/", $values['terminal'])) {
    form_set_error('webform_sermepa', t('Please enter a valid terminal number'));
  }
  if (!preg_match("/^[a-zA-Z0-9]{20}$/", $values['key'])) {
    form_set_error('webform_sermepa', t('Please enter a valid secret key'));
  }
}

/**
 * Render a Webform component to be part of a form.
 */
function _webform_render_sermepa($component, $value = NULL, $filter = TRUE) {
  $item = menu_get_item();
  $tpv = FALSE;
  if ($value['status'] == 'paid') {
    $status = 1;
  }
  elseif ($value['status'] == 'paid_tpv') {
    $tpv = TRUE;
    $status = -1;
  }
  else {
    $status = 0;
  }
  $element = array(
    '#weight' => $component['weight'],
  );
  if (strpos($item['path'], 'edit') && user_access('update payment status')) {
    if ($tpv) {
      $element['order'] = array(
        '#type' => 'hidden',
        '#default_value' => $value['order'],
        '#theme_wrappers' => array('webform_element'),
        '#webform_component' => $component,
      );
      $element['amount'] = array(
        '#type' => 'hidden',
        '#default_value' => $value['amount'],
        '#theme_wrappers' => array('webform_element'),
        '#webform_component' => $component,
      );
      $element['status'] = array(
        '#type' => 'hidden',
        '#default_value' => $status,
        '#theme_wrappers' => array('webform_element'),
        '#webform_component' => $component,
      );
    }
    else {
      $element['status'] = array(
        '#title' => t('Mark manually status as Paid'),
        '#weight' => $component['weight'],
        '#type' => 'checkbox',
        '#theme_wrappers' => array('webform_element'),
        '#webform_component' => $component,
        '#default_value' => $status,
      );
    }
  }
  else {
    $element['status'] = array(
      '#weight' => $component['weight'],
      '#type' => 'hidden',
      '#default_value' => $status,
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => $component,
    );
  }
  return $element;
}

/**
 * A hook for changing the input values before saving to the database.
 */
function _webform_submit_sermepa($component, $value) {
  $return = array();
  $return['status'] = 'unpaid';
  $return['order'] = empty($value['order']) ? '' : $value['order'];
  $return['amount'] = empty($value['amount']) ? '' : $value['amount'];
  if (intval($value['status']) == 1) {
    $return['status'] = 'paid';
  }
  elseif (intval($value['status']) == -1) {
    $return['status'] = 'paid_tpv';
  }
  return $return;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_sermepa($component, $value) {
  if (!empty($value['status'])) {
    if ($value['status'] == 'paid_tpv') {
      $value = t('Submitted to sermepa - Payment Confirmed TPV (Order: @order, Amount: @amount).', array('@order' => $value['order'], '@amount' => $value['amount']));
    }
    elseif ($value['status'] == 'paid') {
      $value = t('Submitted to sermepa - Payment confirmed manually.');
    }
    else {
      $value = t('Submitted to sermepa');
    }
  }
  return array(
    '#title' => t('Sermepa status'),
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_sermepa',
    '#theme_wrappers' => array('webform_element'),
    '#value' => $value,
    '#webform_component' => $component,
  );
}

/**
 * Format the output of data for this component.
 */
function theme_webform_display_sermepa($variables) {
  $element = $variables['element'];
  if (empty($element['#value'])) {
    return;
  }
  else {
    return $element['#value'];
  }
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_sermepa($component, $value) {
  if (empty($value['status'])) {
    return;
  }
  else {
    return ucfirst($value['status']);
  }
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_sermepa($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_sermepa($component, $export_options, $value) {
  if (empty($value['status'])) {
    return;
  }
  else {
    return ucfirst($value['status']);
  }
}
