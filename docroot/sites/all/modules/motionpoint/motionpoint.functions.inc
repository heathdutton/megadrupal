<?php
/**
 * @file
 * Functions for module motionpoint.
 */

/**
 * MotionPoint API Request.
 */
function motionpoint_send_transltation_request($original_text, $prefix, $action_type, $original_language = 'en', $force = FALSE, $scedule = TRUE) {
  $url = 'http://' . $prefix . '.convertlanguage.com/?mpactionid=' . $action_type;
  $headers = array(
    'Content-Type' =>  'application/html; charset=UTF-8',
    'X-MPTrans-Language' => mb_strtoupper($original_language),
    'X-MPTrans-Translate' => $force,
    'X-MPTrans-Schedule' => $scedule,
  );

  $translated_text = drupal_http_request(
    $url,
    array(
      'method' => 'POST',
      'data' => $original_text,
      'headers' => $headers,
    )
  );

  if ($translated_text->code == '200' && $translated_text->status_message == 'OK') {
    $mp_status = $translated_text->headers['x-mptrans-status'];
    return array('status' => $mp_status, 'data' => $translated_text->data);
  }

  return FALSE;
}

/**
 * Translate job.
 */
function motionpoint_translate_job($job, $original = FALSE) {
  if (empty($original)) {
    $query = db_select('locales_source', 'lt');
    $query->addField('lt', 'source')->condition('lt.lid', $job['lid']);
    $locale_object = $query->execute()->fetchObject();
    if (empty($locale_object)) {
      return FALSE;
    }
    $original = $locale_object->source;
  }

  $settings = variable_get('motionpoint_settings_languages');
  $params = array(
    'original_text' => $original,
    'prefix' => $settings[$job['lang']],
    'action_type' => MOTIONPOINT_ACTION_RETURN_ORIGINAL_IF_TRANSLATION_IS_UNAVAILABLE,
    'original_language' => 'en',
    'force' => FALSE,
    'schedule' => TRUE,
  );
  drupal_alter('motionpoint_translate_job_request_pre_send', $params, $job);
  $result = motionpoint_send_transltation_request(
    $params['original_text'],
    $params['prefix'],
    $params['action_type'],
    $params['original_language'],
    $params['force'],
    $params['schedule']
  );
  drupal_alter('motionpoint_translate_job_request_after_send', $result);

  if ($result === FALSE) {
    return FALSE;
  }
  elseif (!empty($result)) {
    $translation_data = array('lid' => $job['lid'], 'translation' => $result['data']);
    drupal_write_record('locales_target', $translation_data, array('lid'));
    $job['status'] = $result['status'];
    motionpoint_update_job($job);
  }
}

/**
 * Add job to translate via MotionPoint.
 */
function motionpoint_add_job($lid, $lang = array()) {
  if (!$lid) {
    return FALSE;
  }
  if (empty($lang)) {
    $lang = array_keys(language_list());
  }
  elseif (!is_array($lang)) {
    $lang = array($lang);
  }
  foreach ($lang as $l) {
    if ($l == 'en') {
      continue;
    }
    if (!motionpoint_get_job($lid, $l)) {
      $data = array(
        'lid' => $lid,
        'lang' => $l,
        'status' => '',
        'translated_date' => 0,
      );
      drupal_write_record(MOTIONPOINT_JOBS_TABLE, $data);
    }
  }

  return TRUE;
}

/**
 * Get job to translate via Motionpoint by lid and lang.
 */
function motionpoint_get_job($lid, $lang) {
  if (!$lid || empty($lang)) {
    return FALSE;
  }
  $row = db_select(MOTIONPOINT_JOBS_TABLE, 'mtj')
    ->fields('mtj', array())
    ->condition('lid', $lid)
    ->condition('lang', $lang)
    ->execute()
    ->fetchAll(PDO::FETCH_ASSOC);

  if (!empty($row)) {
    return (is_array($lang) && count($lang) > 0) ? $row : $row[0];
  }

  return FALSE;
}

/**
 * Delete jobs.
 */
function motionpoint_delete_job($lid, $lang) {
  if (!$lid || empty($lang)) {
    return FALSE;
  }
  $query = db_delete(MOTIONPOINT_JOBS_TABLE);
  $query->condition('lid', $lid);
  $query->condition('lang', $lang);

  return $query->execute();
}

/**
 * Delete jobs.
 */
function motionpoint_update_job($data) {
  if (empty($data['lid']) || !empty($data['lang']) ) {
    return FALSE;
  }

  return drupal_write_record(MOTIONPOINT_JOBS_TABLE, $data, array('lid', 'lang'));
}
