<?php
/**
 * @file
 * Database functions for OAI Harvester module
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Return the list of links to the providers which support a given format.
 *
 * @param $format_id (int)
 *   The id of format
 *
 * @return (array)
 *   List of links
 */
function _oaiharvester_providers_get_by_format($format_id) {
  $providers = array();

  $query = db_select('oaiharvester_formats_to_providers', 'ftp');
  $query->leftJoin('oaiharvester_providers', 'prov', 'prov.provider_id = ftp.provider_id');
  $query->fields('prov', array('provider_id', 'name'))
        ->condition('ftp.format_id', $format_id)
        ->orderBy('prov.name');

  $result = $query->execute();

  foreach ($result as $data) {
    $providers[] = l(check_plain($data->name),
      'admin/xc/harvester/repository/' . check_plain($data->provider_id));
  }
  return $providers;
}

/**
 * Return the list of links to the providers which support a given set.
 *
 * @param $set_id (int)
 *   The id of set
 *
 * @return (array)
 *   List of links
 */
function _oaiharvester_providers_get_by_set($set_id) {
  $providers = array();

  $query = db_select('oaiharvester_sets_to_providers', 'stp');
  $query->leftJoin('oaiharvester_providers', 'prov', 'prov.provider_id = stp.provider_id');
  $query->fields('prov', array('provider_id', 'name'))
        ->condition('stp.set_id', $set_id)
        ->orderBy('prov.name');

  $result = $query->execute();

  foreach ($result as $data) {
    $providers[] = l(check_plain($data->name),
      'admin/xc/harvester/repository/' . check_plain($data->provider_id));
  }
  return $providers;
}

/**
 * Gets repository by an URL. There should be only one repository for an URL.
 *
 * @param $url (string)
 *   The URL of the repository
 *
 * @return (object)
 *   The oaiharvester_providers record belongs to the URL
 */
function _oaiharvester_provider_get_by_url($url) {
  $query = db_select('oaiharvester_providers', 'op');
  $data = $query->fields('op')
            ->condition('op.oai_provider_url', $url)
            ->execute()
            ->fetchObject();
  return $data;
}

/**
 * Gets all provider's name.
 *
 * @return (array)
 *   Associative array of provider id and its name
 */
function oaiharvester_provider_get_all_names() {
  $providers = array();

  $query = db_select('oaiharvester_providers', 'op');
  $result = $query->fields('op')->execute();

  foreach ($result as $data) {
    $providers[$data->provider_id] = $data->name;
  }
  return $providers;
}

/**
 * Gets all providers.
 *
 * @return (array)
 *   List of all OAI harvester provider objects
 */
function oaiharvester_provider_get_all() {
  $providers = array();

  $query = db_select('oaiharvester_providers', 'op');
  $result = $query->fields('op')->orderBy('name')->execute();

  foreach ($result as $data) {
    $providers[] = $data;
  }
  return $providers;
}

/**
 * Gets the URL of a data provider by its identifier.
 *
 * @param $provider_id (int)
 *   The identifier of the data provider
 *
 * @return (string)
 *   The URL of the data provider
 */
function _oaiharvester_provider_get_url_by_id($provider_id) {
  static $provider_cache;

  if (!isset($provider_cache[$provider_id])) {
    $query = db_select('oaiharvester_providers', 'op');
    $result = $query->fields('op', array('oai_provider_url'))
                    ->condition('provider_id', $provider_id)
                    ->execute();

    $data = $result->fetchObject();
    $provider_cache[$provider_id] = $data->oai_provider_url;
  }

  return $provider_cache[$provider_id];
} // _oaiharvester_provider_get_url_by_id

/**
 * Deletes all steps of a schedule by its identifier.
 *
 * @param $harvest_schedule_id (int)
 *  The schedule's identifier
 */
function _oaiharvester_schedule_steps_delete($schedule_id) {
  $result = db_delete('oaiharvester_harvest_schedule_steps')
    ->condition('schedule_id', $schedule_id)
    ->execute();
}

/**
 * Deletes a step.
 *
 * @param $step_id (int)
 *   The step's identifier
 */
function _oaiharvester_schedule_steps_delete_by_stepid($step_id) {
  $result = db_delete('oaiharvester_harvest_schedule_steps')
    ->condition('step_id', $step_id)
    ->execute();
}

/**
 * Deletes a step.
 *
 * @param $step (object)
 *   A oaiharvester_harvest_schedule_steps object
 */
function _oaiharvester_schedule_step_delete_object($step) {
  _oaiharvester_schedule_step_delete($step->schedule_id, $step->format_id, $step->set_id);
}

