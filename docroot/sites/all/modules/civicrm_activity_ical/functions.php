<?php
/*
 * @file
 * Contains functions that are not Drupal or CiviCRM hooks
 */

/**
 * Gets content for the page showing the contact's feed URL.
 */
function civicrm_activity_ical_user_page() {

  $contact_id = civicrm_activity_ical_get_id('contact_id');

  $url = civicrm_activity_ical_get_feed_url($contact_id);

  $alt = t('Add to Google Calendar');
  
  $page = '<p>'
    . t('Your CiviCRM assigned activities feed is available at this URL:')
    . "<br /><input type=\"text\" size=\"120\" id=\"civicrm_activity_ical_url\" value=\"$url\" readonly=\"readonly\"/></p>"
    . '<a href="http://www.google.com/calendar/render?cid='. urlencode($url) .'" target="_blank"><img src="//www.google.com/calendar/images/ext/gc_button6.gif" border="0" alt="'. $alt .'" title="'. $alt .'"></a>'
  ;

  $page .= drupal_render(drupal_get_form('civicrm_activity_ical_settings_user_form'));

  // Select the URL field text automatically
  $js = "
    Drupal.behaviors.civicrm_activity_ical_url = {
      attach: function(context, settings) {
        jQuery('#civicrm_activity_ical_url').select();
      }
    }
  ";
  drupal_add_js($js, 'inline');

  return $page;
}

/**
 * Form constructor for the module settings form.
 *
 * @ingroup forms
 */
function civicrm_activity_ical_admin_settings_form() {

  $form = array();

  $form['civicrm_activity_ical_cache_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache interval'),
    '#default_value' => variable_get('civicrm_activity_ical_cache_interval', 30),
    '#description' => t('Length of time (in minutes) for which iCalendar feed data should be cached.'),
    '#size' => 60,
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Form constructor for the "Rebuild feed URL" form.
 *
 * @ingroup forms
 */
function civicrm_activity_ical_settings_user_form($form, &$form_state) {
  $form = array();

  $form['rebuild'] = array(
    '#type' => 'fieldset',
    '#description' =>
      '<p>'
      . t('Anyone who knows your feed URL will be able to view your activities. If you think this URL is known by people who should not have that access, you can rebuild a new URL, so that any existing URL no longer works.')
      . '</p><p>'
      . t("Note: This will cause the existing URL to stop working, so if you're already using the URL in your calendar software (Google Calendar, etc.), you'll need to update that software with the new URL.")
      .'</p>'
    ,
  );
  $form['rebuild']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Rebuild feed URL',
  );
  return $form;
}

/**
 * Submit handler for the module settings form.
 *
 * @ingroup forms
 */
function civicrm_activity_ical_settings_user_form_submit($form, &$form_state) {
  $contact_id = civicrm_activity_ical_get_id('contact_id');
  civicrm_activity_ical_make_contact_key($contact_id);

  drupal_set_message(t('Activities iCal feed URL has been changed. Be sure to update any calendar software configurations that were using the old URL.'));
}

/**
 * Generates an iCalendar feed for the given contact, if the given key is valid.
 */
function civicrm_activity_ical_view_feed($contact_id, $key) {
  // Validate user key
  if ($key != civicrm_activity_ical_get_contact_key($contact_id)) {
    drupal_access_denied();
    return;
  }

  civicrm_activity_ical_print_cache($contact_id);
}

/**
 * Checks for a valid feed cache for the given contact, and rebuilds that cache
 * if no valid feed cache is found.
 */
function civicrm_activity_ical_print_cache($contact_id) {
  require_once drupal_get_path('module', 'civicrm_activity_ical') .'/cache.php';

  // Initialize CiviCRM, including its dynamic include path.
  civicrm_initialize();

  // Check for valid cache.
  $cache = CivicrmActivityIcalCache::get($contact_id);
  if ($cache === FALSE) {
    // Require a file from CiviCRM's dynamic include path.
    require_once 'CRM/Core/Smarty.php';
    $tpl = CRM_Core_Smarty::singleton();
    $tpl->assign('activities', civicrm_activity_ical_get_feed_data($contact_id));

    // Assign base_url to be used in links.
    global $base_url;
    $tpl->assign('base_url', $base_url);

    // Calculate and assign the domain for activity uids
    $domain = parse_url('http://'. $_SERVER['HTTP_HOST'], PHP_URL_HOST);
    $tpl->assign('domain', $domain);

    $template = dirname(__FILE__) . '/templates/ICal.tpl';
    $output = $tpl->fetch($template);

    CivicrmActivityIcalCache::set($contact_id, $output);
    $time = time();
  }
  else {
    $output = $cache['cache'];
    $time = $cache['cached'];
  }

  header('Last-Modified: ' . gmdate('D, d M Y H:i:s \G\M\T', $time));
  // Require a file from CiviCRM's dynamic include path.
  require_once 'CRM/Utils/ICalendar.php';
  CRM_Utils_ICalendar::send($output);
  exit;
}


