<?php
/**
 * @file
 * Internal function to handle the comunication with OSM.
 */

/**
 * Authorise the user.
 */
function osm_authorise($email, $password) {
  $parts = array();
  $json = array();

  $parts['email']    = $email;
  $parts['password'] = $password;

  $json = osm_perform_query('users.php?action=authorise', $parts);

  if (!empty($json)) {
    variable_set('osm_userid', $json['userid']);
    variable_set('osm_secret', $json['secret']);
    return TRUE;
  }
  else {
    // TODO: Better description.
    drupal_set_message(t('Some error accour.'));
    return FALSE;
  }
}

/**
 * Performance call to osm page.
 */
function osm_perform_query($url = '', $parts = array()) {

  $base_url = variable_get('osm_base_url', 'https://www.onlinescoutmanager.co.uk/');

  $data = '';

  // Token API.
  $data .= 'token=' . variable_get('osm_token', '');

  // Id API.
  $data .= '&apiid=' . variable_get('osm_apiid', '');

  // User ID token.
  $data .= variable_get('osm_userid', '') ? '&userid=' . variable_get('osm_userid', '') : '';

  // Secret key API.
  $data .= variable_get('osm_secret', '') ? '&secret=' . variable_get('osm_secret', '') : '';

  if (!empty($parts)) {
    foreach ($parts as $key => $val) {
      $data .= '&' . $key . '=' . urlencode($val);
    }
  }

  $curl_handle = curl_init();
  curl_setopt($curl_handle, CURLOPT_URL, $base_url . $url);
  curl_setopt($curl_handle, CURLOPT_POSTFIELDS, $data);
  curl_setopt($curl_handle, CURLOPT_POST, 1);
  curl_setopt($curl_handle, CURLOPT_CONNECTTIMEOUT, 2);
  curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, 1);

  $msg = curl_exec($curl_handle);

  return drupal_json_decode($msg);
}

/**
 * Get the current roles and keep it.
 */
function osm_get_users_roles() {
  $osm_roles = osm_perform_query('api.php?action=getUserRoles');
  $all_roles = array();

  $roles = array('beavers', 'cubs', 'scouts', 'explorers');

  if (!is_array($osm_roles)) {
    return FALSE;
  }

  foreach ($osm_roles as $osm_role) {
    if (in_array($osm_role['section'], $roles)) {
      $all_roles[$osm_role['sectionid']] = array(
        'groupname'   => $osm_role['groupname'],
        'sectionname' => $osm_role['sectionname'],
        'section'     => $osm_role['section'],
        'sectionid'   => $osm_role['sectionid'],
      );
    }
  }

  variable_set('osm_all_roles', $all_roles);
  return $all_roles;

}

/**
 * Block settings for coming up by each role.
 */
function osm_coming_up_block_setting($delta, $sectionid) {
  $form = array();

  $num_entries = array(1, 2, 3, 4, 5);

  $form[$delta . '_num_entries'] = array(
    '#type'          => 'select',
    '#title'         => t('Number of entries to show'),
    '#default_value' => variable_get('oms_' . $delta . '_num_entries', array()),
    '#options'       => $num_entries,
  );

  $display = array(t('Programme and Events'), t('Programme'), t('Events'));

  $form[$delta . '_display'] = array(
    '#type'          => 'select',
    '#title'         => t('Display'),
    '#default_value' => variable_get('oms_' . $delta . '_display', 0),
    '#options'       => $display,
  );


  $format_types = system_get_date_types();
  foreach ($format_types as $type => $type_info) {
    $choices[$type] = $type_info['title'];
  }

  $form[$delta . '_date_format'] = array(
    '#type'          => 'select',
    '#title'         => t('Date Format'),
    '#default_value' => variable_get('oms_' . $delta . '_date_format', 'date_format_long'),
    '#options'       => $choices,
  );

  $form[$delta . '_sectionid'] = array(
    '#type'  => 'hidden',
    '#value' => $sectionid,
  );

  return $form;
}

/**
 * Block settings for patrol points by each role.
 */
function osm_patrol_points_block_setting($delta, $sectionid) {
  $form = array();

  $form[$delta . '_sectionid'] = array(
    '#type'  => 'hidden',
    '#value' => $sectionid,
  );

  return $form;
}

/**
 * Autocomplete helper.
 */
function osm_autocomplete($string) {

  $osm_active_roles = variable_get('osm_active_roles', '');
  $matches = array();

  foreach ($osm_active_roles as $role) {
    $sectionid = check_plain($role['section'] . '_' . $role['sectionid'] . '_coming_up');

    if (preg_match('@' . $string . '@', $sectionid)) {
      $matches[$sectionid] = $sectionid;
    }
  }

  drupal_json_output($matches);
}