function _oaiharvester_schedule_step_delete($schedule_id, $format_id, $set_id) {
  $query = db_delete('oaiharvester_harvest_schedule_steps')
    ->condition('schedule_id', $schedule_id)
    ->condition('format_id', $format_id);
  if (empty($set_id)) {
    $query->condition('set_id', NULL);
  }
  else {
    $query->condition('set_id', $set_id);
  }
  $query->execute();
}

/**
 * Sets schedule status to active (running)
 *
 * @param $schedule_id (int)
 *   The ID of the schedule
 *
 * @return (Boolean)
 *   Flag of action's success (true/false)
 */
function oaiharvester_schedule_set_active($schedule_id) {
  return _oaiharvester_schedule_set_status($schedule_id, OAIHARVESTER_STATUS_ACTIVE);
}

/**
 * Sets schedule status to passive (not running)
 *
 * @param $schedule_id (int)
 *   The ID of the schedule
 *
 * @return (Boolean)
 *   Flag of action's success (true/false)
 */
function oaiharvester_schedule_set_passive($schedule_id) {
  return _oaiharvester_schedule_set_status($schedule_id, OAIHARVESTER_STATUS_PASSIVE);
}

/**
 * Sets the status of the schedule (active or passive)
 *
 * @param $schedule_id (int)
 *   The ID of the schedule
 * @param $status (String)
 *   The status of the schedule
 *
 * @return (Boolean)
 *   Flag of action's success (true/false)
 */
