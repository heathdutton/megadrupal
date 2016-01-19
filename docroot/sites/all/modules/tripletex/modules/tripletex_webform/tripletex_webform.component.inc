<?php

/**
 * @file
 * Webform module invoice component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_invoice() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array(
      'pfid' => NULL, // Pay form ID.
      'pmid' => 'invoice', // Pay method ID.
      'price_components' => array(),
      'other_components' => array(
        'mail' => '',
        'first_name' => '',
        'last_name' => '',
        'billto' => '<none>',
        'street1' => '<none>',
        'street2' => '<none>',
        'city' => '<none>',
        'state' => '<none>',
        'zip' => '<none>',
        'country' => '<none>',
        'phone' => '<none>',
      ),
      'fieldset' => 1,
      'title_display' => 0,
  		'private' => TRUE,
      'attributes' => array(),
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_invoice() {
  return array(
      'webform_display_invoice' => array(
        'render element' => 'element',
      ),
    );

  return array(
    'webform_display_invoice' => array(
      'arguments' => array('element' => NULL),
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_invoice($component) {
//  pay_load_handler('pay_form', 'pay_form');

  $form = array();
  $form['value'] = array(
    '#type' => 'value',
    '#value' => '',
  );
  $form['extra']['description'] = array(
    '#type' => 'value',
    '#value' => '',
  );

  $options = array();
  $options['invoice'] = 'invoice';

  $form['extra']['pfid'] = array(
    '#type' => 'value',
    '#value' => $component['extra']['pfid'],
  );

  $form['extra']['pmid'] = array(
    '#title' => t('Payment method'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $component['extra']['pmid'],
    '#required' => TRUE,
  );

  if (empty($options)) {
    $message = t('No payment methods are available. At least <a href="!url">one payment method</a> must be added before payment information is accepted.', array('!url' => url('admin/settings/pay')));
    drupal_set_message($message, 'error');
    $form['extra']['handler']['#options'][] = t('No methods available');
    $form['extra']['handler']['#disabled'] = TRUE;
    $form['extra']['handler']['#description'] = $message;
  }

  $node = node_load($component['nid']);
  $price_components = webform_component_list($node, 'price', FALSE, FALSE);
  $form['extra']['price_components'] = array(
    '#type' => 'select',
    '#title' => t('Price components'),
    '#options' => $price_components,
    '#default_value' => $component['extra']['price_components'],
    '#multiple' => TRUE,
    '#size' => 10,
    '#description' => t('Select the components that contain price values. The total value of these components will be charged to the user.'),
    '#process' => array('webform_component_select'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#all_checkbox' => FALSE,
    '#required' => TRUE,
  );

  $email_components = webform_component_list($node, 'email_address', TRUE, TRUE);
  $name_components = webform_component_list($node, 'email_name', TRUE, TRUE);
  $address_components = webform_component_list($node, 'address', TRUE, TRUE);
  $bill_components = $name_components;

  $built_in = array('' => t('Use built-in field'));
  $do_not_collect = array('<none>' => t('Do not collect'));
  $separate_fields = array('<none>' => t('Use individual fields'));

  $form['extra']['other_components'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other component mappings'),
    '#description' => t('Additional information may also be sent to the payment provider. Note that not all fields may be supported by all payment systems.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['extra']['other_components']['mail'] = array(
    '#type' => 'select',
    '#title' => t('E-mail'),
    '#options' => $built_in + $do_not_collect + $email_components,
    '#default_value' => $component['extra']['other_components']['mail'],
  );
  $form['extra']['other_components']['first_name'] = array(
    '#type' => 'select',
    '#title' => t('First name'),
    '#options' => $built_in + $do_not_collect + $name_components,
    '#default_value' => $component['extra']['other_components']['first_name'],
  );
  $form['extra']['other_components']['last_name'] = array(
    '#type' => 'select',
    '#title' => t('Last name'),
    '#options' => $built_in + $do_not_collect + $name_components,
    '#default_value' => $component['extra']['other_components']['last_name'],
  );
  $form['extra']['other_components']['billto'] = array(
    '#type' => 'select',
    '#title' => t('Billing address'),
    '#options' => $separate_fields + $address_components,
    '#default_value' => $component['extra']['other_components']['billto'],
    '#access' => !empty($address_components),
    '#description' => t('A combined address field may be used if you have a dedicated address component available. If used, do not specify any of the fields below.'),
  );
  $form['extra']['other_components']['street1'] = array(
    '#type' => 'select',
    '#title' => t('Street 1'),
    '#options' => $do_not_collect + $bill_components,
    '#default_value' => $component['extra']['other_components']['street1'],
  );
  $form['extra']['other_components']['street2'] = array(
    '#type' => 'select',
    '#title' => t('Street 2'),
    '#options' => $do_not_collect + $bill_components,
    '#default_value' => $component['extra']['other_components']['street1'],
    '#description' => t('Note that Street 2 may not be recorded by some payment gateways.'),
  );
  $form['extra']['other_components']['city'] = array(
    '#type' => 'select',
    '#title' => t('City'),
    '#options' => $do_not_collect + $bill_components,
    '#default_value' => $component['extra']['other_components']['city'],
  );
  $form['extra']['other_components']['state'] = array(
    '#type' => 'select',
    '#title' => t('State/Province'),
    '#options' => $do_not_collect + $bill_components,
    '#default_value' => $component['extra']['other_components']['state'],
  );
  $form['extra']['other_components']['zip'] = array(
    '#type' => 'select',
    '#title' => t('Postal code'),
    '#options' => $do_not_collect + $bill_components,
    '#default_value' => $component['extra']['other_components']['zip'],
  );
  $form['extra']['other_components']['country'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#options' => $do_not_collect + $bill_components,
    '#default_value' => $component['extra']['other_components']['country'],
  );
  $form['extra']['other_components']['phone'] = array(
    '#type' => 'select',
    '#title' => t('Phone'),
    '#options' => $do_not_collect + $bill_components,
    '#default_value' => $component['extra']['other_components']['phone'],
  );

  $form['display']['fieldset'] = array(
    '#title' => t('Show payment label as fieldset'),
    '#type' => 'checkbox',
    '#default_value' => $component['extra']['fieldset'],
    '#description' => t('Payment information includes multiple fields which you may want to visually group together. The label will be used as the fieldset title.'),
    '#parents' => array('extra', 'fieldset'),
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 *
 * This function makes sure the invoice component is displayed on the webform.
 *
 */
