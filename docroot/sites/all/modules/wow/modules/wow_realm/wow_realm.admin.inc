<?php

/**
 * @file
 * Admin page callback file for the character module.
 */

/**
 * Menu callback; Configure realm settings for this site.
 */
function wow_realm_admin_settings() {
  $rows = array();
  $header = array(t('Region'), t('Realms'), t('Status'), t('Operations'));

  foreach (wow_api_info() as $region => $info) {
    $result = db_select('wow_realm', 'realm')
      ->fields('realm', array('lastFetched'))
      ->condition('region', $region)
      ->execute()
      ->fetchAll(PDO::FETCH_COLUMN);

    $operations = array();

    $service = variable_get("wow_realm_service:$region");
    $service_status = isset($service) ? $service->status : ((count($result) && max($result) != 0) ? 'running' : 'stopped');

    if ($service_status == 'running') {
      // The most recently saved realm time is not equal to zero.
      // Service is running and realms are updating; Provides two operations.
      $operations['refresh'] = array(
        'title' => t('refresh'),
        'href' => 'admin/config/wow/realm/' . $region . '/refresh',
      );
      $operations['stop'] = array(
        'title' => t('stop'),
        'href' => 'admin/config/wow/realm/' . $region . '/stop',
      );

      $last_fetched = format_interval(REQUEST_TIME - max($result));
      $status = '<span class="ok">' . t('Running') . ' (' . t('last fetched !time ago', array('!time' => $last_fetched)) . ')</span>';
    }
    else {
      if (count($result)) {
        $status = '<em class="warning">' . t('Paused') . '</em>';
      }
      else {
        $status = '<em class="error">' . t('Not Installed') . '</em>';
      }
      $operations['start'] = array(
        'title' => t('start'),
        'href' => 'admin/config/wow/realm/' . $region . '/start',
      );
    }
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => 'admin/config/wow/realm/' . $region . '/edit',
    );

    $elements = array(
      '#theme' => 'links__node_operations',
      '#links' => $operations,
      '#attributes' => array('class' => array('links', 'inline')),
    );

    $rows[] = array($info->name, count($result), $status, drupal_render($elements));
  }

  $build['wow_realm_status'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}

/**
 * Menu callback; Starts a realm service.
 */
function wow_realm_admin_start($region) {
  $service = variable_get("wow_realm_service:$region", (object) array('status' => 'stopped', 'threshold' => 3600));
  $service->status = 'running';
  batch_set(array(
    'title' => t('Starting realm service for !region region.', array('!region' => $region)),
    'operations' => array(array('wow_realm_update_process', array($region))),
    'file' => drupal_get_path('module', 'wow_realm') . '/wow_realm.inc',
  ));
  variable_set("wow_realm_service:$region", $service);

  batch_process('admin/config/wow/realm');
}

/**
 * Menu callback; Refreshes a realm service.
 */
function wow_realm_admin_refresh($region) {
  batch_set(array(
    'title' => t('Refreshing realm service for !region region.', array('!region' => $region)),
    'operations' => array(array('wow_realm_update_process', array($region))),
    'finished' => 'wow_realm_delete_process',
    'file' => drupal_get_path('module', 'wow_realm') . '/wow_realm.inc',
  ));

  batch_process('admin/config/wow/realm');
}

/**
 * Menu callback; Stops a realm service.
 */
function wow_realm_admin_stop($region) {
  $service = variable_get("wow_realm_service:$region");
  $service->status = 'stopped';
  variable_set("wow_realm_service:$region", $service);

  drupal_goto('admin/config/wow/realm');
}

/**
 * Form builder; Edit a realm service.
 *
 * @ingroup forms
 */
function wow_realm_admin_edit($form, $form_state, $region) {
  $service = variable_get("wow_realm_service:$region", (object) array('status' => 'stopped', 'threshold' => 3600));
  $cron_safe_threshold = variable_get('cron_safe_threshold', DRUPAL_CRON_DEFAULT_THRESHOLD);

  $form['service'] = array(
    '#type' => 'fieldset',
    '#title' => t('Realm Service for !region region', array('!region' => wow_api_info($region, 'name'))),
    '#description' => t('Realm Serivce takes care of running periodic tasks like checking for updates and modification for realms.')
  );

  $form['service']['region'] = array(
    '#type' => 'value',
    '#value' => $region,
  );
  $form['service']['status'] = array(
    '#type' => 'value',
    '#value' => $service->status,
  );
  $form['service']['threshold'] = array(
    '#type' => 'select',
    '#title' => t('Run service every'),
    '#default_value' => $service->threshold,
    '#options' => array(0 => t('Never')) + drupal_map_assoc(array(3600, 10800, 21600, 43200, 86400, 604800), 'format_interval'),
  );

  if ($form['service']['threshold']['#default_value'] < $cron_safe_threshold) {
    $form['service']['threshold']['#description'] = '<span class="warning">' . t('The cron safe threshold value is higher than the !entity threshold. You can safely ignore this warning message if you manage externally your cron tasks.', array('!entity' => wow_api_info($region, 'name') . ' service')) . '</span>';
  }

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}

/**
 * Submit handler for the wow_realm_admin_edit form.
 */
function wow_realm_admin_edit_submit($form, &$form_state) {
  $service = array(
    'status' => $form_state['values']['status'],
    'threshold' => $form_state['values']['threshold'],
  );
  $region = $form_state['values']['region'];

  variable_set("wow_realm_service:$region", (object) $service);
}
