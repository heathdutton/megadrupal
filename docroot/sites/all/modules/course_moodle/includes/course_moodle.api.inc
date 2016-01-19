<?php

/**
 * @file course_moodle.api.inc
 * Utility functions for dealing with Moodle courses, without equivalent API
 * functions.
 */

/**
 * Replicated Moodle function
 */
function course_moodle_get_coursemodule_from_id($modulename, $cmid, $courseid = 0) {
  global $_course_moodle_prefix;
  $courseselect = ($courseid) ? 'cm.course = ' . intval($courseid) . ' AND ' : '';

  return db_query("SELECT cm.*, m.name, md.name as modname
                           FROM {$_course_moodle_prefix}course_modules cm,
                                {$_course_moodle_prefix}modules md,
                                {mdl_" . $modulename . "} m
                           WHERE $courseselect
                                 cm.id = " . intval($cmid) . " AND
                                 cm.instance = m.id AND
                                 md.name = '{$modulename}' AND
                                 md.id = cm.module")->fetch();
}

/**
 * Replicated Moodle functions
 */
function course_moodle_moodle_get_record($table, $field1, $value1, $field2 = '', $value2 = '', $field3 = '', $value3 = '', $fields = '*') {
  //call the datalib and the dmllib
  // Check to see whether this record is eligible for caching (fields=*, only condition is id)

  $select = course_moodle_moodle_where_clause($field1, $value1, $field2, $value2, $field3, $value3);



  $record = db_query('SELECT ' . $fields . ' FROM {mdl_' . $table . '} ' . $select)->fetch();

  return $record;
}

/**
 * Replicated Moodle functions
 */
function course_moodle_moodle_where_clause($field1 = '', $value1 = '', $field2 = '', $value2 = '', $field3 = '', $value3 = '') {
  if ($field1) {
    $select = is_null($value1) ? "WHERE $field1 IS NULL" : "WHERE $field1 = '$value1'";
    if ($field2) {
      $select .= is_null($value2) ? " AND $field2 IS NULL" : " AND $field2 = '$value2'";
      if ($field3) {
        $select .= is_null($value3) ? " AND $field3 IS NULL" : " AND $field3 = '$value3'";
      }
    }
  }
  else {
    $select = '';
  }
  return $select;
}

/**
 * Save SCORM/AICC file to Moodle(TM) from Drupal's Course_moodle:
 * accepts: $node = (course node)
 *          $scorm_file = (array, with keys:
 *                                        "filename" => (name of SCORM zip file)
 *                                        and "path" => (path to the SCORM zip file in Drupal)
 * @todo refactor for new directory structure (files/moodle/data)
 */
function course_moodle_save_scorm_file($node, $scorm_file) {
  global $base_path;
  $cid = $node->course['cid'];

  //path
  $dir_moodle_data = $base_path . 'files/moodle/data';
  $course_data_dir = $_SERVER['DOCUMENT_ROOT'] . $dir_moodle_data . "/$cid";
  $course_scorm_file_dir = $course_data_dir . "/course_moodle_scorm";

  //make the course data dir if it doesn't exist
  if (!is_dir($course_data_dir)) {
    mkdir($course_data_dir, 0755);
  }

  //make the scorm data dir if it doesn't exist
  if (is_dir($course_data_dir) && !is_dir($course_scorm_file_dir)) {
    mkdir($course_scorm_file_dir, 0755);
  }
  //course scorm data dir exists, so copy the file to moodle
  if (is_dir($course_scorm_file_dir)) {
    //save the scorm file from $path_scorm_file_source to the dir $course_scorm_file_dir
    $path_to_scorm_file = $course_scorm_file_dir . '/' . $scorm_file['filename'];
    if (!file_exists($path_to_scorm_file)) {
      $success = copy($scorm_file['path'], $path_to_scorm_file);
      watchdog('course_moodle', 'scorm file successfully saved');
      return $success;
    }
    else {
      $success = copy($scorm_file['path'], $path_to_scorm_file);
      watchdog('course_moodle', 'scorm file successfully updated');
      return $success;
    }
  }
  watchdog('course_moodle', 'scorm file not saved');
  return FALSE;
}

/**
 * What is this?
 */
function courses_moodle_scorm_undefined($mixed = NULL) {
  global $user;
  global $_course_moodle_prefix;
  $referer_url = $_SERVER["HTTP_REFERER"];
  $referer_url_array = explode('&', $referer_url);
  foreach ($referer_url_array as $refer_var_pair) {
    $refer_var_pair_array = explode('=', $refer_var_pair);
    if (stristr($refer_var_pair_array[0], 'scoid')) {
      $scoid = $refer_var_pair_array[1];
    }
  }

  //$referer_url
  if ($scoid) {
    $sql = "SELECT mss.id AS scoid, mss.manifest, ms.id AS scormid, ms.course AS cid , ms.name, ms.reference, cn.nid  FROM {$_course_moodle_prefix}scorm_scoes mss, {$_course_moodle_prefix}scorm ms, {course_node} cn WHERE mss.scorm = ms.id AND cn.cid = ms.course AND mss.id = :scoid";


    $result = db_query($sql, array('scoid' => $scoid));
    $record = $result->fetch();
    if ($record->nid) {
      //check for completion:
      //select * from drupal_mdl_scorm_scoes_track where element like '%lesson_status%' and scoid = 38 and scormid = 14 and userid = 71;
      $sql2 = "SELECT * FROM {$_course_moodle_prefix}scorm_scoes_track where element like :element and scoid = :scoid and scormid = :scormid and userid IN (SELECT id FROM {$_course_moodle_prefix}user WHERE idnumber = :uid)";
      $result = db_query($sql2, array(':element' => '%lesson_status%', ':scoid' => $record->scoid, ':scormid' => $record->scormid, ':uid' => $user->uid));
      while ($record_status = $result->fetch()) {
        if (strtolower($record_status->value) == 'passed' || strtolower($record_status->value) == 'completed' || strtolower($record_status->value) == 'complete') {
          $passed = 1;
          $sql3 = "SELECT * FROM {course_outline} where nid = :nid and instance in (SELECT id FROM {$_course_moodle_prefix}course_modules where module = (SELECT id FROM {$_course_moodle_prefix}modules where name = :scorm) and course = :cid and instance = :scormid)";
          $result3 = db_query($sql3, array(':nid' => $record->nid, ':scorm' => 'scorm', ':cid' => $record->cid, ':scormid' => $record->scormid));
          $req_record = $result3->fetch();
          $moodle_user = course_moodle_get_moodle_user($user->uid);
          course_moodle_activity_fulfillment_check($req_record->coid, $user->uid);
        }
      }
      drupal_goto("node/{$record->nid}/takecourse");
    }
  }
  elseif (is_numeric($mixed)) {
    drupal_goto("node/{$mixed}/takecourse");
  }
  else {
    $sql = "SELECT *, (SELECT nid FROM {course_node} where cid in (select course from {$_course_moodle_prefix}scorm WHERE id = scormid)) AS nid  FROM {$_course_moodle_prefix}scorm_scoes_track WHERE userid in (SELECT id FROM {$_course_moodle_prefix}user WHERE idnumber = :uid) ORDER BY timemodified DESC LIMIT 1";
    $result = db_query($sql, array(':uid' => $user->uid));
    $record = $result->fetch();

    if ($record->nid) {
      $sql2 = "SELECT * FROM {$_course_moodle_prefix}scorm_scoes_track where element like :element and scoid = :scoid and scormid = :scormid and userid IN (SELECT id FROM {$_course_moodle_prefix}user WHERE idnumber = :uid)";
      $result = db_query($sql2, array(':element' => '%lesson_status%', ':scoid' => $record->scoid, ':scormid' => $record->scormid, ':uid' => $user->uid));
      while ($record_status = $result->fetch()) {
        if (strtolower($record_status->value) == 'passed' || strtolower($record_status->value) == 'completed' || strtolower($record_status->value) == 'complete') {
          $passed = 1;
          $sql3 = "SELECT * FROM {course_outline} where nid = :nid and instance in (SELECT id FROM {$_course_moodle_prefix}course_modules where module = (SELECT id FROM {$_course_moodle_prefix}modules where name = :scorm) and course = :cid and instance = :scormid)";
          $result3 = db_query($sql3, array(':nid' => $record->nid, ':scorm' => 'scorm', array(':cid' => $record->cid, array(':scormid' => $record->scormid))));
          $req_record = $result3->fetch();
          $moodle_user = course_moodle_get_moodle_user($user->uid);
          course_moodle_activity_fulfillment_check($req_record->coid, $user->uid);
        }
      }
      drupal_goto("node/{$record->nid}/takecourse");
    }
  }
  echo "";
}

/**
 * Replicated Moodle functions
 */
function course_moodle_api_stripslashes_safe($mixed) {
  if (empty($mixed)) {
    //do nothing...
  }
  elseif (is_string($mixed)) {
    // only unescape single quotes
    if (course_moodle_api_ini_get_bool('magic_quotes_sybase')) {
      $mixed = str_replace("''", "'", $mixed);
      // the rest, simple and double quotes and backslashes
    }
    else {
      $mixed = str_replace("\\'", "'", $mixed);
      $mixed = str_replace('\\"', '"', $mixed);
      $mixed = str_replace('\\\\', '\\', $mixed);
    }
  }
  elseif (is_array($mixed)) {
    foreach ($mixed as $key => $value) {
      $mixed[$key] = course_moodle_api_stripslashes_safe($value);
    }
  }
  elseif (is_object($mixed)) {
    $vars = get_object_vars($mixed);
    foreach ($vars as $key => $value) {
      $mixed->$key = course_moodle_api_stripslashes_safe($value);
    }
  }

  return $mixed;
}

/**
 * Replicated Moodle functions
 */
function course_moodle_api_ini_get_bool($ini_get_arg) {
  $temp = ini_get($ini_get_arg);

  if ($temp == '1' or strtolower($temp) == 'on') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Replicated Moodle functions
 */
function course_moodle_api_get_records($table, $field = '', $value = '', $sort = '', $fields = '*', $limitfrom = '', $limitnum = '') {
  $rs = course_moodle_api_get_recordset($table, $field, $value, $sort, $fields, $limitfrom, $limitnum);
  return course_moodle_api_recordset_to_array($rs);
}

/**
 * Replicated Moodle functions
 */
function course_moodle_api_get_records_select($table, $select = '', $sort = '', $fields = '*', $limitfrom = '', $limitnum = '') {
  $rs = course_moodle_api_get_recordset_select($table, $select, $sort, $fields, $limitfrom, $limitnum);
  return course_moodle_api_recordset_to_array($rs);
}

/**
 * Replicated Moodle functions
 */
function course_moodle_api_get_recordset($table, $field = '', $value = '', $sort = '', $fields = '*', $limitfrom = '', $limitnum = '') {

  if ($field) {
    $select = "$field = '$value'";
  }
  else {
    $select = '';
  }

  return course_moodle_api_get_recordset_select($table, $select, $sort, $fields, $limitfrom, $limitnum);
}

/**
 * Replicated Moodle functions
 */
function course_moodle_api_get_recordset_select($table, $select = '', $sort = '', $fields = '*', $limitfrom = '', $limitnum = '') {
  if ($select) {
    $select = ' WHERE ' . $select;
  }
  if ($sort) {
    $sort = ' ORDER BY ' . $sort;
  }
  return course_moodle_api_get_recordset_sql('SELECT ' . $fields . ' FROM {mdl_' . $table . '} ' . $select . $sort, $limitfrom, $limitnum);
}

/**
 * Replicated Moodle functions
 */
function course_moodle_api_get_recordset_sql($sql, $limitfrom = NULL, $limitnum = NULL) {
  $res = db_query($sql);

  return $res;
}

/**
 * Replicated Moodle functions
 */
function course_moodle_api_get_field($table, $return, $field1, $value1, $field2 = '', $value2 = '', $field3 = '', $value3 = '') {
  $select = course_moodle_api_where_clause($field1, $value1, $field2, $value2, $field3, $value3);
  return get_field_sql('SELECT ' . $return . ' FROM {mdl_' . $table . '} ' . $select);
}

/**
 * Replicated Moodle functions
 */
function course_moodle_api_get_field_select($table, $return, $select) {
  global $_course_moodle_prefix;
  if ($select) {
    $select = 'WHERE ' . $select;
  }
  return course_moodle_api_get_field_sql('SELECT ' . $return . ' FROM ' . $_course_moodle_prefix . $table . ' ' . $select);
}

/**
 * Replicated Moodle functions
 */
function course_moodle_api_where_clause($field1 = '', $value1 = '', $field2 = '', $value2 = '', $field3 = '', $value3 = '') {
  if ($field1) {
    $select = is_null($value1) ? "WHERE $field1 IS NULL" : "WHERE $field1 = '$value1'";
    if ($field2) {
      $select .= is_null($value2) ? " AND $field2 IS NULL" : " AND $field2 = '$value2'";
      if ($field3) {
        $select .= is_null($value3) ? " AND $field3 IS NULL" : " AND $field3 = '$value3'";
      }
    }
  }
  else {
    $select = '';
  }
  return $select;
}

/**
 * Replicated Moodle functions
 */
function course_moodle_api_recordset_to_array($rs) {
  if ($rs && !rs_EOF($rs)) {
    $objects = array();
    $firstcolumn = $rs->FetchField(0);
    if ($records = $rs->GetAssoc(TRUE)) {
      foreach ($records as $key => $record) {
        $record[$firstcolumn->name] = $key;
        $objects[$key] = (object) $record;
      }
      return $objects;
    }
    elseif ($rs->_numOfFields == 1 && $records = $rs->GetRows()) {
      foreach ($records as $key => $record) {
        $objects[$record[$firstcolumn->name]] = (object) $record;
      }
      return $objects;
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}
