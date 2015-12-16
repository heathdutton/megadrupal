<?php

/**
 * @file
 * Implements the agres_timeframe content type. This module was created by Amazeelabs and
 * given to me as a contribution. Thanks amazee labs!
 */

/**
 * Checks if a restriction is checked for a start date, end date, and a set of agres_timeframe nodes.
 *
 * @param array $data
 *   An array with parameters:
 *   - start_date: The start date (format: Y-m-d, or a format that can be used with strtotime)
 *   - end_date: The end date (format: Y-m-d, or a format that can be used with strtotime)
 *   - nids: An array with node ids to be taken into consideration.
 */
function agres_timeframe_api_check_restrictions($data) {
  //First, load the data associated with the restriction nodes. At the moment, the agres_timeframe node is loaded for each nid.
  //TO DO: maybe find a better solution in the future.
  //If no nids are provided, consider the global ones.

  if (!isset($data['nids'])) {
//    $result = db_query("SELECT n.nid FROM {node} n INNER JOIN {content_type_agres_timeframe} ctf ON ctf.nid=n.nid WHERE n.status = :n.status AND ctf.field_agres_timeframe_global_value = :ctf.field_agres_timeframe_global_value", array(':n.status' => 1, ':ctf.field_agres_timeframe_global_value' => 'yes'));
//    while ($nid = db_fetch_object($result)) {
//      $data['nids'][] = $nid->nid;
//    }
    $query = db_select('node', 'n');
    $query->fields('n', array('nid', 'title'));
    $query->condition('type', 'agres_timeframe');
    $query->innerJoin('field_data_field_agres_timeframe_global', 'fatg', 'n.nid = fatg.entity_id', array());
    $query->addField('fatg', 'field_agres_timeframe_global_value');
    $query->where('n.status = :status AND fatg.field_agres_timeframe_global_value = :global_value', array(':status' => 1, ':global_value' => 1));
//      $query->orderBy('field_agreservations_unitnumber_value', 'ASC');
    $res = $query->execute();


//     $query = db_select('node', 'n');
//     $query->fields('n', array('nid',));
//     $query->condition('type', 'agreservations_unit');
//     $query->innerJoin('field_data_field_agres_timeframe_global', 'fatg', 'n.nid = fatg.entity_id', array());
//     $query->addField('fatg', 'field_agres_timeframe_global_value');
//     $query->where('n.status = :status AND fatg.field_agres_timeframe_global_value = :global_value"' , array(':status' => 1,':global_value' => 1));
    $data['nids'] = array();
    $res = $query->execute();
    while ($nid = $res->fetchObject()) {

      $data['nids'][] = $nid->nid;
    }
  }

  if (isset($data['nids'])) {
    foreach ($data['nids'] as $nid) {

      if (isset($nid['nid'])) {
        $agres_timeframe = node_load($nid['nid']);
      }
      else {
        $agres_timeframe = node_load($nid);
      }

//        drupal_set_message('<pre>$query$query$query4444444**'.print_r($agres_timeframe,true).'</pre>');
      $checked = timefram_api_check_restriction(array('start_date' => $data['start_date'], 'end_date' => $data['end_date'], 'node' => $agres_timeframe));
      if (!$checked) {
//          drupal_set_message('<pre>falssch'.print_r($agres_timeframe,true).'</pre>');
        $message = agres_timeframe_api_get_restriction_message($agres_timeframe);
//         drupal_set_message('<pre>falssch'.print_r($message,true).'</pre>');
        return $message;
      }
    }
  }
  //If we reach this point, all the restrictions are checked.
  return TRUE;
}