function _oaiharvester_schedule_set_status($schedule_id, $status) {
  static $possible_values = array(
    OAIHARVESTER_STATUS_ACTIVE => 1,
    OAIHARVESTER_STATUS_PASSIVE => 1,
  );

  if (!isset($possible_values[$status])) {
    return FALSE;
  }

  $schedule = _oaiharvester_schedule_get($schedule_id);
  $schedule->status = $status;
  $ret_val = drupal_write_record('oaiharvester_harvester_schedules', $schedule, 'harvest_schedule_id');
  if ($ret_val == SAVED_UPDATED) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Gets all steps of a schedule.
 *
 * @param $schedule_id (int)
 *   The schedule's identifier
 *
 * @return (array)
 *   The list of step objects. The properties are:
 *   - step_id (int): the step's identifier
 *   - last_ran (date): the last ran of the step
 *   - format_id (int): the format identifier
 *   - name (string): name of format
 *   - namespace (string): namespace of format
 *   - schema_location (string): the schema location (URL) of the format
 *   - set_id (int): set identifier
 *   - display_name (string): The human readable name of the set.
 *   - description (string): The short description about the set's content.
 *   - set_spec (string): The OAI standard's set specification.
 *   - is_provider_set (int): Flag to indicate whether it is a provider set.
 *   - is_record_set (int): Flag to indicate whether it is a record set.
 */
function _oaiharvester_schedule_steps_get($schedule_id) {
  $steps = array();

  $query = db_select('oaiharvester_harvest_schedule_steps', 'step');
  $query->join('oaiharvester_formats', 'fmt', 'fmt.format_id = step.format_id');
  $query->leftJoin('oaiharvester_sets', 'sets', 'sets.set_id = step.set_id');
  $query->fields('step', array('step_id', 'last_ran'))
        ->fields('fmt')
        ->fields('sets')
        ->condition('schedule_id', $schedule_id);

  $result = $query->execute();

  foreach ($result as $data) {
    $steps[] = $data;
  }
  return $steps;
}

/**
 * Gets the minimal properties of each steps of a schedule.
 *
 * @param $schedule_id (int)
 *   The schedule identifier
 *
 * @return (array)
 *   The list of step objects. The properties are:
 *   - step_id (int): the step's identifier
 *   - last_ran (date): the last ran of the step
 *   - schedule_id (int): the schedule identifier
 *   - format_id (int): the format identifier
 *   - set_id (int): set identifier
 */
function _oaiharvester_schedule_steps_get_atomic($schedule_id) {
  $steps = array();
  $query = db_select('oaiharvester_harvest_schedule_steps', 'ohss');
  $query->fields('ohss', array('step_id', 'schedule_id', 'format_id', 'set_id', 'last_ran'))
        ->condition('schedule_id', $schedule_id);

  $result = $query->execute();

  foreach ($result as $data) {
    $steps[] = $data;
  }
  return $steps;
}

/**
 * Reset the last ran value to null for a given schedule step
 *
 * @param $step_id (int)
 *   The step identifier
 *
 * @return (boolean)
 */
function _oaiharvester_schedule_step_reset($step_id) {
  $result = db_update('oaiharvester_harvest_schedule_steps')
              ->fields(array('last_ran' => NULL))
               ->condition('step_id', $step_id)
               ->execute();
  return $result;
}

/**
 * Gets the atomic steps of a schedule. Each steps has the following fields:
 * step_id, schedule_id, format_id, set_id, last_ran
 *
 * @param $schedule_id (int)
 *   The schedule identifier
 * @param $format_name (string)
 *   The format name (the OAI-PMH format parameter)
 * @param $set_spec (string)
 *   The set specification (OAI-PMH SetSpec parameter)
 *
 * @return (array)
 *   The list of set objects.
 */
function _oaiharvester_schedule_step_get_atomic($schedule_id, $format_name, $set_spec = NULL, $schedule_type = OAIHARVESTER_PROVIDERTYPE_SERVER) {
  $query = db_select('oaiharvester_harvest_schedule_steps', 'step');
  $query->leftJoin('oaiharvester_formats', 'format', 'format.format_id = step.format_id');
  $query->fields('step');

  if ($set_spec != NULL) {
    $query->leftJoin('oaiharvester_sets', 'sets', 'sets.set_id = step.set_id');
  }

  $query->condition('schedule_id', $schedule_id);

  if (!empty($format_name)) {
    $query->condition('format.name', $format_name);
  }

  if ($set_spec != NULL) {
    $query->condition('sets.set_spec', $set_spec);
  }
  else {
    $query->condition('step.set_id', NULL);
  }
  $result = $query->execute();
  $steps = array();
  foreach ($result as $data) {
    $steps[] = $data;
  }
  return $steps;
}

/**
 * Gets schedule by identifier.
 *
 * @param $id (int)
 *   The schedule identifier
 *
 * @return (onject)
 *   The schedule object
 */
function _oaiharvester_schedule_get($id) {
  static $cache;
  if (!isset($cache[$id])) {
    $query = db_select('oaiharvester_harvester_schedules', 'ohs');
    $result = $query->fields('ohs')
                    ->condition('harvest_schedule_id', $id)
                    ->execute();

    $data = $result->fetchObject();
    $cache[$id] = $data;
  }
  return $cache[$id];
}

/**
 * Gets schedule by provider id.
 *
 * @param $id (int)
 *   The ID of data provider
 *
 * @return (Array)
 *   List of links to schedules
 */
function _oaiharvester_schedule_get_by_provider($provider_id) {
  static $cache;
  if (!isset($cache[$provider_id])) {
    $links = array();
    $query = db_select('oaiharvester_harvester_schedules', 'ohs');
    $result = $query->fields('ohs')
                    ->condition('provider_id', $provider_id)
                    ->execute();
    foreach ($result as $data) {
      $links[] = l($data->schedule_name, 'admin/xc/harvester/schedule/' . $data->harvest_schedule_id);
    }
    $cache[$provider_id] = $links;
  }
  return $cache[$provider_id];
}

/**
 * Get all schedule data, and the provider name and URL
 *
 * @param $schedule_id (int)
 *   The schedule ID
 *
 * @return (object)
 *   The schedule's properties including the data provider's properties.
 */
function _oaiharvester_schedule_get_full($schedule_id) {

  static $cache;

  if (!isset($cache[$schedule_id])) {
    $curdate = (db_driver() == 'pgsql') ?  'current_date' : 'CURDATE()';

    $sql = 'SELECT schedule.*,
                   provider.name AS repository_name,
                   provider.oai_provider_url,
                   provider.oai_identifier,
                   provider.type,
                   (DATE(end_date) >= ' . $curdate .') AS active
            FROM {oaiharvester_harvester_schedules} AS schedule
              LEFT JOIN {oaiharvester_providers} AS provider
                     ON provider.provider_id = schedule.provider_id
            WHERE schedule.harvest_schedule_id = :id';
    $data = db_query($sql, array(':id' => $schedule_id))->fetchObject();
    if (!is_null($data)) {
      $data->oai_identifier = unserialize($data->oai_identifier);
    }
    $cache[$schedule_id] = $data;
  }
  return $cache[$schedule_id];
}

/**
 * Gets all schedule data, and the provider name and URL.
 */
function _oaiharvester_schedule_get_all() {
  $curdate = (db_driver() == 'pgsql') ?  'current_date' : 'CURDATE()';

  $sql = 'SELECT schedule.*,
                 provider.name AS repository_name,
                 provider.oai_provider_url,
                 provider.oai_identifier,
                 provider.type,
                 (DATE(end_date) >= ' . $curdate . ') AS active
          FROM {oaiharvester_harvester_schedules} AS schedule
            LEFT JOIN {oaiharvester_providers} AS provider
                   ON provider.provider_id = schedule.provider_id
          ORDER BY name';
  $result = db_query($sql);
  $schedules = array();

  foreach ($result as $data) {
    $data->oai_identifier = unserialize($data->oai_identifier);
    $schedules[] = $data;
  }

  return $schedules;
}

/**
 * Deletes a schedule from the database.
 *
 * @param $schedule_id (int)
 *   The identifier of the schedule
 *
 * @return (boolean)
 *   TRUE if the record were successfully deleted, FALSE if the query was not executed correctly.
 */
function _oaiharvester_schedule_delete($schedule_id) {
  return db_delete('oaiharvester_harvester_schedules')
    ->condition('harvest_schedule_id', $schedule_id)
    ->execute();
}

/**
 * Gets sets of a provider.
 *
 * @param $provider_id (int)
 *   The ID of provider
 *
 * @return (resource)
 *   Database query result resource or FALSE
 */
function _oaiharvester_sets_get_by_provider($provider_id) {
  static $cache;

  if (!isset($cache[$provider_id])) {
    $sql = 'SELECT sets.* '
         .   'FROM {oaiharvester_sets} AS sets, '
         .        '{oaiharvester_sets_to_providers} AS con'
         . ' WHERE sets.set_id = con.set_id'
         .   ' AND con.provider_id = :id';
    $result = db_query($sql, array(':id' => $provider_id));
    $cache[$provider_id] = $result;
  }
  return $cache[$provider_id];
}

/**
 * Gets the metadata formats by a provider.
 *
 * @param $provider_id (int)
 *   The ID of a provider
 *
 * @return (resource)
 *   Database query result resource or FALSE
 */
function _oaiharvester_metadataformats_get_by_provider($provider_id) {
  $sql = 'SELECT formats.*
           FROM {oaiharvester_formats} AS formats,
                {oaiharvester_formats_to_providers} AS con'
       . ' WHERE formats.format_id = con.format_id'
       .   ' AND con.provider_id = :id';
  return db_query($sql, array(':id' => $provider_id));
}

/**
 * Gets the whole queue.
 *
 * @return (resource)
 *   Database query result resource or FALSE
 */
function _oaiharvester_queue_get() {
  $sql = 'SELECT * FROM {oaiharvester_harvest_queue}';
  return db_query($sql);
}

function _oaiharvester_queue_count() {
  return db_query('SELECT count(*) FROM {oaiharvester_harvest_queue}')->fetchField();
}

/**
 * Gets a queue item by its ID.
 *
 * @param $queue_id (int)
 *   The queue identifier
 *
 * @return (object)
 *   A oaiharvester_harvest_queue table record
 */
function _oaiharvester_queue_get_item_by_id($queue_id) {
  $sql = 'SELECT * FROM {oaiharvester_harvest_queue} WHERE harvest_id = :id';
  return db_query($sql, array(':id' => $queue_id))->fetchObject();
}

/**
 * Deletes a queue by identifier.
 *
 * @param $queue_id (int)
 *   The queue identifier
 */
function _oaiharvester_queue_delete_item_by_id($queue_id) {
  return db_delete('oaiharvester_harvest_queue')
  ->condition('harvest_id', $queue_id)
  ->execute();
}

/**
 * Deletes all queues.
 */
function _oaiharvester_queue_delete_all() {
  return db_delete('oaiharvester_harvest_queue')->execute();
}

/**
 * Returns a queue item connected to a given schedule.
 *
 * @param $schedule_id (int)
 *   The schedule ID
 * @param $format (string)
 *   The metadata format's name to harvest
 * @param $set (string)
 *   The set spec to harvest
 *
 * @return (resource)
 *   A database query result resource, or FALSE if the query was not executed
 *   correctly.
 */
function _oaiharvester_queue_get_item_by_schedule($schedule_id, $format_name,
    $set_spec) {
  $sql = 'SELECT * FROM {oaiharvester_harvest_queue}
            WHERE harvest_schedule_id = :sid
              AND metadata_prefix = :fname';
  if (!empty($set)) {
    $sql .= ' AND set_name = :setspec';
    return db_query($sql, array(':sid' => $schedule_id, ':fname' => $format_name, ':setspec' => $set_spec));
  }
  $sql .= ' AND set_name IS NULL';
  return db_query($sql, array(':sid' => $schedule_id, ':fname' => $format_name));
}
