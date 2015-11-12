<?php
/**
 * @file
 *  wildfire_broadcasts.api.inc
 *
 * API for the Wildfire Broadcasts module
 *
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * Code derived from Wildfire 1:
 * @author Chris Cohen <chris.cohen@tiger-fish.com>
 */

/**
 * Add a new broadcast.
 *
 * @param array $broadcast
 *    A full broadcast array.
 * @return bool
 *    FALSE if there was a validation problem with the broadcast. Otherwise,
 *    the ID of the broadcast that was just added.
 */
function wildfire_broadcasts_add($broadcast = array()) {

  $new_broadcast = new WildfireBroadcast();
  return $new_broadcast->newBroadcast(
    $broadcast['template'],
    $broadcast['name'],
    !empty($broadcast['subject']) ? $broadcast['subject'] : '',
    !empty($broadcast['from_name']) ? $broadcast['from_name'] : '',
    !empty($broadcast['from_email']) ? $broadcast['from_email'] : '',
    !empty($broadcast['reply_name']) ? $broadcast['reply_name'] : '',
    !empty($broadcast['reply_email']) ? $broadcast['reply_email'] : ''
  );

}

/**
 * Update an existing broadcast.
 *
 * @param array $broadcast
 *    A full broadcast array. Must contain a broadcast ID.
 *
 * @return bool
 *    FALSE if there was a validation problem with the broadcast.
 *    Returns SAVED_NEW or SAVED_UPDATED on successful update.
 */
function wildfire_broadcasts_update($broadcast = array()) {

  $update = new WildfireBroadcast($broadcast['mid']);
  foreach ($broadcast as $key => $value) {
    $update->{$key} = $value;
  }
  return $update->save();

}



/**
 * Check Link Lengths
 *
 * Runs through the string passed in $template and checks that all links that
 * are in href attributes are less than the $length specified. Default is 333
 * characters due to restrictions in Unique Indexes using UTF-8 in MySQL.
 *
 * @access public
 * @param string $content
 * @param int $length
 * @return boolean|array
 */
function wildfire_message_check_link_lengths($content, $length = 333) {
  if (!is_string($content) || !is_int($length)) {
    return FALSE;
  }
  $regex = '/href="([^"]+)"/i';
  $count = preg_match_all($regex, $content, $matches, PREG_SET_ORDER);
  if ($count === FALSE) {
    return FALSE;
  }

  $untrackable = array();

  foreach ($matches as $match) {
    $link = $match[1];

    if (drupal_strlen($link) > 333) {
      $untrackable[] = $link;
    }
  }
  return count($untrackable) > 0 ? $untrackable : TRUE;
}

/**
 * Gets the broadcast with the specified ID.
 *
 * @deprecated
 *  Use WildfireBroadcast->load() to obtain a loaded broadcast object
 *
 * @param int $mid
 *    The ID of the broadcast to get.
 * @return array
 *    A broadcast as an array, or FALSE if the broadcast was not found.
 *
 */
function wildfire_broadcast_get($mid = 0) {

  $broadcast = new WildfireBroadcast($mid);

  // FIXME: This is a workaround for the fact this still uses old style
  // rather than passing classes around.
  $broadcast_row = array(
    'mid' => $broadcast->mid,
    'name' => $broadcast->name,
    'created' => $broadcast->created,
    'lastsent' => $broadcast->lastsent,
    'subject' => $broadcast->subject,
    'template' => $broadcast->template,
    'from_name' => $broadcast->from_name,
    'from_email' => $broadcast->from_email,
    'reply_name' => $broadcast->reply_name,
    'reply_email' => $broadcast->reply_email,
  );

  /**
   * Return the broadcasts regions. The callees for this callback expect it to
   * be there.
   * TODO: This needs to be refined at some point.
   */
  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.api');
  $broadcast_row['regions'] = wildfire_template_get_regions($broadcast_row['template']);

  // 'Type' is also needed
  $broadcast_row['type'] = 'broadcast';

  return $broadcast_row;

}