/**
 * Own cmp function helper.
 */
function osm_cmp($a, $b) {
  if ($a['points'] == $b['points']) {
      return 0;
  }
  return ($a['points'] > $b['points']) ? -1 : 1;
}

/**
 * Gets the list of terms
 */
function _osm_get_terms() {
  $terms = &drupal_static(__FUNCTION__, array());

  if (!empty($terms->data)) {
    return $terms->data;
  }

  $terms = cache_get('osm:store_terms', 'cache_osm');

  if (isset($terms->data)) {
    return $terms->data;
  }

  $uri = 'api.php?action=getTerms';

  $terms = osm_perform_query($uri);

  cache_set('osm:store_terms', $terms, 'cache_osm');
  return $terms;
}

/**
 * Trown the term by termid.
 *
 * @param array $term
 *   Terms array to explore.
 * @param int $termid
 *   Termid by GET proces.
 *
 * @return array
 *   The term array filter by termid.
 */
function _osm_term_by_termid($terms, $termid = 0) {

  if (is_array($terms)) {
    foreach ($terms as $term) {
      if (!empty($termid) && $term['termid'] === $termid) {
        // Catchya!
        return $term;
      }
      else {
        // This mean we have to take a look to the dates.
        $start_Date = strtotime($term['startdate']);
        $end_date = strtotime($term['enddate']);
        if ($start_Date  <= time() && time() <= $end_date) {
          // Catchya!
          return $term;
        }
      }
    }
  }

  // If the terms array is empty or wrong.
  return;
}

/**
 * Get all terms list.
 *
 * @return array
 *   With al terms and events and programmes.
 */
function osm_get_terms() {

  $termid = '';
  if (!empty($_GET['termid'])) {
    $termid = check_plain($_GET['termid']);
  }

  $terms = _osm_get_terms();

  if (empty($terms)) {
    watchdog('osm', 'OSM: Fail to retrive terms.', array(), WATCHDOG_ERROR);
    return;
  }

  $all_terms = array();
  foreach ($terms as $ts){
    foreach($ts as $term) {
      $all_terms[$term['termid']] = array(
        'termid'         => $term['termid'],
        'sectionid'      => $term['sectionid'],
        'name'           => $term['name'],
        'startdate'      => $term['startdate'],
        'enddate'        => $term['enddate'],
        'termidselected' => $termid,
      );
    }
  }

  return $all_terms;
}

/**
 * Get a single unit of term.
 *
 * @param integer $sectionid
 *   ID section.
 *
 * @return array
 *   With the term.
 */
function osm_get_term($sectionid = 0) {
  $terms = _osm_get_terms();


  $termid = '';
  if (!empty($_GET['termid'])) {
    $termid = check_plain($_GET['termid']);
  }

  if (empty($terms)) {
    watchdog('osm', 'OSM: Fail to retrive term section: @sectionid.', array('@sectionid' => check_plain($sectionid)), WATCHDOG_ERROR);
    return;
  }

  if ($sectionid !== 0 && !empty($terms[$sectionid])) {
    $term = _osm_term_by_termid($terms[$sectionid], $termid);

    return $term;
  }
  else {
    watchdog('osm', 'OSM: Term section unavailable @sectionid', array('@sectionid' => check_plain($sectionid)), WATCHDOG_ERROR);
  }

  return;
}

/**
 * Return patrol points
 * TODO: Need docu.
 */
function osm_get_patrol_points($sectionid = 0) {

  if ($sectionid === 0) {
    return FALSE;
  }

  $patrols = osm_perform_query('users.php?action=getPatrols&sectionid=' . $sectionid);

  return $patrols;
}

/**
 * Returns the store programme and easy way to handle it.
 *
 * @param int $sectionid
 *   Section int.
 * @param int $termid
 *   Termn id.
 * @param int $display
 *   The kind of section to show if 2 event, 1 programme of 0 both.
 *
 * @return array
 *   Return into an array the store programme.
 */