function timefram_api_check_restriction($data) {
  if (!is_object($data['node'])) {
    $data['node'] = node_load($data['node']);
  }
  if (is_object($data['node']) && $data['node']->type == 'agres_timeframe' && $data['node']->status == 1) {
    //3 cases can be identified here:
    //1. The start date and end date are in the agres_timeframe interval: in this case check that the start date is in the specific day, there is a minimum number of days
//   until the end date, and the end date is in a specific day of a week.
    //2. Only the start date is in the interval: in this case, check only if the start date is in a specific day of the week.
    //3. Only the end date is in the interval: in this case, check only if the end date is in a specific day of the week.

    $start_timestamp = strtotime($data['start_date']);
    $start_day = format_date($start_timestamp, 'custom', 'w');
    if (!$start_day) {
      $start_day = 7;
    }
    $end_timestamp = strtotime($data['end_date']);
    $end_day = format_date($end_timestamp, 'custom', 'w');
    if (!$end_day) {
      $end_day = 7;
    }

    $tmpperiod1 = new DateObject($data['node']->field_agres_timeframe_period[$data['node']->language][0]['value']);
    $tmpperiod2 = new DateObject($data['node']->field_agres_timeframe_period[$data['node']->language][0]['value2']);
    $tmpagresdate1 = new DateObject($data['start_date']);
    $tmpagresdate2 = new DateObject($data['end_date']);
    $strperiod1 = date_format($tmpperiod1, 'Y-m-d');
    $strperiod2 = date_format($tmpperiod2, 'Y-m-d');
    $agresdate1 = date_format($tmpagresdate1, 'Y-m-d');
    $agresdate2 = date_format($tmpagresdate2, 'Y-m-d');

    //First, check 1.
    if ($agresdate1 >= $strperiod1 && $agresdate1 <= $strperiod2 &&
      $agresdate2 >= $strperiod1 && $agresdate2 <= $strperiod2) {
      if ((isset($data['node']->field_agres_timeframe_from_day[$data['node']->language][0]['value'])) && $start_day != $data['node']->field_agres_timeframe_from_day[$data['node']->language][0]['value']) {
//          drupal_set_message('<pre>$query$query$query41111111111neeuuu1*'.print_r($data['node']->field_agres_timeframe_from_day[$data['node']->language][0]['value'],true).'</pre>');
        return FALSE;
      }
      if ((isset($data['node']->field_agres_timeframe_to_day[$data['node']->language][0]['value'])) && $end_day != $data['node']->field_agres_timeframe_to_day[$data['node']->language][0]['value']) {
        return FALSE;
      }
      $min_end_timestamp = strtotime('+' . $data['node']->field_agres_timeframe_min_days[$data['node']->language][0]['value'] . ' days', $start_timestamp);
//       drupal_set_message('<pre>$min_end_timestamp  '.print_r('+'.$data['node']->field_agres_timeframe_min_days[$data['node']->language][0]['value'].' days',true).'</pre>');
      if ($end_timestamp < $min_end_timestamp) {
        return FALSE;
      }

      return TRUE;
    }
    //Check 2.
    if ($agresdate1 >= $strperiod1 && $agresdate1 <= $strperiod2) {
      if ($data['node']->field_agres_timeframe_from_day[$data['node']->language][0]['value'] && $start_day != $data['node']->field_agres_timeframe_from_day[$data['node']->language][0]['value']) {
        return FALSE;
      }
      return TRUE;
    }
    //Check 3.
    if ($agresdate2 >= $strperiod1 && $agresdate2 <= $strperiod2) {
      if (isset($data['node']->field_agres_timeframe_to_day[$data['node']->language][0]['value']) && $data['node']->field_agres_timeframe_to_day[$data['node']->language][0]['value'] && $end_day != $data['node']->field_agres_timeframe_to_day[$data['node']->language][0]['value']) {
        return FALSE;
      }
      return TRUE;
    }
  }
  //If we reach this point, that means that the node is not a valid agres_timeframe, or the start and end dates are both outside the interval, so just return TRUE.
  return TRUE;
}

/**
 * Same as agres_timeframe_api_check_restrictions, but only for a single node.
 *
 */