/**
 * Gets the correct key for the given contact, or for the contact matching the
 * current user if no Contact ID is given.
 */
function civicrm_activity_ical_get_contact_key($contact_id = NULL) {

  if (NULL === $contact_id) {
    $contact_id = civicrm_activity_ical_get_id('contact_id');
  }

  $key = db_select('civicrm_activity_ical', 'i')
    ->fields('i', array('feed_key'))
    ->condition('cid', $contact_id)
    ->execute()
    ->fetchField();
  
  if ($key) {
    return $key;
  }
  else {
    return civicrm_activity_ical_make_contact_key($contact_id);
  }
}

/**
 * Creates a new key and associates it with the given contact.
 *
 * @return string The new key
 */
function civicrm_activity_ical_make_contact_key($contact_id) {
  $key = md5(mt_rand(0, 10000000) . microtime());

  db_merge('civicrm_activity_ical')
    ->key(array('cid' => $contact_id))
    ->fields(
      array(
        'cid' => $contact_id,
        'feed_key' => $key,
      )
    )
    ->execute();
  
  return $key;
}

/**
 * For the given contact, gets the most current activity data from the CiviCRM
 * database.
 *
 * @return Array An array of activity data suitable for parsing in the calendar
 */
function civicrm_activity_ical_get_feed_data($contact_id) {
  $return = array();

  // Initialize CiviCRM, including its dynamic include path.
  civicrm_initialize();

  global $base_url;

  // Set up placeholders for CiviCRM query. CiviCRM's query method doesn't
  // have anything like Drupals db_placeholders, so we do it ourselves here.
  $placeholders = $params = array();
  $placeholders['status'] = array();
  $placeholder_count = 1;

  // Placeholders for blocked statuses
  $blocked_status_values = variable_get('civicrm_activity_ical_blocked_status_values', array());
  if (empty($blocked_status_values)) {
    $blocked_status_values[] = 0;
  }
  foreach ($blocked_status_values as $value) {
    $i = $placeholder_count++;
    $placeholders['status'][] = '%' . $i;
    $params[$i] = array(
      $value,
      'Integer',
    );
  }

  // Placeholder for contact_id
  $i = $placeholder_count++;
  $placeholders['contact_id'] = '%' . $i;
  $params[$i] = array(
    $contact_id,
    'Integer',
  );

  if (_civicrm_activity_ical_compare_civicrm_version('4.4') > -1) {
    $query = "
      SELECT
        contact_primary.id as contact_id,
        civicrm_activity.id,
        source.id AS source_id,
        source.display_name AS `source_display_name`,
        GROUP_CONCAT(
          DISTINCT
          other_assignee.display_name
          SEPARATOR '; '
        ) AS other_assignees,
        GROUP_CONCAT(
          DISTINCT
          target.display_name
          SEPARATOR '; '
        ) AS targets,
        civicrm_activity.activity_type_id,
        activity_type.label AS activity_type,
        civicrm_activity.subject AS activity_subject,
        civicrm_activity.activity_date_time AS activity_date_time,
        civicrm_activity.duration AS activity_duration,
        civicrm_activity.location AS activity_location,
        civicrm_activity.details AS activity_details
      FROM civicrm_contact contact_primary
        INNER JOIN civicrm_activity_contact activity_assignment
          ON (
            activity_assignment.contact_id = contact_primary.id
            AND activity_assignment.record_type_id = 1
          )
        INNER JOIN civicrm_activity
          ON (
            civicrm_activity.id = activity_assignment.activity_id
            AND civicrm_activity.is_deleted = 0
            AND civicrm_activity.is_current_revision = 1
          )
        INNER JOIN civicrm_activity_contact activity_source
          ON (
            activity_source.activity_id = civicrm_activity.id
            AND activity_source.record_type_id = 2
          )
        INNER JOIN civicrm_contact source ON source.id = activity_source.contact_id
        INNER JOIN civicrm_option_group option_group_activity_type
          ON (option_group_activity_type.name = 'activity_type')
        INNER JOIN civicrm_option_value activity_type
          ON (
            civicrm_activity.activity_type_id = activity_type.value
            AND option_group_activity_type.id = activity_type.option_group_id
          )
        LEFT JOIN civicrm_activity_contact other_activity_assignment
          ON (
            civicrm_activity.id = other_activity_assignment.activity_id
            AND other_activity_assignment.record_type_id = 1
          )
        LEFT JOIN civicrm_contact other_assignee
          ON other_activity_assignment.contact_id = other_assignee.id
          AND other_assignee.is_deleted = 0
          AND other_assignee.id <> contact_primary.id
        LEFT JOIN civicrm_activity_contact activity_target
          ON (
            civicrm_activity.id = activity_target.activity_id
            AND activity_target.record_type_id = 3
          )
        LEFT JOIN civicrm_contact target ON activity_target.contact_id = target.id
      WHERE 
        civicrm_activity.status_id NOT IN
          (". implode(',', $placeholders['status']) . ")
        AND contact_primary.id = '{$placeholders['contact_id']}'
        AND civicrm_activity.is_test = 0
      GROUP BY civicrm_activity.id
      ORDER BY activity_date_time desc
    ";
  }
  else {
    $query = "
      SELECT
      contact_primary.id as contact_id,
      civicrm_activity.id,
      source.id AS source_id,
      source.display_name AS `source_display_name`,
      GROUP_CONCAT(
        DISTINCT
        other_assignee.display_name
        SEPARATOR '; '
      ) AS other_assignees,
      GROUP_CONCAT(
        DISTINCT
        target.display_name
        SEPARATOR '; '
      ) AS targets,
      civicrm_activity.activity_type_id,
      activity_type.label AS activity_type,
      civicrm_activity.subject AS activity_subject,
      civicrm_activity.activity_date_time AS activity_date_time,
      civicrm_activity.duration AS activity_duration,
      civicrm_activity.location AS activity_location,
      civicrm_activity.details AS activity_details
      FROM civicrm_contact contact_primary
      INNER JOIN civicrm_activity_assignment activity_assignment
        ON activity_assignment.assignee_contact_id = contact_primary.id
      INNER JOIN civicrm_activity
        ON (
          civicrm_activity.id = activity_assignment.activity_id
          AND civicrm_activity.is_deleted = 0
          AND civicrm_activity.is_current_revision = 1
        )
      INNER JOIN civicrm_contact source
        ON source.id = civicrm_activity.source_contact_id
      INNER JOIN civicrm_option_group option_group_activity_type
        ON (option_group_activity_type.name = 'activity_type')
      INNER JOIN civicrm_option_value activity_type
        ON (
          civicrm_activity.activity_type_id = activity_type.value
          AND option_group_activity_type.id = activity_type.option_group_id
        )
      LEFT JOIN civicrm_activity_assignment other_activity_assignment
        ON civicrm_activity.id = other_activity_assignment.activity_id
      LEFT JOIN civicrm_contact other_assignee 
        ON (
          other_activity_assignment.assignee_contact_id = other_assignee.id
          AND other_assignee.is_deleted = 0
          AND other_assignee.id <> contact_primary.id
        )
      LEFT JOIN civicrm_activity_target activity_target
        ON civicrm_activity.id = activity_target.activity_id
      LEFT JOIN civicrm_contact target
        ON activity_target.target_contact_id = target.id

      WHERE
        civicrm_activity.status_id NOT IN
          (". implode(',', $placeholders['status']) . ")
        AND contact_primary.id = '{$placeholders['contact_id']}'
        AND civicrm_activity.is_test = 0

      GROUP BY civicrm_activity.id
      ORDER BY activity_date_time DESC
    ";
  }
  // Require a file from CiviCRM's dynamic include path.
  require_once 'CRM/Core/DAO.php';
  $dao = CRM_Core_DAO::executeQuery($query, $params);

  while ($dao->fetch()) {
    $row = $dao->toArray();

    $description = array();
    if ($row['activity_details']) {
      $description[] = preg_replace('/(\n|\r)/', '', $row['activity_details']);
    }
    if ($row['targets']) {
      $description[] = 'With: '. $row['targets'];
    }
    if ($row['other_assignees']) {
      $description[] = 'Other assignees: '. $row['other_assignees'];
    }
    $row['description'] = implode("\n", $description);

    $row['activity_date_time'] = civicrm_activity_contact_datetime_to_utc($row['activity_date_time'], $contact_id);

    $return[] = $row;
  }
  return $return;
}