function _osm_store_programme($sectionid = 0, $termid = 0, $display = 0, $options = array()) {

  // We have to process all and store it in cache.
  $store_programme = array();

  $sectionnames = array();
  $osm_active_roles = variable_get('osm_active_roles', '');

  if (!is_array($osm_active_roles)) {
    return array();
  }

  $ordered_sectionnames = array();
  foreach ($osm_active_roles as $section_name) {
    $ordered_sectionnames[] = array (
      'sectionid' => $section_name['sectionid'],
      'section' => $section_name['section']
    );
  }

  foreach ($ordered_sectionnames as $sn) {
    if ($sn['sectionid'] == $sectionid) {
      $section = $sn['section'];
    }
  }

  $now = date("Ymd");

  $date_format_type = 'date_format_long';
  $date_format = variable_get($date_format_type, 'd-m-Y');

  if ($display == 0 || $display == 1) {
    // Retrive cache data.
    $programme = cache_get('osm:store_programme:' . $sectionid . ($termid ? ':' . $termid : ''), 'cache_osm');

    if (isset($programme->data)) {
      $programme = $programme->data;
    }
    else {
      $programme = osm_perform_query('programme.php?action=getProgramme&sectionid=' . $sectionid . '&termid=' . $termid);
      cache_set('osm:store_programme:' . $sectionid . ($termid ? ':' . $termid : ''), $programme, 'cache_osm');
    }

    if (!empty($programme['items'])) {
      foreach ($programme['items'] as $meeting) {
        $meeting_date = new DateTime($meeting['meetingdate']);

        $store_programme[$meeting_date->format('Ymd')][] = array(
          'type'     => 'programme',
          'date'     => $meeting_date->format($date_format),
          'title'    => $meeting['title'],
          'summary'  => $meeting['notesforparents'],
          'location' => $meeting['prenotes'],
          'section'  => $section,
        );
      }
    }
  }

  if ($display == 0 || $display == 2) {
    // Retrive cache data.
    $event = cache_get('osm:store_event:' . $sectionid, 'cache_osm');

    if (isset($event->data)) {
      $event = $event->data;
    }
    else {
      $event = osm_perform_query('events.php?action=getEvents&sectionid=' . $sectionid);
      cache_set('osm:store_event:' . $sectionid, $event, 'cache_osm');
    }

    if (!empty($event['items'])) {
      $event['items'] = array_reverse($event['items']);

      foreach ($event['items'] as $meeting) {

        $termsarray = osm_get_terms();
        $startdateofterm = $termsarray[$termid]['startdate'];
        $enddateofterm = $termsarray[$termid]['enddate'];

        if (($meeting['startdate'] >= $startdateofterm) && ($meeting['startdate'] <= $enddateofterm)) {

          $meeting_date = new DateTime($meeting['startdate']);

          // In OSM when an end date is not entered the end date defaults to
          // this date.
          if ($meeting['enddate'] == "1970-01-01") {
            $end_date = new DateTime($meeting['startdate']);
          }
          else {
            $end_date = new DateTime($meeting['enddate']);
          }

          $store_programme[$meeting_date->format('Ymd')][] = array(
            'type'     => 'events',
            'date'     => $meeting_date->format($date_format) . '<br /> to ' . $end_date->format($date_format),
            'title'    => $meeting['name'],
            'summary'  => $meeting['notes'],
            'location' => $meeting['location'],
            'section'  => $section,
          );
        }
      }
    }
  }

  return $store_programme;
}

/**
 * Store block programme shows the next event/programme item.
 *
 * @param integer $sectionid
 *   Section ID.
 * @param integer $termid
 *   Which term we want.
 * @param integer $display
 *   Kind of display (actually this only work for full node views.)
 * @param array $options
 *   TODO: Docu.
 * @return array
 *   Full array with the programmes.
 */
