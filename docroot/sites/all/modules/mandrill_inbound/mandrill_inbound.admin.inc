<?php
/**
 * @file
 * Administrative functions.
 */

/**
 * Menu callback for admin settings.
 */
function mandrill_inbound_settings_form() {
  $form = array();

  $form['mandrill_inbound_email_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Email domain'),
    '#default_value' => variable_get('mandrill_inbound_email_domain', 'example.co.uk'),
  );

  $form['current_webhook'] = array(
    '#markup' => t('You should add follow webhook to routes: %url', array(
      '%url' => url('mandrill/webhook/inbound', array('absolute' => TRUE)),
    )),
  );

  $rows = array();
  foreach (mandrill_get_api_object()->webhooks_list() as $list) {
    $rows[] = array(
      $list['id'],
      $list['url'],
      $list['auth_key'],
      isset($list['last_sent_at']) ? $list['last_sent_at'] : t('Never'),
      $list['batches_sent'],
      $list['events_sent'],
      $list['description'],
    );
  }

  $header = array(
    t('ID'),
    t('URL'),
    t('Auth Key'),
    t('Last Successful'),
    t('Batches Sent'),
    t('Events Sent'),
    t('Description'),
  );

  $form['webhooks'] = array(
    '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
  );

  return system_settings_form($form);
}