/**
 * Confirms access for the Drupal user matching the given contact.
 */
function civicrm_activity_ical_feed_access($contact_id) {
  $uid = civicrm_activity_ical_get_id('uf_id', $contact_id);
  $account = user_load($uid);
  return user_access('access CiviCRM activity iCalendar feed', $account);
}

/**
 * Given an ID for a Drupal user or CiviCRM contact, gets the corresponding
 * "UF Match" using CiviCRM APIs. Uses static caching to avoid redundant
 * operations.
 *
 * @param $return
 *   The type of data to return. Either 'contact_id' to get a CiviCRM contact_id,
 *   or 'uf_id' to get a Drupal uid.
 * @param $id
 *   The known ID. If $return is 'contact_id', this is a Drupal uid. If $return
 *   is 'uf_id', this is required and must be a CiviCRM contact_id.  If NULL,
 *   (only allowed if $return is 'contact_id'), the current Drupal user's uid is
 *   used.
 * @param $domain_id
 *   The CiviCRM domain (multisite) ID to use in querying for the data. If NULL,
 *   the current CiviCRM domain ID is used.
 *
 * @return CiviCRM contact_id on success, or NULL on failure.
 */
function civicrm_activity_ical_get_id($return = '', $id = NULL, $domain_id = NULL) {
  $valid_returns = array(
    'contact_id',
    'uf_id',
  );
  if (!in_array($return, $valid_returns)) {
    $args = array(
      '%return' => $return,
    );
    watchdog(
      'civicrm_activity_ical',
      'Invalid return type %return requested.',
      $args
    );
    return NULL;
  }

  static $cache = array();

  if (NULL === $id) {
    if ($return == 'contact_id') {
      global $user;
      $id = $user->uid;
    }
    elseif ($return == 'uf_id') {
      $args = array(
        '%function' => __FUNCTION__,
      );
      watchdog(
        'civicrm_activity_ical',
        'Missing required $id parameter when searching for uf_id in %function',
        $args
      );
      return NULL;
    }
  }
  if (NULL === $domain_id) {
    $domain_id = civicrm_activity_ical_get_domain_id();
  }

  $cache_key = "{$return}.{$id}.{$domain_id}";

  if (!isset($cache[$cache_key])) {
    $cache[$cache_key] = NULL;

    if ($return == 'uf_id') {
      $id_param_name = 'contact_id';
    }
    else {
      $id_param_name = 'uf_id';
    }

    // Initialize CiviCRM, including its dynamic include path.
    civicrm_initialize();
    require_once('api/api.php');
    $api_params = array(
      'version' => 3,
      $id_param_name => $id,
      'domain_id' => $domain_id,
      'return' => $return,
    );
    $result = civicrm_api('uf_match', 'getvalue', $api_params);
    if (is_array($result) && $result['is_error']) {
      $args = array(
        '%return' => $return,
        '%uid' => $id,
        '%domain_id' => $domain_id,
        '%msg' => $result['error_message']
      );
      watchdog(
        'civicrm_activity_ical', 
        'Could not get %return for system uid %uid and CiviCRM domain_id %domain_id. Error message: %msg',
        $args
      );
    }
    else {
      $cid = $result;
      if (is_numeric($cid)) {
        $cache[$cache_key] = $cid;
      }
    }
  }
  return $cache[$cache_key];
}

