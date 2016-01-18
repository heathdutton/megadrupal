<?php

/**
 * @file
 * Administration pages for global quotes
 */

/**
 * Configures the store shipping rates
 */
function uc_global_quote_admin($form, $form_state, $id = 0) {
  $form = array();
  $form['rates'] = array(
    '#tree' => TRUE,
  );

  if ($id != 0) {
    $label = 'Edit quote';
  }
  else {
    $label = 'Add a new quote';
    
    $edit = new stdClass();
    $edit->id = NULL;
    $edit->zid = NULL;
    $edit->qid = NULL;
    $edit->min = 0;
    $edit->max = 0;
    $edit->rate = 0.00;
    $edit->type = NULL;
  }

  $result = db_query("SELECT * FROM {uc_global_quote}")->fetchAll();

  foreach ($result as $r) {
    $qid = $r->qid;
    $form['rates'][$qid]['delete'] = array(
      '#type' => 'checkbox',
      '#default_value' => 0,
    );
    if ($id && $id == $qid) {
      $edit = $r;
    }

  }

  // Get zones
  $zones = uc_shipping_zones_get_select();

  $form['quote'] = array(
    '#type' => 'fieldset',
    '#title' => t($label),
    '#description' => t('Defines a range of weights for the order total weight.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['quote']['qid'] = array(
    '#type' => 'hidden',
    '#value' => $edit->qid,
  );

  $form['quote']['uc_global_quote_minimum'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum weight'),
    '#default_value' => number_format($edit->min, 2, '.', ''),
    '#size' => 12,
    '#description' => t('minimum order weight in %unit, (order weight >= min)', array('%unit' => variable_get('uc_weight_unit', 'lb'))),
  );
  $form['quote']['uc_global_quote_maximum'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum weight'),
    '#default_value' => number_format($edit->max, 2, '.', ''),
    '#size' => 12,
    '#description' => t('maximum order weight in %unit, (order weight < max)', array('%unit' => variable_get('uc_weight_unit', 'lb'))),
  );
  $form['quote']['uc_global_quote_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipping Rate'),
    '#default_value' => number_format($edit->rate, variable_get('uc_currency_prec', 2), '.', ''),
    '#size' => 12,
    '#description' => t('Shipping price in %currency', array('%currency' => variable_get('uc_currency_code', 'USD'))),
  );
  $form['quote']['uc_global_quote_zone'] = array(
    '#type' => 'select',
    '#title' => t('Shipping zone'),
    '#options' => $zones,
    '#default_value' => $edit->zid,
    '#multiple' => FALSE,
    '#description' => t('Select a zone for this rate.') . l('edit/add zones', 'admin/store/settings/quotes/methods/zones'),
  );

  if ($edit->qid) {
    $form['quote']['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    );
  }
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit Changes'),
  );
  $form['#redirect'] = 'admin/store/settings/quotes/methods/global_quote';
  return $form;
}

/**
 * Admin form validation
 */
