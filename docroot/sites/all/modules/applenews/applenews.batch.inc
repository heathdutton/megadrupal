<?php
/**
 * @file
 * Batch operations.
 */

/**
 * Batch Apple News export.
 *
 * @see applenews_settings_exports_form_submit()
 */
function applenews_batch_run_export(ApplenewsExport $export, $op) {

  $operations = [];

  switch ($op) {

    case 'export-to-file':
      $op_function = 'applenews_batch_preview_generate';
      $op_finish = 'applenews_batch_export_to_file_finished';
      break;

    case 'export':
    default:
      $op_function = 'applenews_batch_post';
      $op_finish = 'applenews_batch_run_export_finished';
      break;

  }

  $result = $export->query()->execute();
  $entity_type = $export->entityType();

  if (isset($result[$entity_type])) {
    foreach ($result[$entity_type] as $entity_id => $object) {
      // Load entity object and pass it to the document generator.
      $entity = entity_load_single($entity_type, $entity_id);
      $operations[] = [
        $op_function,
        [
          $export->id,
          $entity_type,
          $entity,
          count($result[$entity_type])
        ],
      ];
    }
  }

  return [
    'operations' => $operations,
    'finished' => $op_finish,
  ];
}

/**
 * Batch operation for export.
 *
 * This is the function that is called on each operation in Apple News export.
 */
function applenews_batch_post($export_id, $entity_type, $entity, $total_count, &$context) {
  module_load_include('inc', 'applenews');

  $export = applenews_export_load($export_id);

  $export_settings = $export->settings;

  // Add data to context so it can be resused in 'finished' callback.
  if (!isset($context['results']['info']['starttime'])) {
    $context['results']['info']['starttime'] = REQUEST_TIME;
  }
  $context['results']['info']['name'] = $export->title;
  $context['results']['info']['export_id'] = $export_id;
  $context['results']['info']['default_channels'] = !empty($export_settings['channels'])
    ? array_keys($export_settings['channels']) : [];

  foreach (['count', 'insert', 'update', 'skip', 'step_count'] as $key) {
    if (!isset($context['results']['info'][$key])) {
      $context['results']['info'][$key] = 0;
    }
  }

  $count = !empty($context['results']['info']['step_count']) ? $context['results']['info']['step_count'] : 0;

  // Save batch progress status. Export status indicator visible in the Status
  // column.
  $status = [
    'count'     => $count,
    'starttime' => !empty($context['results']['info']['starttime']) ? $context['results']['info']['starttime'] : 0,
    'status'    => 'in progress ' . $count . '/' . $total_count,
  ];
  applenews_batch_export_set_status($export_id, $status);

  /** @var EntityDrupalWrapper $wrapper */
  $wrapper = entity_metadata_wrapper($entity_type, $entity);

  // Check default channel settings for each export.
  // This is a requried field, make sure to enable at least one channel.
  $settings = [];
  if (isset($export_settings['sections'])) {
    foreach ($export_settings['sections'] as $section_id => $checked) {
      if ($checked) {
        $settings['metadata']['data']['links']['sections'][] = applenews_section_url($section_id);
      }
    }
  }

  // Visbile to members of the channel only.
  $settings['metadata']['data']['is_preview'] = !empty($export_settings['is_preview']);

  if (!empty($export_settings['channels'])) {
    foreach ($export_settings['channels'] as $channel_id => $checked) {
      if (!$checked) {
        continue;
      }
      $channel = applenews_channel($channel_id);
      $context['results'][] = $wrapper->getIdentifier() . ' : ' . check_plain($wrapper->label());
      $context['message'] = t('<strong>@export</strong>: Posting "@title" entity to <em>@channel</em> channel.',
        [
          '@export'  => $export->title,
          '@title'   => $wrapper->label(),
          '@channel' => filter_xss($channel->name)
        ]
      );
      // Check if the article had already been exported.
      $export_log = applenews_is_exported($channel_id, $wrapper, $entity_type);
      // Process only non-published articles.
      if (!$export_log) {
        // Post an article to Apple News.
        if ($success = applenews_op('insert', $channel_id, $entity, $entity_type, $export)) {
          $context['results']['info']['insert']++;
        };
      }
      else {
        // Update an article only if the entity was updated since the last
        // export.
        if ($export_log['postdate'] < $entity->changed) {
          if ($success = applenews_op('update', $channel_id, $entity, $entity_type, $export)) {
            $context['results']['info']['update']++;
          }
        }
        else {
          $success = TRUE;
          $context['results']['info']['skip']++;
        }
      }
      if (!$success) {
        drupal_set_message(
          t('Unable to push %title to %channel',
            [
              '%title'   => $wrapper->label(),
              '%channel' => filter_xss($channel->name)
            ]), 'error'
        );
      }
    }
    $context['results']['info']['count']
      = $context['results']['info']['insert'] +
        $context['results']['info']['update'] +
        $context['results']['info']['skip'];
    $context['results']['info']['step_count']++;
  }

}