function _osm_store_block_programme($sectionid = 0, $termid = 0, $display = 0, $options = array()) {

  $store_programme = cache_get('osm:store_block_programme:' . $sectionid, 'cache_osm');

  if (isset($store_programme->data)) {
    // return $store_programme->data;
  }

  // We have to restore the cache.
  $store_programme = array();

  $now = date("Ymd");

  $date_format_type = variable_get($options['date_format'], 'date_format_long');
  $date_format = variable_get('date_format_' . $options['date_format'], 'd-m-Y');

  // TODO: Ask Sam, Why we do this? We don't use this programme definition.
  // Is the current programme empty.
  // <S> The idea was that towards the end of a term, the last meeting will be
  // held. After this date, there will be no more meetings in this term so it
  // should look in the next term for the next meeting to display that one.
  // If that makes sense?
  // TODO: Refactore. @see comments below.
  /*
  if (empty($programme)) {
    // Gets the id of the next term.
    $terms = osm_get_terms();

    if (!empty($terms)) {
      $ordered_terms = array();
      $terms_stored = array();
      foreach ($terms as $terms_event) {
        if ($sectionid == $terms_event['sectionid'] && strtotime($terms_event['startdate']) >= strtotime('now')) {
          $ordered_terms[] = array(
            'sectionid'      => $terms_event['sectionid'],
            'name'           => $terms_event['name'],
            'termid'         => $terms_event['termid'],
            'termidselected' => $terms_event['termidselected'],
            'startdate'      => $terms_event['startdate'],
            'enddate'        => $terms_event['enddate'],
          );
        }
      }
    }
    $nextterm = $ordered_terms[0]['termid'];

    // If the programme is empty then look in the next term.
    $programme = osm_perform_query('programme.php?action=getProgramme&sectionid=' . $sectionid . '&termid=' . $nextterm);
  }
  // */

  if ($display == 0 || $display == 1) {
    // We store the programme because I know it'll be used on pages.
    $programme = cache_get('osm:store_programme:' . $sectionid . ($termid ? ':' . $termid : ''), 'cache_osm');

    if (isset($programme->data)) {
      $programme = $programme->data;
    }
    else {
      $programme = osm_perform_query('programme.php?action=getProgramme&sectionid=' . $sectionid . '&termid=' . $termid);
      cache_set('osm:store_programme:' . $sectionid . ($termid ? ':' . $termid : ''), 'cache_osm');
    }

    if (!empty($programme['items'])) {
      foreach ($programme['items'] as $meeting) {
        $meeting_date = new DateTime($meeting['meetingdate']);

        if ($meeting_date->format('Ymd') >= $now) {
          $store_programme[$meeting_date->format('Ymd')][] = array(
            'type'     => 'programme',
            'date'     => $meeting_date->format($date_format),
            'title'    => $meeting['title'],
            'summary'  => $meeting['notesforparents'],
            'location' => $meeting['prenotes'],
            'section'  => $sectionid,
          );
        }
      }
    }

    // If the meeting has past in the term and there are no more meetings in that
    // term - look for the first meeting in the next term
    if (empty($store_programme)) {
      $terms = osm_get_term($sectionid, $termid);

      if (!empty($terms)) {
        $ordered_terms = array();
        $terms_stored = array();

        foreach ($terms as $terms_event) {
          if ($sectionid == $terms_event['sectionid'] && strtotime($terms_event['startdate']) >= strtotime('now')){
            $ordered_terms[] = array('sectionid' => $terms_event['sectionid'], 'name' => $terms_event['name'], 'termid' => $terms_event['termid'], 'termidselected' => $terms_event['termidselected'], 'startdate' => $terms_event['startdate'], 'enddate' => $terms_event['enddate']);
          }
        }

        $nextterm = $ordered_terms[0]['termid'];

        $programme2 = cache_get('osm:store_programme:' . $sectionid . ($termid ? ':' . $termid : ''), 'cache_osm');

        if (isset($programme2->data)) {
          $programme2 = $programme2->data;
        }
        else {
          $programme2 = osm_perform_query('programme.php?action=getProgramme&sectionid=' . $sectionid . '&termid=' . $nextterm);
          cache_set('osm:store_programme:' . $sectionid . ($termid ? ':' . $termid : ''), 'cache_osm');
        }

        foreach ($programme2['items'] as $meeting) {
          $meeting_date = new DateTime($meeting['meetingdate']);

          // added to check it's within 7 days
          if ($meeting_date->format('Ymd') >= $now && $meeting_date->format('Ymd') <= $now+7) {
            $store_programme[$meeting_date->format('Ymd')][] = array(
              'type'     => 'programme',
              'date'     => $meeting_date->format($date_format),
              'title'    => $meeting['title'],
              'summary'  => $meeting['notesforparents'],
              'location' => $meeting['prenotes'],
              'section'  => $sectionid,
            );
          }
        }
      }
    }
  }

  if ($display == 0 || $display == 2) {
    $event = cache_get('osm:store_event:' . $sectionid, 'cache_osm');

    if (isset($event->data)) {
      $event = $event->data;
    }
    else {
      $event = osm_perform_query('events.php?action=getEvents&sectionid=' . $sectionid . '&futureonly=true');
      cache_set('osm:store_event:' . $sectionid, 'cache_osm');
    }

    if (!empty($event['items'])) {
      $event['items'] = array_reverse($event['items']);

      foreach ($event['items'] as $meeting) {
        $meeting_date = new DateTime($meeting['startdate']);

        // TODO: Figure out why this err date. <S> same as above
        if ($meeting['enddate'] == "1970-01-01") {
          $end_date = new DateTime($meeting['startdate']);
        }
        else {
          $end_date = new DateTime($meeting['enddate']);
        }

        // Added to check it's within 7 days.
        if ($end_date->format('Ymd') >= $now && $end_date->format('Ymd') <= $now + 7) {
          $store_programme[$meeting_date->format('Ymd')][] = array(
            'type'     => 'events',
            'date'     => $meeting_date->format($date_format) . '<br /> to ' . $end_date->format($date_format),
            'title'    => $meeting['name'],
            'summary'  => $meeting['notes'],
            'location' => $meeting['location'],
            'section'  => $sectionid,
          );
        }
      }
    }
  }

  cache_set('osm:store_block_programme:' . $sectionid, $store_programme, 'cache_osm');

  return $store_programme;
}
