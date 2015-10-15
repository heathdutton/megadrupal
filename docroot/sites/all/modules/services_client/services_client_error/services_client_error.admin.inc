<?php

/**
 * @file
 * Administration callbacks for error handling.
 */

/**
 * List all services client errors
 */
function services_client_error_admin_list() {
  $header = array(
    array('data' => 'Date', 'field' => 'created', 'sort' => 'desc'),
    array('data' => 'Entity'),
    array('data' => 'Entity Id'),
    array('data' => 'Event'),
    array('data' => 'Retries'),
    array('data' => 'Status'),
    array('data' => 'Code'),
    array('data' => 'Message'),
    array('data' => 'Actions'),
  );

  $query = db_select('services_client_error', 's')
    ->fields('s', array('eid', 'created','entity_type', 'entity_id', 'event',
      'retries', 'status', 'error_code', 'error_message'))
    ->extend('PagerDefault')     // Pager Extender
      ->limit(50)
    ->extend('TableSort')        // Sorting Extender
      ->orderByHeader($header);

  // Status filter was applied.
  $statuses = services_client_error_status_map();
  if (isset($_GET['status']) && in_array($_GET['status'], array_keys($statuses))) {
    $query->condition('status', $_GET['status'], '=');
  }

  $rows = array();
  foreach ($query->execute() as $error) {
    $error = (array) $error;

    $entities = entity_load($error['entity_type'], array($error['entity_id']));
    $entity = reset($entities);
    $event = services_client_get_event($error['event']);

    $row = array(
      'created' => format_date($error['created'], 'short'),
      'entity_type' => check_plain($error['entity_type']),
      'entity_id' => check_plain($error['entity_id']),
      'event' => !empty($event) ? l($error['event'], $event->getUrl()) : '[' . t('missing event') . ']',
      'retries' => check_plain($error['retries']),
      'status' => services_client_error_status_title($error['status']),
      'error_code' => check_plain($error['error_code']),
      'error_message' => check_plain($error['error_message']),
    );

    $actions = array();
    // If entity still exists on system, show link.
    if (!empty($entity)) {
      $uri = entity_uri($error['entity_type'], $entity);
      $actions[] = l(t('View Entity'), $uri['path'], (array) $uri['options']);
    }
    $actions[] = l(t('History'), 'admin/structure/services_client/errors/' . $error['eid'] . '/log', array('query' => drupal_get_destination()));
    $actions[] = l(t('Delete'), 'admin/structure/services_client/errors/' . $error['eid'] . '/delete', array('query' => drupal_get_destination()));
    if (!empty($entity) && $error['status'] != SC_ERROR_COMPLETED) {
      $actions[] = l(t('Retry'), 'admin/structure/services_client/errors/' . $error['eid'] . '/retry', array('query' => drupal_get_destination()));
    }
    $row['actions'] = implode(' | ', $actions);

    $rows[] = $row;
  }

  $filters = array(
    array(
      'title' => t('All'),
      'href' => 'admin/structure/services_client/errors'
    ),
  );
  foreach (services_client_error_status_map() as $id => $name) {
    // $filters[] = l($name, 'admin/structure/services_client/errors', array('query' => array('status' => $id)));
    $filters[] = array(
      'title' => $name,
      'href' => 'admin/structure/services_client/errors',
      'query' => array('status' => $id)
    );
  }

  $output['filter'] = array(
    '#theme' => 'links',
    '#links' => $filters,
    '#heading' => t("Filter"),
    '#attributes' => array('class' => array('links', 'inline')),
  );

  $output['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#sticky' => TRUE,
    '#empty' => !isset($_GET['status']) ? t('No syncing errors. Wohoo!') : t('No errors matching filter criteria'),
  );

  $output['pager'] = array(
    '#theme' => 'pager'
  );

  return $output;
}

/**
 * Delete error confirm form.
 */
function services_client_error_admin_delete_confirm($form, &$form_state, $error) {
  $form['error'] = array('#type' => 'value', '#value' => $error);
  return confirm_form($form, t('Are you sure you want to delete error?'), 'admin/structure/services_client/errors');
}

/**
 * Submit handler; Delete error.
 */
function services_client_error_admin_delete_confirm_submit($form, &$form_state) {
  services_client_error_delete($form_state['values']['error']['eid']);
  drupal_set_message(t('Error was deleted.'));
  $form_state['redirect'] = 'admin/structure/services_client/errors';
}

/**
 * Page callback; Show list of changes for error.
 */
