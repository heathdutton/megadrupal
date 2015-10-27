<?php
/**
 * @file
 * All adlibapi functions that are realy meant as api functions
 * can be found in this file.
 */

/**
 * Get available adlib databases.
 *
 * @return array
 *   Returns an array containing all adlib databases
 *   for which settings has been saved
 */
function adlibapi_api_get_available_databases() {
  $databaselist = array();
  $databases = ctools_export_load_object(ADLIBAPI_DATABASE_SCHEMA);
  foreach ($databases as $database) {
    adlibapi_deserialize_config($database);
    $databaselist[$database->name] = $database;
  }
  return $databaselist;
}

/**
 * Get adlib database settings.
 *
 * Get database based on the database preset name.
 *
 * @param string $databasename
 *   Database preset name.
 */
function adlibapi_api_get_database_settings($databasename) {
  $database = ctools_export_load_object(ADLIBAPI_DATABASE_SCHEMA, 'names', array($databasename));
  adlibapi_deserialize_config($database[$databasename]);
  // Unset the config, as it contains now useless data.
  unset($database[$databasename]->config);
  return $database[$databasename];
}

/**
 * Get metadata.
 *
 * Get metadata about the fields.
 * From the adlib api docu:
 * Note that field names can be multilingual.
 * When you use wwwopac.ashx to perform a search,
 * you should use either the field tag (case-sensitive)
 * or the English (language 0) field name.
 *
 * @param object $database_config
 *   Object containing databases configuration. url en database are needed.
 */
function adlibapi_api_get_metadata($database_config) {
  // Create adlibconnector.
  $conn = adlibapi_api_get_connection($database_config);
  $response = $conn->getMetadata();
  $metadata = $response->getXMLObject();
  return $metadata;
}

/**
 * Get a list of fields.
 *
 * This is actualy the same as metadata.
 * TODO: choose which function to keep.
 *
 * @param object $database_config
 *   Object containing databases configuration. Url en database are needed.
 *
 * @return array
 *   Array containing all the fields available in the database.
 */
function adlibapi_api_get_fieldList($database_config, $mode = 'raw') {
  $conn = adlibapi_api_get_connection($database_config);
  $return_fields = array();
  if (isset($conn) && $conn) {
    $response = $conn->getFieldList();
    $field_list = $response->getXMLArray();
    switch ($mode) {
      case 'raw':
        $return_fields = $field_list;
        break;

      case 'simple_array':
        $fields = array();
        foreach ($field_list as $field) {
          $fields[$field['fieldName']] = $field['displayName'];
        }
        $return_fields = $fields;
        break;

      default:
        $return_fields = FALSE;
        break;
    }
  }
  return $return_fields;
}

/**
 * Get all indexed fields from the database.
 *
 * @param object $database_config
 *   Object containing databases configuration. Url en database are needed.
 *
 * @return array
 *   Array containing all the fields available in the database.
 */
function adlibapi_api_get_indexedfields($database_config) {
  $conn = adlibapi_api_get_connection($database_config);
  $response = $conn->getFieldList();
  $field_list = $response->getXMLArray();
  $fields = array();
  foreach ($field_list as $field) {
    if ($field['isIndexed']) {
      $fields[$field['fieldName']] = $field['displayName'];
    }
  }
  return $fields;
}

/**
 * Get the adlib database version info.
 *
 * @param object $database_config
 *   Database configuration from database settings.
 */
function adlibapi_api_get_version($database_config) {
  $conn = new AdLibConnector($database_config->url);
  $response = $conn->getVersion();
  if ($response->getError()) {
    watchdog(ADLIBAPI_WATCHDOG_ERROR, 'Adlib api error: %error', array('%error' => $response->getErrorMessage()), WATCHDOG_ERROR);
  }
  $version_info = $response->getXMLObject();
  $raw = $response->getRaw();
  $version = array();
  // Check if there is a hit.
  if (isset($version_info->diagnostic->hits) && (integer) $version_info->diagnostic->hits > 0) {
    $version['rawtext'] = (string) $version_info->recordList->record->version;
    // Parse the string.
    $elements = explode(', ', $version['rawtext']);
    foreach ($elements as $element) {
      $parts = explode('=', $element);
      if (isset($parts[1])) {
        $version[$parts[0]] = $parts[1];
      }
      else {
        $version[$parts[0]] = $parts[0];
      }
    }
  }
  return $version;
}