/**
 * Gets all the broadcasts.
 *
 * @param int $limit
 *    The maximum number of broadcasts to get. Defaults to all broadcasts.
 * @return array
 *    An array of the broadcasts. Each broadcast is itself an array.
 */
function wildfire_broadcasts_get($limit = -1) {

  $wildfire_broadcasts =& drupal_static(__FUNCTION__);

  if (!isset($wildfire_broadcasts)) {

    module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.api');

    $wildfire_broadcasts = array();

    $query = db_select('wildfire_broadcasts', 'wb');
    $query->fields('wb');
    if (ctype_digit((string)$limit) && $limit > 0) {
      $query->range(0, $limit);
    }
    $query->orderBy('created', 'DESC');

    $result = $query->execute();

    while ($row = $result->fetchAssoc()) {

      // Return all of the row data as an array per broadcast
      $wildfire_broadcasts[$row['mid']] = $row;

      // Add in attached region definition data based upon the template defined
      // for this broadcast.
      $wildfire_broadcasts[$row['mid']]['regions']  = wildfire_template_get_regions($row['template']);
    }
  }

  return $wildfire_broadcasts;
}

/**
 * Given a broadcast, populate its regions with content.
 *
 * @deprecated
 *  This code expects to be passed a broadcast array, but should accept
 *  a WildfireMessage object as input instead.
 *
 * This would typically be done as the broadcast is being prepared to send.
 *
 * @param array $broadcast
 *  A populated broadcast array
 *  @see _wildfire_broadcasts_process_row()
 *
 * @return array
 *  A populated broadcast array containing region content.
 */
function wildfire_broadcast_populate($broadcast) {
  foreach ($broadcast['regions'] as $region_type => &$region) {
    // Remove the 's' from the end of the region type.
    $rtype = drupal_substr($region_type, 0, drupal_strlen($region_type) - 1);
    foreach ($region as $rname => &$inner_region) {
      $broadcast['regions'][$region_type][$rname]['content'] =
        wildfire_region_content($rname, $rtype, $broadcast['mid'], 'broadcast');
    }
  }

  return $broadcast;
}

/**
 * Check whether a given broadcast is valid.
 *
 * @param int $mid
 *    The broadcast ID to be checked.
 */
function wildfire_broadcast_valid($mid = 0) {

  try {
    $broadcast = new WildfireBroadcast($mid);
    return TRUE;
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    watchdog('wildfire', $e->getMessage(), array(), WATCHDOG_ERROR);
    return FALSE;
  }

}

/**
 * Check whether a broadcasts template is valid.
 *
 * @param int $mid
 *  Message ID to check.
 * @param bool $check_enabled
 *  $enabled whether to check if the template is enabled. Defaults to TRUE
 * @return bool
 *  TRUE if the template is valid and enabled, else it performs a redirect
 * to the message edit page to allow the user to rectify the issue.
 */
function wildfire_broadcast_valid_template($mid, $check_enabled = TRUE) {

  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.api');

  $message = new WildfireBroadcast($mid);

  $tpl = wildfire_template_get($message->template);

  if ($tpl === FALSE) {
    wildfire_error(
      'The selected template for this broadcast (!template) is invalid',
      array(
        '!template' => $message->template,
      ),
      'admin/wildfire/broadcasts/' . $mid . '/edit'
    );
    return FALSE;
  }

  if ($check_enabled) {
    if ($tpl['status'] !== '1') {
      $form['send_fieldset']['#disabled'] = TRUE;
      wildfire_error(
        'The selected template for this broadcast (!template) is disabled; please choose a different template for this broadcast.',
        array(
          '!template' => check_plain($tpl['title']),
        ),
        'admin/wildfire/broadcasts/' . $mid . '/edit'
      );
    }
  }

  return TRUE;

}
