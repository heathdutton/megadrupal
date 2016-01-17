<?php

/**
 * @file
 * Administration functions for wow.module.
 */

/**
 * @defgroup wow-api-services-predefined List of predefined services
 * @{
 * API to provide a list of predefined services.
 */

/**
 * Prepares the service code list for a select form item with only the unsupported ones.
 */
function _wow_prepare_predefined_list() {
  $services = wow_service_list();
  $predefined = _wow_get_predefined_list();
  foreach ($predefined as $key => $value) {
    if (isset($services[$key])) {
      unset($predefined[$key]);
      continue;
    }
    // Include native name in output, if possible
    if (count($value) > 3) {
      $tname = t($value[2]);
      $predefined[$key] = ($tname == $value[3]) ? $tname : "$tname ($value[3])";
    }
    $predefined[$key] = t($value[2]);
  }
  asort($predefined);
  return $predefined;
}


/**
 * Some of the supported services.
 */
function _wow_get_predefined_list() {
  include_once DRUPAL_ROOT . '/includes/iso.inc';
  $predefined = _locale_get_predefined_list();
  foreach (wow_api_info() as $region => $info) {
    foreach ($info->locales as $langcode => $locale) {
      // TODO: check if still needed.
      $key = drupal_strtolower(str_replace('_', '-', $locale));
      if (array_key_exists($key, $predefined)) {
        $services[$key] = array_merge(array($region, $locale), $predefined[$key]);
      }
      else {
        $services[$langcode] = array_merge(array($region, $locale), $predefined[$langcode]);
      }
    }
  }
  return $services;
}

/**
 * @} End of "wow-api-services-predefined"
 */

/**
 * Prepare a batch to import data for all enabled modules in a given locale.
 *
 * @param string $langcode
 *   Language code to import data for.
 * @param string $finished
 *   Optional finished callback for the batch.
 * @param $force
 *   An array of data resources to force refresh, by entity type.
 */
function wow_batch_by_service($langcode, $finished = NULL, $force = array()) {
  $services = wow_service_list('language');
  // Collect all data endpoint to import for all enabled modules.
  $data = array();
  foreach (module_invoke_all('wow_data_resources') as $module => $resources) {
    foreach ($resources as $key => $resource) {
      // Collect all resources for all components, for a specific $langcode.
      // This allows for batch API to be able to import in small chunks of data.
      $expires = @$services[$langcode]->expires[$module][$key];
      $expires = isset($expires) ? $expires : REQUEST_TIME + 1;
      if ($expires > REQUEST_TIME || array_key_exists($resource['entity type'], $force)) {
        $resource += array(
          'data key' => $key,
          'base module' => $module,
        );
        $data[] = $resource;
      }
    }
  }

  return _wow_batch_build($services[$langcode], $data, $finished);
}

/**
 * Build a wow service batch from an array of data resources.
 *
 * @param $service
 *   The service definition.
 * @param $resources
 *   Array of data resources to import.
 * @param $finished
 *   Optional finished callback for the batch.
 */
function _wow_batch_build($service, $resources, $finished = NULL) {
  if (count($resources)) {
    $operations = array();
    foreach ($resources as $resource) {
      // We call _wow_batch_import for every batch operation.
      $operations[] = array('_wow_batch_import', array($service, $resource));
    }
    $batch = array(
      'operations'      => $operations,
      'title'           => t('Updating data resources translations.'),
      'init_message'    => t('Starting import.'),
      'error_message'   => t('Error importing data resources translations.'),
      'file'            => drupal_get_path('module', 'wow') . '/wow.inc',
    );
    if (isset($finished)) {
      $batch['finished'] = $finished;
    }
    return $batch;
  }
  return FALSE;
}

/**
 * Execute a wow service defined batch.
 *
 * @param $batch
 *   A batch object to execute.
 *
 * @see _wow_batch_build().
 */
function _wow_batch_execute($batch) {
  $context = array();

  foreach ($batch['operations'] as $operation) {
    _wow_batch_import($operation[1][0], $operation[1][1], $context);
  }

  $batch['finished'](TRUE, $context['results']);
}

/**
 * Perform data resource translation import as a batch step.
 *
 * @param $service
 *   The wow data service.
 * @param $resource
 *   The resource to import.
 */
function _wow_batch_import($service, $resource, &$context) {
  $key = $resource['data key'];
  $module = $resource['base module'];
  $entity_type = $resource['entity type'];

  $response = entity_get_controller($entity_type)->fetchAll($service->region, $service->locale);

  if ($response instanceof WoWHttpStatus) {
    // Do not continue the operation if a status code is returned.
    $context['results'][$module . ':' . $key] = 'fail';
    watchdog_exception($module, new WoWHttpException($response), t('An exception has been thrown during %name data resources import.', array('%name' => $resource['name'])));
    return;
  }

  $report = array('additions' => 0, 'updates' => 0, 'deletes' => 0);
  $info = entity_get_info($entity_type);
  $entities = entity_load($entity_type);

  foreach ($response->getArray($resource['data key']) as $values) {
    $id = $values[$info['entity keys']['id']];
    $entity = entity_create($resource['entity type'], $values + array(
      'language' => $service->language,
      'is_new' => empty($entities[$id])
    ));
    $op = $entity->save();

    if ($op == SAVED_NEW) {
      $report['additions']++;
    }
    elseif ($op == SAVED_UPDATED) {
      $report['updates']++;
    }

    unset($entities[$id]);
  }

  // Delete extra entities that are not needed anymore.
  foreach ($entities as $entity) {
    $entity->delete();
    $report['deletes']++;
  }

  $result = db_select('wow_services', 's')
    ->fields('s', array('expires'))
    ->condition('language', $service->language)
    ->execute();

  $headers = $response->getHeaders();
  $cache_control = explode('=', $headers['cache-control']);
  $expires = unserialize($result->fetchField());
  $expires[$module][$key] = $response->getDate()->format('U') + $cache_control[1];

  db_update('wow_services')
    ->fields(array('expires' => serialize($expires)))
    ->condition('language', $service->language)
    ->execute();

  $context['message'] = t('Imported: %name.', array('%name' => $resource['name']));
  $context['results'][$module . ':' . $key] = $report;
}

/**
 * Finished callback of service addition locale import batch.
 * Inform the user of translation data imported.
 */
function _wow_batch_service_finished($success, $results) {
  if ($success) {
    foreach ($results as $key => $report) {
      list($module, $key) = explode(':', $key);
      $info = module_invoke($module, 'wow_data_resources');
      $args = array(
        '!name' => $info[$module][$key]['name'],
        '@additions' => $report['additions'],
        '@updates' => $report['updates'],
        '@deletes' => $report['deletes'],
      );
      drupal_set_message(t('Imported !name: @additions addition(s), @updates update(s), @deletes delete(s).', $args));
    }
  }
}

/**
 * Finished callback of service cron run.
 * Log in the database imported data.
 */
function _wow_batch_cron_service_finished($success, $results) {
  if ($success) {
    foreach ($results as $key => $report) {
      list($module, $key) = explode(':', $key);
      $info = module_invoke($module, 'wow_data_resources');
      $args = array(
        '!name' => $info[$module][$key]['name'],
        '@additions' => $report['additions'],
        '@updates' => $report['updates'],
        '@deletes' => $report['deletes'],
      );
      watchdog('wow', 'Imported !name: @additions addition(s), @updates update(s), @deletes delete(s).', $args, WATCHDOG_INFO);
    }
  }
}
