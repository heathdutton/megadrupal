<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 19.7.13
 * Time: 12:13
 */

//=============================================================================
// ADMIN
//=============================================================================

/**
 * Settings form with batch processing launch button
 */
function exporter_overview_page() {
  $info_all = exporter_get_info();
  $list = array();
  foreach ($info_all as $id => $info) {
    $list[] = l($info['name'], 'admin/content/exporter/' . $id . '/settings') . ': ' . $info['description'];
  }
  return array(
    'overview_page' => array(
      '#theme' => 'item_list',
      '#items' => $list,
    ),
  );
}

/**
 * Settings form with batch processing launch button
 */
function exporter_settings_form($form, $form_state, $id) {
  $exporter = exporter_get_exporter($id);
  $items_count = $exporter->getCount();
  $operations_left = $exporter->queue->numberOfItems();
  if ($operations_left) {
    $last_operation_count = $items_count % $exporter->limit ? $items_count % $exporter->limit : $exporter->limit;
    $items_left = ($operations_left - 2) * $exporter->limit + $last_operation_count;
    $items_left_percent = $items_left / $items_count * 100;
    $next_run = 0;
  }
  else {
    $items_left = $items_left_percent = 0;
    $next_run = $exporter->last_finish + $exporter->period;
  }

  $form['exporter_id'] = array(
    '#type' => 'value',
    '#value' => $exporter->id,
  );

  $form['name'] = array(
    '#value' => '<h2>' . $exporter->name . ' (' .  $exporter->id . ')</h2>',
  );

  $form['description'] = array(
    '#value' => '<p>' . $exporter->description . '</p>',
  );

  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Informations'),
  );

  $info = array();
  $info['items_count'] = t('There are @count items in the database which will be included in the export.', array('@count' => $items_count));

  if ($exporter->last_finish) {
    $info['last_run'] = t('Last export was finished at %date (%ago ago) and it took %duration to finish it.', array('%ago' => format_interval(REQUEST_TIME - $exporter->last_finish), '%date' => format_date($exporter->last_finish), '%duration' => format_interval($exporter->last_duration)));
  }
  else {
    $info['last_run'] = t('Export has never run on this site.');
  }

  if ($operations_left) {
    $info['next_run'] = t('Export is in processs since %date (%ago ago).', array('%ago' => format_interval(REQUEST_TIME - $exporter->last_start), '%date' => format_date($exporter->last_start)));
  }
  else {
    $info['next_run'] = t('Next export is scheduled to %date (in %interval).', array('%interval' => format_interval($next_run - REQUEST_TIME), '%date' => format_date($next_run)));
  }

  $info['queue'] = format_plural($operations_left, 'There is 1 operation in the queue.', 'There are @count operations in the queue.') . ' ' . t('Around @items_count (@items_pct%) items to be processed.', array('@items_count' => $items_left, '@items_pct' => round($items_left_percent)));

  if ($exporter->fileExists() && $file_path = $exporter->getFilePath()) {
    $info['file'] = t('Current export file: <a href="@url">@url</a>', array('@url' => file_create_url($file_path)));
  }

  $info['preview'] = l(t('Export preview'), "admin/content/exporter/$exporter->id/preview");

  $form['info']['info'] = array(
    '#theme' => 'item_list',
    '#items' => $info,
  );

  $form['batch_export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Batch export'),
    '#description' => t('Run export using the Drupal batch interface.'),
  );

  $form['batch_export']['export'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );

  $form['basic_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
  );

  $form['basic_settings']['exporter_' . $id . '_use_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Cron'),
    '#options' => array(
      1 => t('Enabled'),
      0 => t('Disabled'),
    ),
    '#default_value' => $exporter->use_cron,
    '#description' => t('Enable or disable periodical generation of the file invoked via cron.'),
  );

  $form['basic_settings']['exporter_' . $id . '_use_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Cache'),
    '#options' => array(
      1 => t('Enabled'),
      0 => t('Disabled'),
    ),
    '#default_value' => $exporter->use_cache,
    '#description' => t('Enable or disable fragment caching.'),
  );

  $form['basic_settings']['exporter_' . $id . '_period'] = array(
    '#type' => 'select',
    '#title' => t('Cron treshold'),
    '#options' => drupal_map_assoc(array(0, 60, 300, 900, 1800, 3600, 7200, 10800, 14400, 21600, 43200, 86400)),
    '#description' => t('The minimal cron time interval in seconds between XML feed re-generation.'),
    '#default_value' => $exporter->period,
  );

  $form['basic_settings']['exporter_' . $id . '_limit'] = array(
    '#type' => 'select',
    '#title' => t('Number of items per operation'),
    '#options' => drupal_map_assoc(array(5, 10, 20, 50, 100, 200, 300, 500, 1000)),
    '#description' => t('The maximum number of items re-generated in each pass of a <a href="@cron">cron maintenance task</a> '.
      'or manual export operation. If necessary, reduce the number of items to prevent timeouts and memory errors while generating.',
      array('@cron' => url('admin/reports/status'))),
    '#default_value' => $exporter->limit,
  );

  $form['basic_settings']['exporter_' . $id . '_filename'] = array(
    '#type' => 'textfield',
    '#title' => t('Filename'),
    '#description' => t('The name of the generated file. File will be placed in your files directory: %files_dir.', array('%files_dir' => file_default_scheme() . '://')),
    '#default_value' => $exporter->filename,
  );

  $form['basic_settings']['exporter_' . $id .'_log_level'] = array(
    '#type' => 'radios',
    '#title' => t('Log level'),
    '#options' => array(
      WATCHDOG_EMERGENCY => t('0: Emergency'),
      WATCHDOG_ALERT     => t('1: Alert'),
      WATCHDOG_CRITICAL  => t('2: Critical'),
      WATCHDOG_ERROR     => t('3: Error'),
      WATCHDOG_WARNING   => t('4: Warning'),
      WATCHDOG_NOTICE    => t('5: Notice'),
      WATCHDOG_INFO      => t('6: Info'),
      WATCHDOG_DEBUG     => t('7: Debug'),
    ),
    '#description' => t('Choose maximal log level. Messages with higher severity number will be ignored.'),
    '#default_value' => $exporter->log_level,
  );

  $form['#validate'] = array('exporter_settings_form_validate');

  return system_settings_form($form);
}