function services_client_error_admin_log_list($error) {
  $query = db_query("SELECT lid, created, message, status_change, uid, error_code, error_message FROM {services_client_error_log} WHERE eid = :eid ORDER BY lid DESC", array(':eid' => $error['eid']));

  $rows = array();
  foreach ($query->fetchAll() as $item) {
    $row = array(
      'date' => format_date($item->created),
      'message' => check_plain($item->message),
      'status' => isset($item->status_change) ? services_client_error_status_title($item->status_change) : t('N/A'),
      'uid' => $item->uid,
      'error_code' => check_plain($item->error_code),
      'error_message' => check_plain($item->error_message),
    );

    if (!empty($row['uid'])) {
      $account = user_load($row['uid']);
      $row['uid'] = check_plain($account->name);
    }

    $rows[] = $row;
  }

  $header = array(
    array('data' => 'Date', 'field' => 'created', 'sort' => 'desc'),
    array('data' => 'Message'),
    array('data' => 'Status update'),
    array('data' => 'User'),
    array('data' => 'Error code'),
    array('data' => 'Error message'),
  );

  $output['table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
    '#empty' => t('No error history is available.'),
  );

  if (!empty($_REQUEST['destination'])) {
    $output['back'] = array(
      '#markup' => l(t('Back'), $_REQUEST['destination'])
    );
  }

  return $output;
}

/**
 * Resend data to connection that produced error
 */
function services_client_error_admin_repair($form, &$form_state, $error) {
  $form['error'] = array(
    '#type' => 'value',
    '#value' => $error
  );

  $entities = entity_load($error['entity_type'], array($error['entity_id']));
  $entity = reset($entities);

  $rows = array();
  $rows[] = array(t('Created'), format_date($error['created'], 'short'));
  $rows[] = array(t('Entity type'), check_plain($error['entity_type']));
  // If entity exists on local system, show link to entity.
  if (!empty($entity)) {
    $uri = entity_uri($error['entity_type'], $entity);
    $rows[] = array(t('Entity ID'), l($error['entity_id'], $uri['path'], $uri['options']));
  }
  else {
    $rows[] = array(t('Entity ID'), check_plain($error['entity_id']));
  }
  $event = services_client_get_event($error['event']);
  $rows[] = array(t('Event'), l($error['event'], $event->getUrl()));
  $rows[] = array(t('Retries'), check_plain($error['retries']));
  $rows[] = array(t('Result'), services_client_error_status_title($error['status']));

  $form['info'] = array(
    '#theme' => 'table',
    '#header' => array(array('data' => t('Info'), 'colspan' => 2)),
    '#rows' => $rows,
  );

  $history = services_client_error_admin_log_list($error);
  if (!empty($history['table']['#rows'])) {
    $form['history'] = $history['table'];
  }

  if (empty($history) && !empty($_REQUEST['destination'])) {
    $form['back'] = array(
      '#markup' => '<p>' . l(t('Back'), $_REQUEST['destination']) . '</p>',
    );
  }

  $form['actions'] = array();

  if (empty($error['completed'])) {
    $form['actions']['sync'] = array(
      '#type' => 'submit',
      '#value' => t('Synchronize'),
      '#submit' => array('services_client_error_admin_repair_synchronize'),
    );

    $form['actions']['mark'] = array(
      '#type' => 'submit',
      '#value' => t('Mark as completed (synchronized).'),
      '#submit' => array('services_client_error_admin_repair_mark'),
    );
  }

  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete error'),
    '#submit' => array('services_client_error_admin_repair_delete'),
  );

  return $form;
}

/**
 * Try to resynchronize data.
 */
function services_client_error_admin_repair_synchronize($form, &$form_state) {
  $error = $form_state['values']['error'];
  $result = services_client_error_retry($error);
  $log = array('eid' => $error['eid']);
  if (!$result->success()) {
    drupal_set_message(t('Failed to repair error. Reason: @error', array('@error' => check_plain($result->error_message))), 'error');
    $log += array(
      'message' => t('Manual re-try failed.'),
      'error_code' => $result->error_code,
      'error_message' => $result->error_message,
    );
  }
  else {
    $log += array(
      'message' => t('Manual re-try succeeded.'),
      'status_change' => SC_ERROR_COMPLETED,
    );
    drupal_set_message(t('Data has been successfully pushed.'));
  }
  services_client_error_log_save($log);
}

/**
 * Mark as repaired error.
 */
function services_client_error_admin_repair_mark($form, &$form_state) {
  $error = $form_state['values']['error'];
  $error['status'] = SC_ERROR_COMPLETED;
  drupal_write_record('services_client_error', $error, array('eid'));
  $log = array('eid' => $error['eid'], 'message' => t('Marked as synchronized.'), 'status_change' => SC_ERROR_COMPLETED);
  services_client_error_log_save($log);
  drupal_set_message(t('Error has been marked as synchronized.'));
}

/**
 * Delete error from error log.
 */
function services_client_error_admin_repair_delete($form, &$form_state) {
  $error = $form_state['values']['error'];
  services_client_error_delete($error['eid']);
  drupal_set_message(t('Services Client error has been deleted.'));
  $form_state['redirect'] = '<front>';
}