/**
 * Gets the correct feed URL for the given contact.
 */
function civicrm_activity_ical_get_feed_url($contact_id) {
  global $base_url;
  $key = civicrm_activity_ical_get_contact_key($contact_id);
  return $base_url . "/civicrm_activity_ical/feed/$contact_id/$key";
}

/**
 * Sets module variables to default values().
 */
function civicrm_activity_ical_set_default_variables() {
  variable_set('civicrm_activity_ical_cache_interval', 30);

  // Initialize CiviCRM, including its dynamic include path.
  civicrm_initialize();
  // Require a file from CiviCRM's dynamic include path.
  require_once 'CRM/Core/OptionGroup.php';
  $values = CRM_Core_OptionGroup::values('activity_status');
  $blocked_statuses = array(
    'Completed',
    'Cancelled',
    'Left Message',
    'Unreachable',
    'Not Required',
  );
  $blocked_status_values = array_keys(array_intersect($values, $blocked_statuses));
  variable_set('civicrm_activity_ical_blocked_status_values', $blocked_status_values);
}

/**
 * Returns HTML for the module's 'quick_links' block.
 *
 * @ingroup themeable
 */
function theme_civicrm_activity_ical_quick_links() {
  return l(t('Feed details'), 'civicrm_activity_ical/settings/user');
}