/**
 * Validate callback.
 */
function exporter_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Export')) {
    drupal_goto('admin/content/exporter/' . $form_state['values']['exporter_id'] . '/export');
  }
}

/**
 * Menu callback: confirm regeneration of XML export.
 */
function exporter_batch_confirm($form, $form_state, $name) {
  $form['exporter_id'] = array(
    '#type' => 'value',
    '#value' => $name,
  );
  return confirm_form($form,
    t('Are you sure you want to re-generate Zbozi.cz export?'),
    'admin/settings/exporter',
    t('This action cannot be undone.'),
    t('Re-generate'),
    t('Cancel')
  );
}

/**
 * Handler for export confirmation
 */
function exporter_batch_confirm_submit(&$form, &$form_state) {
  if ($form['confirm']) {
    $id = $form_state['values']['exporter_id'];
    $exporter = exporter_get_exporter($id);
    $scheduler = ExporterScheduler::instance();
    $scheduler->batchRun($exporter);
    $form_state['redirect'] = 'admin/content/exporter';
    return;
  }
}

function exporter_preview_page($exporter_id) {
  $exporter = exporter_get_exporter($exporter_id);
  $item_id = reset($exporter->getItemIds(0, 1));
  $item = $exporter->loadItem($item_id);

  $output = '';
  $output .= '<textarea cols="200" rows="20" style="font-family: monospace; font-size: 1.2em">';
  $output .= $exporter->renderHeader();
  $output .= $exporter->renderItem($item);
  $output .= $exporter->renderFooter();
  $output .= '</textarea>';

  return $output;
}
