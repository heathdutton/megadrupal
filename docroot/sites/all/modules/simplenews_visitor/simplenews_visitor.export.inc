<?php

/**
 * @file
 * Extends Simplenews by providing a visitor log by user.
 *
 * Date: mar 15 lug 2014, 17.43.05, CEST
 * File: simplenews_visitor.export.inc
 * Author: stefano
 */

/**
 * Implements hook_form().
 *
 * Exports visitor/no visitor to a csv file.
 *
 * SC 15/lug/2014 17:47:20 stefano
 */
function simplenews_visitor_exports($form, &$form_state, $node) {
  $form = array();

  $form['needed'] = array(
    '#type' => 'radios',
    '#title' => t('What exports.'),
    '#default_value' => 0,
    '#options' => array('No visitors', 'Visitors'),
    '#description' => t('Select what you need to export to a csv file.'));

  $form['interval'] = array(
    '#type' => 'fieldset',
    '#title' => t('<b>Visitors</b> date interval.'),
    '#description' => t('Only for <b>Visitors</b> is possible to set a date range from where it export datas.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE);

  $form['interval']['date_from'] = array(
    '#type' => 'date',
    '#title' => t('From date.'),
    '#required' => TRUE);

  // Default is 1 day before.
  //
  // SC 15/lug/2014 18:39:48 stefano.
  $date_to = time() - 60 * 60 * 24;

  $form['interval']['date_to'] = array(
    '#type' => 'date',
    '#title' => t('To date.'),
    '#required' => TRUE,
    '#default_value' => array(
      'year' => date('Y', $date_to),
      'month' => date('n', $date_to),
      'day' => date('j', $date_to)));

  $form['nid'] = array('#type' => 'hidden', '#value' => $node->nid);

  $form['submit'] = array('#type' => 'submit', '#value' => t('Export'));

  return $form;
}

/**
 * Implements hook_form().
 *
 * Validate form.
 *
 * SC 15/lug/2014 19:29:46 stefano
 */
function simplenews_visitor_exports_validate($form, &$form_state) {
  if ($form_state['values']['needed']) {
    $date_from = mktime(23, 59, 59, $form_state['values']['date_from']['month'], $form_state['values']['date_from']['day'], $form_state['values']['date_from']['year']);
    $date_to = mktime(0, 0, 0, $form_state['values']['date_to']['month'], $form_state['values']['date_to']['day'], $form_state['values']['date_to']['year']);

    if ($date_from < $date_to) {
      form_set_error('date_to', t('<b>To date</b> must be less then <b>From date</b>.'));
    }
  }
}

/**
 * Implements hook_form().
 *
 * Submit form.
 *
 * SC 15/lug/2014 19:30:11 stefano
 */
function simplenews_visitor_exports_submit($form, &$form_state) {
  global $user;

  $date_from = mktime(23, 59, 59, $form_state['values']['date_from']['month'], $form_state['values']['date_from']['day'], $form_state['values']['date_from']['year']);
  $date_to = mktime(0, 0, 0, $form_state['values']['date_to']['month'], $form_state['values']['date_to']['day'], $form_state['values']['date_to']['year']);

  if ($form_state['values']['needed']) {
    $query = db_select('{simplenews_visitor}', 's');
    $query->fields('s', array('mail', 'url', 'date', 'link'));
    $query->where('s.nid = :nid AND UNIX_TIMESTAMP(date) BETWEEN :date_to AND :date_from', array(
      ':nid' => $form_state['values']['nid'],
      ':date_from' => $date_from,
      ':date_to' => $date_to));
  }
  else {
    $query = db_select('{simplenews_subscriber}', 's');
    $query->leftJoin('{simplenews_subscription}', 't', '(s.snid = t.snid)');
    $query->leftJoin('{simplenews_newsletter}', 'n', '(n.tid = t.tid)');
    $query->fields('s', array('mail'));
    $query->where('n.nid = :nid1
  AND s.mail NOT IN (SELECT v.mail FROM {simplenews_visitor} AS v WHERE v.nid = :nid2)
  AND (:uid1 = 1)
  AND s.activated = 1', array(
      ':nid1' => $form_state['values']['nid'],
      ':nid2' => $form_state['values']['nid'],
      ':uid1' => $user->uid));
  }

  $results = $query->execute();

  $period = date("Ymd", $date_from) . "-" . date("Ymd", $date_to);

  $filename = "simplenews_visitor_" . ($form_state['values']['needed'] ? "visitors_" . $period : "NO_visitors_" . date("YmdHis")) . "_nid_" . $form_state['values']['nid'] . '.csv';

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename = ' . $filename);

  // Instead of writing to a file, we write to the output stream.
  $fh = fopen('php://output', 'w');

  // Add a header row.
  if ($form_state['values']['needed']) {
    fputcsv($fh, array(t('e-mail'), t('URL'), t('date'), t('link')));
  }
  else {
    fputcsv($fh, array(t('e-mail')));
  }

  while ($item = $results->fetchObject()) {

    $data = array($item->mail);

    if ($form_state['values']['needed']) {
      $data[] = $item->url;
      $data[] = $item->date;
      $data[] = $item->link;
    }

    fputcsv($fh, $data);
  }

  // Close the output stream.
  fclose($fh);

  exit();
}