function timefram_api_check_restriction_old($data) {
  if (!is_object($data['node'])) {
    $data['node'] = node_load($data['node']);
  }
  if (is_object($data['node']) && $data['node']->type == 'agres_timeframe' && $data['node']->status == 1) {
    //3 cases can be identified here:
    //1. The start date and end date are in the agres_timeframe interval: in this case check that the start date is in the specific day, there is a minimum number of days
//   until the end date, and the end date is in a specific day of a week.
    //2. Only the start date is in the interval: in this case, check only if the start date is in a specific day of the week.
    //3. Only the end date is in the interval: in this case, check only if the end date is in a specific day of the week.

    $start_timestamp = strtotime($data['start_date']);
    $start_day = format_date($start_timestamp, 'custom', 'w');
    if (!$start_day) {
      $start_day = 7;
    }
    $end_timestamp = strtotime($data['end_date']);
    $end_day = format_date($end_timestamp, 'custom', 'w');
    if (!$end_day) {
      $end_day = 7;
    }
    //drupal_set_message('<pre>$query$query$query411111111111*' . print_r($data, true) . '</pre>');
    //First, check 1.
    if ($data['start_date'] >= $data['node']->field_agres_timeframe_period[$data['node']->language][0]['value'] && $data['start_date'] <= $data['node']->field_agres_timeframe_period[$data['node']->language][0]['value2'] &&
      $data['end_date'] >= $data['node']->field_agres_timeframe_period[$data['node']->language][0]['value'] && $data['end_date'] <= $data['node']->field_agres_timeframe_period[$data['node']->language][0]['value2']) {
      //To DO: handle different strtotime strings if needed.

      if ($data['node']->field_agres_timeframe_from_day[$data['node']->language][0]['value'] && $start_day != $data['node']->field_agres_timeframe_from_day[$data['node']->language][0]['value']) {
        return FALSE;
      }
      if ($data['node']->field_agres_timeframe_to_day[$data['node']->language][0]['value'] && $end_day != $data['node']->field_agres_timeframe_to_day[$data['node']->language][0]['value']) {
        return FALSE;
      }
      $min_end_timestamp = strtotime('+' . $data['node']->field_agres_timeframe_min_days[$data['node']->language][0]['value'] . ' days', $start_timestamp);
      if ($end_timestamp < $min_end_timestamp) {
        return FALSE;
      }
      return TRUE;
    }
    //Check 2.
    if ($data['start_date'] >= $data['node']->field_agres_timeframe_period[$data['node']->language][0]['value'] && $data['start_date'] <= $data['node']->field_agres_timeframe_period[$data['node']->language][0]['value2']) {
      if ($data['node']->field_agres_timeframe_from_day[$data['node']->language][0]['value'] && $start_day != $data['node']->field_agres_timeframe_from_day[$data['node']->language][0]['value']) {
        return FALSE;
      }
      return TRUE;
    }
    //Check 3.
    if ($data['end_date'] >= $data['node']->field_agres_timeframe_period[$data['node']->language][0]['value'] && $data['end_date'] <= $data['node']->field_agres_timeframe_period[$data['node']->language][0]['value2']) {
      if ($data['node']->field_agres_timeframe_to_day[$data['node']->language][0]['value'] && $end_day != $data['node']->field_agres_timeframe_to_day[$data['node']->language][0]['value']) {
        return FALSE;
      }
      return TRUE;
    }
  }
  //If we reach this point, that means that the node is not a valid agres_timeframe, or the start and end dates are both outside the interval, so just return TRUE.
  return TRUE;
}

/**
 * Returns a restriction message for a agres_timeframe node.
 *
 * @param object $timefram_node
 *   The agres_timeframe node object.
 */
