<?php

/**
 * @file
 * Extends Simplenews by providing a visitor log by user.
 *
 * Date: 10-ott-2013
 * File: simplenews_visitor.admin.inc
 * Author: stefano
 */

/**
 * Setting simplenews visitor.
 *
 * SC 28/giu/2011 00:27:30 stefano
 *
 * @param array $form
 *   form data
 * @param array $form_state
 *   form data
 *
 * @return array
 *   the form structure
 */
function simplenews_visitor_admin_setting($form, &$form_state) {

  $header = array(
    'name' => array(
      'data' => t('Newsletter'),
      'field' => 'name',
      'sort' => 'asc',
    ),
    'max' => array(
      'data' => t('Max'),
      'field' => 'max',
    ),
    'amount' => array(
      'data' => t('Used'),
      'field' => 'amount',
    ),
  );

  // Field to sort on is picked from $header.
  $query = db_select('{simplenews_category}', 'c')->extend('PagerDefault')->extend('TableSort')->orderByHeader($header);
  $query->leftJoin('{taxonomy_term_data}', 't', '(t.tid=c.tid)');
  $query->leftJoin('{simplenews_visitor_max_sent}', 'm', '(m.tid = c.tid)');
  $query->fields('m', array('max', 'amount'));
  $query->fields('t', array('tid', 'name'));

  $result = $query->execute();

  while ($row = $result->fetchObject()) {
    $options[$row->tid] = array(
      'name' => $row->name,
      'max' => (!is_null($row->max) ? $row->max : t('n/a')),
      'amount' => (!is_null($row->amount) ? $row->amount : t('n/a')),
    );
  }

  $form['simplenews_visitor_general_settings']['_max_sent'] = array(
    '#type' => 'fieldset',
    '#title' => t('Max values'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['simplenews_visitor_general_settings']['_max_sent']['simplenews_tid'] = array(
    '#type' => 'tableselect',
    '#title' => t('Newsletter'),
    '#options' => $options,
    '#header' => $header,
    '#empty' => t('No content available.'),
    '#required' => FALSE,
    '#weight' => -2,
    '#empty' => t('No newsletter found.'),
  );
  $form['simplenews_visitor_general_settings']['_max_sent']['pager'] = array(
    '#markup' => theme('pager'),
    '#weight' => -1,
  );
  $form['simplenews_visitor_general_settings']['_max_sent']['max_sent'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Max amount to sent'),
    '#description' => t('The value must be 0 or more.'),
    '#default_value' => 0,
    '#required' => FALSE,
    '#weight' => 0,
  );

  /* SC Wed Oct 23 09:38:26 CEST 2013 stefano
   *
   * Adds the fieldsets ad the item for the key
   */
  $form['simplenews_visitor_general_settings']['_url_key'] = array(
    '#type' => 'fieldset',
    '#title' => t('Url key'),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['simplenews_visitor_general_settings']['_url_key']['url_key'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#title' => t('The key to validate the Url'),
    '#description' => t('Insert an alphanumeric case sensitive string [a-z 0-9]. <b>Avoid to change the string or the old URLs will be rejected</b>.'),
    '#required' => TRUE,
    '#default_value' => check_plain(variable_get('simplenews_visitor_urlkey')),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Resend'),
    '#weight' => 99,
  );

  return $form;
}

/**
 * Form validate.
 *
 * SC Fri Oct 11 19:30:11 CEST 2013 stefano
 */
function simplenews_visitor_admin_setting_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['max_sent']) || floor($form_state['values']['max_sent']) != $form_state['values']['max_sent'] || $form_state['values']['max_sent'] < 0) {
    form_set_error('max_sent', t('Max amount must be a positive integer'));
  }
  if (preg_match('|.*[^a-zA-z0-9]+.*|', $form_state['values']['url_key'])) {
    form_set_error('url_key', t('Only alphanumeric characters.'));
  }
}

/**
 * Form submit.
 *
 * SC Fri Oct 11 19:29:43 CEST 2013 stefano
 */
function simplenews_visitor_admin_setting_submit($form, &$form_state) {
  foreach ($form_state['values']['simplenews_tid'] as $tid => $tid_check) {
    if ($tid_check) {
      $query = db_select('{simplenews_visitor_max_sent}', 's');
      $query->fields('s', array('max', 'amount'));
      $query->condition('s.tid', $tid);

      $result = $query->execute()->fetchObject();

      if ($result) {
        // Update.
        $object = array(
          'tid' => $tid,
          'max' => $form_state['values']['max_sent'],
          'amount' => ($result->amount - $result->max),
        );
        $update = 'tid';
      }
      else {
        // Insert.
        $object = array(
          'tid' => $tid,
          'max' => $form_state['values']['max_sent'],
          'amount' => 0,
        );
        $update = array();
      }

      drupal_write_record('simplenews_visitor_max_sent', $object, $update);
    }
  }

  variable_set('simplenews_visitor_urlkey', $form_state['values']['url_key']);
}