/**
 * Get an adlib format date from several dateformats.
 *
 * @param array $date
 *   A key value array, containing one value:
 *  'timestamp' => unix timestamp.
 *
 * @return string
 *   Adlid date formatted date.
 */
function adlibapi_api_get_adlib_date($date) {
  if (isset($date['timestamp'])) {
    $adlib_date = format_date($date['timestamp'], 'custom', 'Y-m-d');
    return $adlib_date;
  }
  if (isset($date['custom'])) {
    $adlib_date = format_date(strtotime($date['custom']), 'custom', 'Y-m-d');
    return $adlib_date;
  }
  return FALSE;
}

/**
 * Get all databaserecords which are altered after a specific date.
 *
 * @param object $database_config
 *   Database config from settings.
 * @param string $date
 *   Date formatted for adlib.
 *
 * @return object
 *   Response of the call.
 */
function adlibapi_api_get_alteredrecords_by_date($database_config, $date) {
  $conn = adlibapi_api_get_connection($database_config);
  $response = $conn->getAlteredRecordsByDate($date);
  return $response;
}

/**
 * Get base query using dates.
 *
 * Get a base query to get databaserecords which are altered
 * after a specific date.
 *
 * @param string $date
 *   Date formatted for adlib
 */
function adlibapi_api_getquery_altered_records_by_date($date) {
  $query = new AdlibSearchQuery();
  $query->addParameter('modification', $date, ' > ');
  $query->addParameter('sort', 'edit.date', ' ', '');
  return $query;
}

/**
 * Generic function to built different types of queries.
 *
 * @param string $type
 *   The type of query to build, possible values:
 *         - 'by_date'        create a query to get items by date comparison.
 * @param array $queryoptions
 *   Asiociatvie array with the optins which must be added, possible keys:
 *         - 'date'         a valid date
 *         - 'date_type'    the date type (either modification or creation)
 *         - 'sortfield'     one of the adlib database fields
 */
function adlibapi_api_build_query($type, $queryoptions) {
  $query = new AdlibSearchQuery();
  switch ($type) {
    case 'by_date':
      if (isset($queryoptions['date'])) {
        /*
         *  TODO: must $queryoptions['date'] be passed
         *  to 'adlibapi_api_get_adlib_date()'?
         */
        $date = $queryoptions['date'];
        $date_type = $queryoptions['date_type'];
        $query->addParameter($date_type, $date, '>');
      }
      if (isset($queryoptions['sortfield'])) {
        $query->addParameter('sort', $queryoptions['sortfield']);
      }
      break;
  }
  return $query;
}

/**
 * Get an image query object.
 *
 * @param string $imagename
 *   Filename of the image.
 *
 * @return AdlibImageQuery
 *   AdlibImageQuery object.
 */
function adlibapi_api_get_image_query($imagename) {
  $query = new AdlibImageQuery($imagename);
  return $query;
}

/**
 * Get an adlibconnector from database configuration.
 *
 * @param object $database_config
 *   Database configuration from settings.
 *
 * @return AdLibConnector
 *   A new AdLibConnector.
 */
function adlibapi_api_get_connection($database_config) {
  if (isset($database_config)) {
    if (!isset($database_config->database) || $database_config->database == '') {
      // The database info is not set.
      // Create a link for the user to correct this.
      $path = 'admin/structure/adlibapi/list/' . $database_config->name . '/edit';
      $link = l(t('database settings page'), $path);
      drupal_set_message(t('The database in the selected adlib database settings was not set! Be sure to select or fill out a database on the !link.', array('!link' => $link)), 'error');
      return FALSE;
    }
    else {
      $conn = new AdLibConnector($database_config->url, $database_config->database, $database_config->imageserver);
      return $conn;
    }
  }
  return FALSE;
}
