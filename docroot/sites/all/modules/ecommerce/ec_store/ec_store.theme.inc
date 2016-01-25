<?php

/**
 * @file
 * Stored themes for ec_store module.
 */

/**
 * Format an address with carriage returns or HTML line breaks.
 *
 * @param $txn
 *   Object, User's address information.
 * @param $type
 *   String, Is this a shipping or billing address.
 * @param $break
 *   String, Should we break using '\n' (text) or '<br />' (html).
 */
function theme_ec_store_format_address($variables) {
  $txn = $variables['txn'];
  $type = $variables['type'];
  $break = $variables['html'];
  if (empty($txn->address[$type])) {
    return FALSE;
  }
  $data = clone (object) $txn->address[$type];

  // Cleanup values.
  if ($break != 'text') {
    foreach ($data as $key => $value) {
      if (is_scalar($value)) {
        $data->$key = check_plain($value);
      }
    }
  }
  $break = ($break == 'text') ? "\n" : '<br />';

  $address = '';
  if ((!empty($data->firstname) && !empty($data->lastname)) || !empty($data->fullname)) {
    if (!empty($data->firstname) || !empty($data->lastname)) {
      $address .= $data->firstname . ' ' . $data->lastname . $break;
    }
    else {
      $address .= !empty($data->fullname) ? $data->fullname . $break : '';
    }
    $address .= !empty($data->street2) ? $data->street1 . $break . $data->street2 . $break : $data->street1 . $break;
    $address .= drupal_ucfirst($data->city) . ", " . drupal_strtoupper($data->state) . " " . $data->zip . $break;
    $address .= ec_store_get_country($data->country) . $break;
    $address .= $data->phone . $break;
  }

  return $address;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_ec_store_workflow_settings($variables) {
  $elements =& $variables['form'];
  drupal_add_tabledrag('ec-workflow', 'order', 'sibling', 'ec-workflow-weight');

  $output = '';
  $header = array(t('Workflow'), t('Type'), t('Weight'), '');
  $rows = array();

  foreach (element_children($elements['ec_store_workflow']) as $id) {
    $elements['ec_store_workflow'][$id]['weight']['#attributes']['class'][] = 'ec-workflow-weight';
    $rows[] = array(
      'data' => array(
        drupal_render($elements['ec_store_workflow'][$id]['description']),
        drupal_render($elements['ec_store_workflow'][$id]['type']),
        drupal_render($elements['ec_store_workflow'][$id]['weight']),
        drupal_render($elements['ec_store_workflow'][$id]['operations']),
      ),
      'class' => array('draggable'),
    );
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'ec-workflow')));
  $output .= drupal_render_children($elements);

  return $output;
}