function agres_timeframe_api_get_restriction_message($agres_timeframe_node) {
  $message = array();
  $days = array(
    1 => t('Monday'),
    2 => t('Tuesday'),
    3 => t('Wednesday'),
    4 => t('Thursday'),
    5 => t('Friday'),
    6 => t('Saturday'),
    7 => t('Sunday'),
  );
//  drupal_set_message('<pre>agres_timeframe_api_get_restriction_message**'.print_r($agres_timeframe_node,true).'</pre>');
  if ($agres_timeframe_node->field_agres_timeframe_from_day[$agres_timeframe_node->language][0]['value'] > 0 && $agres_timeframe_node->field_agres_timeframe_to_day[$agres_timeframe_node->language][0]['value'] > 0) {
    $message['message_original'] = 'The selected period is not compatible with a reservation restriction from %startdate to %enddate. During this period you can only make reservations from %startweekdate to %endweekdate with a minimum length of %length days. Please change your reservation to this time.';
    $message['args'] = array(
      '%startdate' => format_date(strtotime($agres_timeframe_node->field_agres_timeframe_period[$agres_timeframe_node->language][0]['value']), 'custom', 'd.m.Y'),
      '%enddate' => format_date(strtotime($agres_timeframe_node->field_agres_timeframe_period[$agres_timeframe_node->language][0]['value2']), 'custom', 'd.m.Y'),
      '%startweekdate' => $days[$agres_timeframe_node->field_agres_timeframe_from_day[$agres_timeframe_node->language][0]['value']],
      '%endweekdate' => $days[$agres_timeframe_node->field_agres_timeframe_to_day[$agres_timeframe_node->language][0]['value']],
      '%length' => $agres_timeframe_node->field_agres_timeframe_min_days[$agres_timeframe_node->language][0]['value'],
    );
    $message['message'] = t($message['message_original'], $message['args']);
  }
  elseif ($agres_timeframe_node->field_agres_timeframe_from_day[$agres_timeframe_node->language][0]['value'] > 0) {
    $message['message_original'] = 'The selected period is not compatible with a reservation restriction from %startdate to %enddate. During this period you can only make reservations from %startweekdate with a minimum length of %length days. Please change your reservation to this time.';
    $message['args'] = array(
      '%startdate' => format_date(strtotime($agres_timeframe_node->field_agres_timeframe_period[$agres_timeframe_node->language][0]['value']), 'custom', 'd.m.Y'),
      '%enddate' => format_date(strtotime($agres_timeframe_node->field_agres_timeframe_period[$agres_timeframe_node->language][0]['value2']), 'custom', 'd.m.Y'),
      '%startweekdate' => $days[$agres_timeframe_node->field_agres_timeframe_from_day[$agres_timeframe_node->language][0]['value']],
      '%length' => $agres_timeframe_node->field_agres_timeframe_min_days[$agres_timeframe_node->language][0]['value'],
    );
    $message['message'] = t($message['message_original'], $message['args']);
  }
  elseif ($agres_timeframe_node->field_agres_timeframe_to_day[$agres_timeframe_node->language][0]['value'] > 0) {
    $message['message_original'] = 'The selected period is not compatible with a reservation restriction from %startdate to %enddate. During this period you can only make reservations to %endweekdate with a minimum length of %length days. Please change your reservation to this time.';
    $message['args'] = array(
      '%startdate' => format_date(strtotime($agres_timeframe_node->field_agres_timeframe_period[$agres_timeframe_node->language][0]['value']), 'custom', 'd.m.Y'),
      '%enddate' => format_date(strtotime($agres_timeframe_node->field_agres_timeframe_period[$agres_timeframe_node->language][0]['value2']), 'custom', 'd.m.Y'),
      '%endweekdate' => $days[$agres_timeframe_node->field_agres_timeframe_to_day[$agres_timeframe_node->language][0]['value']],
      '%length' => $agres_timeframe_node->field_agres_timeframe_min_days[$agres_timeframe_node->language][0]['value'],
    );
    $message['message'] = t($message['message_original'], $message['args']);
  }
  else {
    $message['message_original'] = 'The selected period is not compatible with a reservation restriction from %startdate to %enddate. During this period you can only make reservations with a minimum length of %length days. Please change your reservation to this time.';
    $message['args'] = array(
      '%startdate' => format_date(strtotime($agres_timeframe_node->field_agres_timeframe_period[$agres_timeframe_node->language][0]['value']), 'custom', 'd.m.Y'),
      '%enddate' => format_date(strtotime($agres_timeframe_node->field_agres_timeframe_period[$agres_timeframe_node->language][0]['value2']), 'custom', 'd.m.Y'),
      '%length' => $agres_timeframe_node->field_agres_timeframe_min_days[$agres_timeframe_node->language][0]['value'],
    );
    $message['message'] = t($message['message_original'], $message['args']);
  }
  return $message;
}