/**
 * Gets the current CiviCRM domain (multisite) ID.
 * 
 * @return Integer domain ID
 */
function civicrm_activity_ical_get_domain_id() {
  static $domain_id;
  if (!isset($domain_id)) {
    // Initialize CiviCRM, including its dynamic include path.
    civicrm_initialize();
    $domain_id = CRM_Core_Config::domainID();
  }
  return $domain_id;
}

/**
 * Gets the Drupal timezone value for the given user, or for the current
 * user if no uid is given. Based on drupal_get_user_timezone(), except that
 * this function operates for any user, whereas drupal_get_user_timezone() only
 * works for the current user.
 */
function civicrm_activity_get_user_timezone($uid = NULL) {
  $return = NULL;

  // If timezones can be configurable per user, get the user's timezone setting.
  if (variable_get('configurable_timezones', 1)) {
    // Get the global user if no uid is given.
    if ($uid === NULL) {
      global $user;
    }
    else {
      $user = user_load($uid);
    }
    // Get the user's timezone setting, if any.
    if ($user->uid && $user->timezone) {
      $return = $user->timezone;
    }
  }

  // If no timezone has been found yet, get the system timezone.
  if (!$return) {
    // Use @ operator to ignore PHP strict notice if time zone has not yet been
    // set in the php.ini configuration.
    $return = variable_get('date_default_timezone', @date_default_timezone_get());
  }

  return $return;
}


/**
 * Converts the given date-time to UTC, adjusting for the timezone of the contact.
 * Contact timezone is assumed from the Drupal timezone setting of the Drupal
 * user associated with the contact. If no such user exists, no conversion is
 * done and the given date-time value is returned as given.
 *
 * @param string $activity_date_time
 *  The date-time value to be converted. Must be suitable for passing through
 *  strtotime().
 * @param int $contact_id
 *  The CiviCRM contact ID of the given contact.
 *
 * @return string
 *  The converted date-time value in the format 'Y-m-d H:i:s'; or, if the given
 *  contact has no associated Drupal user, the given date-time value, as given.
 */
function civicrm_activity_contact_datetime_to_utc($date_time, $contact_id) {
  // Get the uid of the given contact.
  $uid = civicrm_activity_ical_get_id('uf_id', $contact_id);
  if (!$uid) {
    // If there's no matching user, just return unmodified.
    return $date_time;
  }

  // Create a new DateTimeZone object with the user's timezone setting.
  $dateTimeZone = new DateTimeZone(civicrm_activity_get_user_timezone($uid));
  // Create a new DateTime object for the given date-time in the user's TZ.
  $dateTime = new DateTime($date_time, $dateTimeZone);
  // Calculate the timezone offset for the user's timezone at the given date-time.
  $timeOffset[$contact_id] = $dateTimeZone->getOffset($dateTime);
  // Shift the given time by the opposite of the timezone offset to get the time
  // in UTC.
  $ret = date("Y-m-d H:i:s", strtotime($date_time) + ($timeOffset[$contact_id] * -1));
  return $ret;
}

function _civicrm_activity_ical_get_civicrm_version() {
  civicrm_initialize();
  $query = "SELECT version FROM civicrm_domain";
  $civicrm_version = CRM_Core_DAO::singleValueQuery($query);
  return $civicrm_version;
}

/**
 * Compare a given CiviCRM version identifier to the current CiviCRM version.
 * Comparison is numeric, treating both versions as floats.  Thus, "3.4.1" is
 * greater than "3.4", and "3.4.0" is equal to "3.4".
 *
 * @param $version CiviCRM version, as floating number.
 *
 * @return 1 if current version is greater than $version;
 *  0 if current version is equal to $version;
 *  -1 if current version is less than $version.
 */
function _civicrm_activity_ical_compare_civicrm_version($version) {
  $db_version = _civicrm_activity_ical_get_civicrm_version();
  if ($db_version > $version) {
    return 1;
  }
  elseif ($db_version == $version) {
    return 0;
  }
  elseif ($db_version < $version) {
    return -1;
  }
}