/**
 * Batch 'finished' callback used by batch Apple News export.
 */
function applenews_batch_run_export_finished($success, $results, $operations) {
  module_load_include('inc', 'applenews');
  if ($success) {
    if (empty($results['info']['default_channels'])) {
      drupal_set_message(
        t('Please configure default channel settings for <strong>@export_name</strong> export.', ['@export_name' => !empty($results['info']['name']) ? $results['info']['name'] : '']),
        'warning'
      );
    }
    else {
      $count = $results['info']['count'];
      // Save export information in logs.
      $record = [
        'exid'         => $results['info']['export_id'],
        'starttime'    => $results['info']['starttime'],
        'endtime'      => REQUEST_TIME,
        'numprocessed' => $count,
      ];
      drupal_write_record('applenews_log', $record);

      // Save batch progress status.
      $status = [
        'count'     => $count,
        'starttime' => !empty($results['info']['starttime']) ? $results['info']['starttime'] : 0,
        'endtime'   => REQUEST_TIME,
        'status'    => 'complete',
      ];
      applenews_batch_export_set_status($results['info']['export_id'],
        $status);

      $plural_message = format_plural(
        $count,
        '1 entity successfully processed (!insert created, !update updated, !skip unchanged).',
        '@count entities were successfully processed (!insert created, !update updated, !skip unchanged).',
        [
          '!insert' => $results['info']['insert'],
          '!update' => $results['info']['update'],
          '!skip' => $results['info']['skip'],
        ]
      );
      drupal_set_message($plural_message);
      $msg = t('Batch export: %count entities successfully posted to Apple News.');
      $args = [
        '%count'   => $count,
      ];
      watchdog('applenews', $msg, $args, WATCHDOG_NOTICE);
    }
  }
  else {
    // Save batch progress status.
    $status = [
      'count'     => !empty($results['info']['count']) ? $results['info']['count'] : 0,
      'starttime' => !empty($results['info']['starttime']) ? $results['info']['starttime'] : 0,
      'endtime'   => REQUEST_TIME,
      'status'    => 'error occured',
    ];
    applenews_batch_export_set_status($results['info']['export_id'], $status);
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing @operation with arguments : @args', [
      '@operation' => $error_operation[0],
      '@args'      => print_r($error_operation[0], TRUE),
    ]));
  }
}

/**
 * Batch operation for export preview.
 *
 * This is the function that is called on each operation in Export to File.
 */
function applenews_batch_preview_generate($export_id, $entity_type, $entity, $total_count, &$context) {
  module_load_include('inc', 'applenews');

  $export = applenews_export_load($export_id);

  if (!isset($context['results']['count'])) {
    $context['results']['count'] = 0;
  }

  /** @var EntityDrupalWrapper $entity_wrapper */
  $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);
  $context['results'][] = $entity_wrapper->getIdentifier() . ' : ' . check_plain($entity_wrapper->label());
  $context['message'] = t('<strong>@export</strong>: Exporting "@title" entity to a file.',
    [
      '@export' => $export->title,
      '@title'  => $entity_wrapper->label(),
    ]
  );

  $data = applenews_entity_export($export, $entity_type, $entity_wrapper->getIdentifier());

  $context['results']['export_id'] = $export->id;
  $context['results']['entity_ids'][] = $entity_wrapper->getIdentifier();

  // Generate article files for an entity.
  applenews_export_to_file($entity_wrapper->getIdentifier(), [], NULL, FALSE, $data);

  $context['results']['count']++;

}

/**
 * Batch 'finished' callback used by batch Apple News export to file.
 */
function applenews_batch_export_to_file_finished($success, $results, $operations) {
  module_load_include('inc', 'applenews');
  $count = isset($results['count']) ? $results['count'] : 0;
  if ($success) {
    if ($count > 0) {
      // Archive exported entities and generate a link to the archive.
      $file_url = applenews_export_to_file(NULL, $results['entity_ids'], 'export-' . $results['export_id'], FALSE, []);
      // Remove exported entity directories.
      applenews_export_dir_cleanup($results['entity_ids']);
      // Show message.
      if (!empty($file_url)) {
        $download = t('<strong>!link</strong> the file and preview articles in the Apple News Preview Tool.', [
          '!link' => l(t('Download'), $file_url),
        ]);
        $plural_message = format_plural(
          $count,
          '1 entity successfully exported to a file. ' . $download,
          '@count entities were successfully exported to a file. ' . $download
        );
        drupal_set_message($plural_message);
      }
    }
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing @operation with arguments : @args', [
      '@operation' => $error_operation[0],
      '@args' => print_r($error_operation[0], TRUE),
    ]));
  }
}
