<?php
/**
 * @file
 * Contains rules related hooks.
 */

/**
 * Implements hook_rules_action_info().
 */
function bassets_server_rules_action_info() {
  $action = array();
  $action['rip_merge_iptc_creation_date_time'] = array(
    'group' => 'Bassets',
    'label' => t('Merges the IPTC creation date/time'),
    'named parameter' => TRUE,
    'base' => 'bassets_server_rule_action_iptc_date_time',
    'parameter' => array(
      'iptc_creation_date' => array(
        'label'    => t('IPTC creation date'),
        'type'     => 'text',
        'optional' => FALSE,
      ),
      'iptc_creation_time' => array(
        'label'    => t('IPTC creation time'),
        'type'     => 'text',
        'optional' => FALSE,
      ),
    ),
    'provides' => array(
      'iptc_new_date' => array(
        'label' => t('Merged IPTC creation date/time'),
        'type'  => 'date',
      ),
    ),
  );
  return $action;
}

/**
 * Rule action callback to merge the IPTC date / time values into a ISO date.
 */
function bassets_server_rule_action_iptc_date_time($values) {
  // Format: CCYYMMDD.
  $date = $values['iptc_creation_date'];
  // Format: HHMMSS+HHMM
  $date_time = $values['iptc_creation_time'];
  if (preg_match('/\d\d\d\d\d\d[-+]\d\d\d\d/', $date_time) && preg_match('/\d\d\d\d\d\d\d\d/', $date)) {
    // Target format 2004-02-12T15:19:21+00:00.
    $year = substr($date, 0, 4);
    $month = substr($date, 4, 2);
    $day = substr($date, 6, 2);
    if ($year !== '0000' && $month !== '00' && $day !== '00') {
      $hours = substr($date_time, 0, 2);
      $minutes = substr($date_time, 2, 2);
      $seconds = substr($date_time, 4, 2);

      $new_date = $year . '-' . $month . '-' . $day . 'T';
      $new_date .= $hours . ':' . $minutes . ':' . $seconds;
      $new_date .= '+00:00';
      return array('iptc_new_date' => strtotime($new_date));
    }
  }
  return array('iptc_new_date' => '');
}
