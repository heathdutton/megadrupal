<?php
/**
 * @file
 * Functions related to connecting to Jabber.
 */

/**
 * Format the Jabber group or login name.
 *
 * @param string $name
 *   The name to clean.
 *
 * @return string
 *   The cleaned Jabber group or login name.
 */
function eve_api_jabber_format_name($name) {
  $name = strtolower($name);
  $name = str_replace(" ", "_", $name);
  $name = str_replace("'", "_", $name);

  if (strlen($name) > 50) {
    $name = substr($name, 0, 50);
  }

  return check_plain($name);
}

/**
 * Format the Jabber display name.
 *
 * @param string $name
 *   The name to clean.
 *
 * @return string
 *   The users Jabber display name.
 */
function eve_api_jabber_format_display_name($name) {
  $name = str_replace("'", "_", $name);

  if (strlen($name) > 50) {
    $name = substr($name, 0, 50);
  }

  return $name;
}

/**
 * Prepares a URL query to do various tasks through the Jabber User Service.
 *
 * @param string $type
 *   What we are doing.
 * @param string $character_name
 *   The login name of the user we are modifying..
 * @param array $query
 *   (optional) Additional options as needed for each $type.
 *
 * @return mixed
 *   Returns a true on success, false on failure or message.
 */
function eve_api_jabber_url_query($type, $character_name = '', array $query = array()) {
  if (!variable_get('eve_api_jabber_enable', FALSE) && $type != 'check') {
    throw new EveApiException(t('Jabber Connection disabled.'));
  }

  try {
    switch ($type) {
      case 'enable':
      case 'disable':
      case 'delete':
        if (empty($character_name)) {
          return FALSE;
        }

        $query = array();
        $default_query = array(
          'type' => $type,
          'secret' => variable_get('eve_api_jabber_secret', 'Password'),
          'username' => eve_api_jabber_format_name($character_name),
        );
        break;

      case 'add':
      case 'update':
      case 'add_roster':
      case 'update_roster':
      case 'delete_roster':
        if (empty($query) || empty($character_name)) {
          return FALSE;
        }

        $default_query = array(
          'type' => $type,
          'secret' => variable_get('eve_api_jabber_secret', 'Password'),
          'username' => eve_api_jabber_format_name($character_name),
        );
        break;

      case 'check':
        $query = array();
        $default_query = array(
          'secret' => variable_get('eve_api_jabber_secret', 'Password'),
        );
        break;

      default:
        return FALSE;
    }

    $query = array_merge($query, $default_query);
    $query = drupal_http_build_query($query);

    $url = variable_get('eve_api_jabber_url', 'http://localhost:9090') . '/plugins/userService/userservice?' . $query;
    $result = eve_api_jabber_url_send_query($url);

    if ($result == 'ok') {
      return TRUE;
    }
    else {
      return $result;
    }
  }
  catch (PDOException $e) {
    db_set_active();
    throw new EveApiException(check_plain($e->getMessage()));
  }
}

/**
 * Sends the URL query to do various tasks through the Jabber User Service.
 *
 * @param string $url
 *   URL to send to the Jabber User Service.
 *
 * @return mixed
 *   Returns the XML object on success, NULL on error.
 */
function eve_api_jabber_url_send_query($url) {
  $has_curl = function_exists('curl_init');
  $open_basedir = ini_get('open_basedir');

  $options = array(
    'method' => 'GET',
  );

  if (!$has_curl || $open_basedir) {
    $html = drupal_http_request($url, $options);
  }
  else {
    $html = eve_api_curl_http_request($url, $options);
  }

  if (!empty($html->error)) {
    watchdog('eve_api_jabber', 'EVE API returned an error with the HTTP request. Error: ' . check_plain($html->error), NULL, WATCHDOG_ERROR);
  }

  if ($html->code == 200) {
    try {
      $xml = @new SimpleXMLElement($html->data);
    }
    catch (Exception $e) {
      watchdog('eve_api_jabber', 'EVE API returned an error with the data retrieved. Error: ' . check_plain($e->getMessage()), NULL, WATCHDOG_ERROR);
      $xml = NULL;
    }
  }
  else {
    $xml = NULL;
  }

  return $xml;
}