function uc_global_quote_admin_validate($form, &$form_state) {
  $values = $form_state['values'];
  // Cancel edit
  if ($form_state['input']['op'] == t('Cancel')) {
    $form_state['redirect'] = 'admin/store/settings/quotes/methods/global_quote';
    return;
  }
  
  // Don't check on delete
  if ($form_state['values'] && isset($form_state['values']['rates']) && is_array($form_state['values']['rates']) &&
      array_search( array('delete' => 1), $form_state['values']['rates'])) {
      return;
  }

  // check if everything is a number greater than or equal to 0
  if ($values['uc_global_quote_minimum'] < 0 || !is_numeric($values['uc_global_quote_minimum'])) {
    form_set_error('uc_global_quote_minimum', t('The minimum value must be a number not less than 0.'));
  }
  if ($values['uc_global_quote_maximum'] < 0 || !is_numeric($values['uc_global_quote_maximum'])) {
    form_set_error('uc_global_quote_maximum', t('The maximum value must be a number greater than 0.'));
  }
  if (($values['uc_global_quote_rate'] < 0) ||
      (!is_numeric($values['uc_global_quote_rate']))) {
    form_set_error('uc_global_quote_rate', t('Shipping rate must be a number not less than 0.'));
  }

  // check that max is bigger than min
  if ($values['uc_global_quote_maximum'] && $values['uc_global_quote_maximum'] <= $values['uc_global_quote_minimum']) {
    form_set_error('uc_global_quote_maximum', t('The maximum value must be greater than the minumum value.'));
  }

  if (!$values['uc_global_quote_zone']) {
    form_set_error('uc_global_quote_minimum', t('You must select a zone.'));
  }

  if ($values['uc_global_quote_maximum'] > 0) {
    //check to make sure the new settings dont overlap any existing ranges

    if ($values['qid']) {
      $result = db_query("SELECT * FROM {uc_global_quote} WHERE zid=:zid AND qid <> :qid ORDER BY min",
    array(':zid' => $values['uc_global_quote_zone'], ':qid' => $values['qid']))->fetchAll();
    }

    else {
      $result = db_query("SELECT * FROM {uc_global_quote} WHERE zid=:zid ORDER BY min",
                    array(':zid' => $values['uc_global_quote_zone']))->fetchAll();
    }

    foreach ($result as $r) {
      if ($values['uc_global_quote_minimum'] < $r->max && $values['uc_global_quote_minimum'] >= $r->min) {
        form_set_error('uc_global_quote_minimum', t('The minimum value conflicts with an existing Range.'));
      }
      if ($values['uc_global_quote_maximum'] <= $r->max && $values['uc_global_quote_maximum'] > $r->min) {
        form_set_error('uc_global_quote_maximum', t('The maximum value conflicts with an existing Range.'));
      }
    }
  }
}

/**
 * Admin form submit
 */

function uc_global_quote_admin_submit($form, &$form_state) {

  if ($form_state['input']['op'] == t('Cancel')) {
    return;
  }

  // check for and handle any deletions
  if ($form_state['values'] && isset($form_state['values']['rates'])
   && is_array($form_state['values']['rates']) && array_search( array('delete' => 1), $form_state['values']['rates'])) {
    foreach ($form_state['values']['rates'] as $qid => $value) {
      if ($value['delete']) {
        db_delete('uc_global_quote')->condition('qid', $qid)->execute();
      }
    }
  }

  // check for and insert new rate quote
  else if ($form_state['values']['uc_global_quote_maximum'] > $form_state['values']['uc_global_quote_minimum'] && 
      $form_state['values']['uc_global_quote_maximum'] > 0) {

    $fields = array(
      'zid' => $form_state['values']['uc_global_quote_zone'],
      'min' => $form_state['values']['uc_global_quote_minimum'],
      'max' => $form_state['values']['uc_global_quote_maximum'],
      'rate' => $form_state['values']['uc_global_quote_rate'],
    );

    if (!$form_state['values']['qid']) {
      db_insert('uc_global_quote')->fields($fields)->execute();
    }
    else {
      db_update('uc_global_quote')->fields($fields)->condition('qid', $form_state['values']['qid'], '=')->execute();
      $form_state['redirect'] = 'admin/store/settings/quotes/methods/global_quote';
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Theme admin form
 */

function theme_uc_global_quote_admin($form) {
  $output = '';
  $header = array(t('Delete'), t('Zone'), t('Minimum weight'), t('Maximum weight'), t('Shipping Rate'), '');
  $result = db_query("SELECT qid,min,max,rate,{uc_global_quote}.zid,{uc_shipping_zones}.name as zone FROM {uc_global_quote}
            LEFT JOIN {uc_shipping_zones} ON {uc_shipping_zones}.zid = {uc_global_quote}.zid ORDER by zone, min")->fetchAll();

  if (count($result)) {
    foreach ($result as $r) {
      $row = array();
      $qid = $r->qid;
      $row[] = drupal_render($form['form']['rates'][$qid]['delete']);
      $row[] = ($r->zone) ? $r->zone : t('--unnassigned--');
      $row[] = number_format($r->min, 2, variable_get('uc_currency_dec', '.'), '');
      $row[] = number_format($r->max, 2, variable_get('uc_currency_dec', '.'), '');
      $row[] = ($r->rate > 0) ? number_format($r->rate, 2, variable_get('uc_currency_dec', '.'), '') : 'Free';
      $row[] = l('edit', 'admin/store/settings/quotes/methods/global_quote/' . $qid);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array(
        'data' => t('No rates found.'),
        'colspan' => 4,
      ));
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  $output .= drupal_render_children($form['form']);

  return $output;
}