function _webform_render_invoice($component, $value = NULL, $filter = TRUE) {

//  dpm($component, 'component');
  // This is a merely a placeholder element, since pay forms need to be located
  // at a specific location in the form. This will be populated with the actual
  // form in webform_pay_prerender().
  $element = array(
    '#value' => $value,
    '#weight' => $component['weight'],
    '#webform_component' => $component,
  );

  if ($component['extra']['fieldset']) {
    $element['#type'] = 'fieldset';
    $element['#title'] = $filter ? _webform_filter_xss($component['name']) : $component['name'];
    $element['#title_display'] = $component['extra']['title_display'] ? $component['extra']['title_display'] : NULL;
    $element['#attributes'] = $component['extra']['attributes'];
    $element['#pre_render'] = array('webform_element_title_display', '_tripletex_webform_fieldset_prerender');
    $element['#default_value'] = $filter ? _webform_filter_values($component['value']) : $component['value'];
    $element['#prefix'] = '<div class="webform-component-' . $component['type'] . '" id="webform-component-' . $component['form_key'] . '">';
    $element['#suffix'] = '</div>';
  }
  else {
    // Wrap in the normal component wrapper if not a fieldset.
    $element['#theme_wrappers'] = array('webform_element_wrapper');
    $element['#post_render'] = array('webform_element_wrapper');
  }

  return $element;
}

/**
 * Pre-render function to set a fieldset ID.
 */
function _tripletex_webform_fieldset_prerender($element) {
  $element['#attributes']['id'] = 'webform-component-' . str_replace('_', '-', implode('--', array_slice($element['#parents'], 1)));
  return $element;
}

/**
 * Implements _webform_submit_component().
 */
function _webform_submit_invoice($component, $value) {

  // Only keep the following values to prevent any accidental, insecure storage.
  $value = array_intersect_key($value, drupal_map_assoc(array(
    'pmid',
    'payment_type',
    'first_name',
    'last_name',
    'total',
  )));

  return $value;
}

/**
 * Display the result of a submission for a component.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Submissions". This should output the saved data in some reasonable manner.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database table schema.
 * @param $format
 *   Either 'html' or 'text'. Defines the format that the content should be
 *   returned as. Make sure that returned content is run through check_plain()
 *   or other filtering functions when returning HTML.
 * @return
 *   A renderable element containing at the very least these properties:
 *    - #title
 *    - #weight
 *    - #component
 *    - #format
 *    - #value
 *   Webform also uses #theme_wrappers to output the end result to the user,
 *   which will properly format the label and content for use within an e-mail
 *   (such as wrapping the text) or as HTML (ensuring consistent output).
 */
/**
 * Implements _webform_display_component().
 */
function _webform_display_invoice($component, $value, $format = 'html') {

  return array(
      '#title' => $component['name'],
      '#weight' => $component['weight'],
//      '#theme' => 'webform_display_invoice',
    	'#theme' => 'webform_display_textfield',
  		'#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
      '#post_render' => array('webform_element_wrapper'),
      '#component' => $component,
    	'#pmid' => $component['extra']['pmid'],
  		'#format' => $format,
      '#value' => isset($value[0]) ? $value[0] : '',
  );

}

/**
 * Format the output of data for this component.
 */
function theme_webform_display_invoice($element) {

  $output = array();
  $element = array_pop($element);

  $value = $element['#value'];
  dpm($element);

  if (!empty($value['first_name']) || !empty($value['last_name'])) {
    $name = implode(' ', array($value['first_name'], $value['last_name']));
    $output[] = ($element['#format'] == 'html') ? check_plain($name) : $name;
  }

  if ($value['total']) {
    $output[] = t('Total: @amount', array('@amount' => sprintf("%01.2f", $value['total'])));
  }

  if (!empty($output)) {
    $glue = ($element['#format'] == 'html') ? '<br />' : "\n";
    $output = implode($glue, $output);
  }
  else {
    $output = t('No payment information provided');
  }
//  dpm($output);
  return $output;
}

/**
* Calculate and returns statistics about results for this component.
*
* This takes into account all submissions to this webform. The output of this
* function will be displayed under the "Results" tab then "Analysis".
*
* @param $component
*   An array of information describing the component, directly correlating to
*   the webform_component database schema.
* @param $sids
*   An optional array of submission IDs (sid). If supplied, the analysis will
*   be limited to these sids.
* @param $single
*   Boolean flag determining if the details about a single component are being
*   shown. May be used to provided detailed information about a single
*   component's analysis, such as showing "Other" options within a select list.
* @return
*   An array of data rows, each containing a statistic for this component's
*   submissions.
*/
function _webform_analysis_invoice($component, $sids = array(), $single = FALSE) {

  $submissions = 0;
  $payments = 0;
  $payment_amount = 0;

  // Generate a lookup table of results.
  $query = db_select('webform_submitted_data', 'wsd')
  ->fields('wsd', array('no', 'data'))
  ->condition('nid', $component['nid'])
  ->condition('cid', $component['cid'])
  ->condition('data', '', '<>')
  ->groupBy('no')
  ->groupBy('data');
  $query->addExpression('COUNT(sid)', 'datacount');

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $result = $query->execute();
  $counts = array();
  foreach ($result as $data) {
    if ($data->data > 0) {
      $payments++;
      $payment_amount += (float) $data->data;
    }
    $submissions++;
  }

  $payment_average = $payments ? number_format($payment_amount / $payments, 2) : 0;

  $rows = array();
  $rows[0] = array(t('No payment'), ($submissions - $payments));
  $rows[1] = array(t('Payment received'), $payments);
//  $rows[2] = array(t('Average amount'), theme('pay_money', $payment_average, 'NOK'));
  return $rows;

  // Create an entire table to be put into the returned row.
  $rows = array();
  $header = array('');

  // Add options as a header row.
  foreach ($options as $option) {
    $header[] = $option;
  }

  // Add questions as each row.
  foreach ($questions as $qkey => $question) {
    $row = array($question);
    foreach ($options as $okey => $option) {
      $row[] = !empty($counts[$qkey][$okey]) ? $counts[$qkey][$okey] : 0;
    }
    $rows[] = $row;
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('webform-grid'))));

  return array(array(array('data' => $output, 'colspan' => 2)));
}

/**
 * Implements _webform_analysis_component().
 *
function _old_webform_analysis_invoice($component, $sids = array()) {
  $submissions = 0;
  $payments = 0;
  $payment_amount = 0;

  $placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
  $sidfilter = count($sids) ? " AND sid in (" . implode(",", $placeholders) . ")" : "";
  $query = 'SELECT data' .
    ' FROM {webform_submitted_data}' .
    " WHERE nid = %d AND cid = %d AND no = 'total'" . $sidfilter;

  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  while ($data = db_fetch_array($result)) {
    if ($data['data'] > 0) {
      $payments++;
      $payment_amount += (float) $data['data'];
    }
    $submissions++;
  }

  $payment_average = $payments ? number_format($payment_amount / $payments, 2) : 0;

  $rows[0] = array(t('No payment'), ($submissions - $payments));
  $rows[1] = array(t('Payment received'), $payments);
  $rows[2] = array(t('Average amount'), theme('pay_money', $payment_average, 'USD'));
  return $rows;
}
*/


/**
 * Implements _webform_table_component().
 */
/**
* Return the result of a component value for display in a table.
*
* The output of this function will be displayed under the "Results" tab then
* "Table".
*
* @param $component
*   A Webform component array.
* @param $value
*   An array of information containing the submission result, directly
*   correlating to the webform_submitted_data database schema.
* @return
*   Textual output formatted for human reading.
*/
function _webform_table_invoice($component, $value) {

  $invoice_id = '-';
  $status = t('open');

  if (isset($value['invid'])) {
    $invoice_id = $value['invid'];
    if (_tripletex_get_invoice_balance($invoice_id) < 1) {
      $status = t('settled');
    }
  }
  else {
    $status = '';
  }

  return '#' .  check_plain($invoice_id) . '<br />'. check_plain(number_format($value['total'], 2, ',', '.') ). '<br />'. $status;
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_invoice($component, $export_options) {
  $header = array();
  $header[0] = array_fill(0, 6, '');
  $header[1] = $header[0];
  $header[1][0] = $component['name'];
  $header[2] = array(
    t('First name'),
    t('Last name'),
    t('Total'),
  );
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_invoice($component, $export_options, $value) {
  return array(
    $value['first_name'],
    $value['last_name'],
    $value['total'],
  );
}
