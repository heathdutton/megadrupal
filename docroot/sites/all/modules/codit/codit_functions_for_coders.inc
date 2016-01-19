<?php
/**
 * @file
 * Functions for coders to use to make Drupal life easier with codit module.
 */

/**
 * Loads current node entity or term entity and returns the Entity object.
 *
 * @return object
 *   The entity object that was loaded.
 */
function codit_load_the_entity() {
  // Attempt to load the node.
  $o_entity = menu_get_object();
  // Try loading the term  if term is not available.
  $o_entity = (!empty($o_entity)) ? $o_entity : menu_get_object('taxonomy_term', 2);
  return $o_entity;
}


/**
 * Get the bundle of current page by node type, vocabulary machine name or NULL.
 *
 * @param object $o_entity
 *   The fully loaded entity.
 *
 * @return string
 *   The node bundle(type), the vocabulary machine name or NULL if neither.
 */
function codit_get_entity_bundle($o_entity) {
  // Check to see if it is a node or a term.
  if (!empty($o_entity->vocabulary_machine_name)) {
    // Means this is a vocabulary term page.
    $s_bundle = $o_entity->vocabulary_machine_name;
  }
  elseif (!empty($o_entity->type)) {
    // Means this is a node.
    $s_bundle = $o_entity->type;
  }
  else {
    $s_bundle = NULL;
  }

  return $s_bundle;
}


/**
 * Outputs a template tracer opening comment if user has permission to see it.
 *
 * @param string $s_file
 *   The file ' __FILE__' that is calling the tracer.
 * @param mixed $optional_message
 *   Anything you want included in the template tracer output.
 */
function codit_tto($s_file = 'codit_tto() called incorrectly.  Must be codit_tto(__FILE__)', $optional_message = '') {
  print _codit_tt($s_file, $optional_message, TRUE);
}


/**
 * Outputs a template tracer closing comment if user has permission to see it.
 *
 * @param string $s_file
 *   The file '__FILE__' that is calling the tracer.
 * @param mixed $optional_message
 *   Anything you want included in the template tracer output.
 */
function codit_ttc($s_file = 'codit_ttc() called incorrectly.  Must be codit_ttc(__FILE__)', $optional_message = '') {
  print _codit_tt($s_file, $optional_message, FALSE);
}


/**
 * The base function for Template Tracers. Should not be called directly.
 *
 * @param string $s_file
 *   The file '__FILE__' that is calling the tracer.
 * @param mixed $optional_message
 *   Anything to include in the template tracer output.
 * @param bool $b_open
 *   TRUE if the it is an opening tracer, FALSE if is is closing tracer.
 *
 * @return string
 *   HTML comments to output.
 */
function _codit_tt($s_file = '_codit_tt() called incorrectly.  Must call codit_tto(__FILE__) or codit_ttc(__FILE__)', $optional_message = '', $b_open = TRUE) {
  $s_tag = ($b_open) ? '<TEMPLATE>' : '</TEMPLATE>';
  if (user_access('view codit template tracers')) {
    if ((is_array($optional_message)) || (is_object($optional_message))) {
      // Means it is a array or opject, so convert it to a pretty string.
      $optional_message = print_r($optional_message, TRUE);
    }
    $optional_message = (!empty($optional_message)) ? "<!-- Message: $optional_message -->\n" : '';
    return ("<!-- $s_tag $s_file -->\n $optional_message");
  }
  return '';
}


/**
 * Checks permissions and presence of $_GET debug to output debug information.
 *
 * @param mixed $thing_to_output
 *   The array, object, number or string to output.
 * @param string $s_trigger
 *   The value in the $_GET to look for to output debug  ?debugslug=[s_trigger].
 * @param string $s_message
 *   An optional message that can accompany the output.
 *
 * @return bool
 *   Representing whether the user permission AND the trigger value
 *   was present in the GET
 */
function codit_debug($thing_to_output = array(), $s_trigger = '', $s_message = '') {
  // Check permissions to see if user is allowed to see debug.
  if (user_access('view codit debug output')) {
    // Check to see if kpr is available for nice ouput of arrays or objects.
    $f_output_it = (function_exists('kpr')) ? 'kpr' : 'print_r';
    // Get the site's debug slug.
    $s_debug_slug = variable_get('codit_debug_request_slug', 'debug');
    // Check the $_GET.
    if (!empty($_GET) && !empty($_GET[$s_debug_slug]) && (!empty($s_trigger)) && ($_GET[$s_debug_slug] == $s_trigger)) {
      if (!empty($s_message)) {
        $f_output_it("Message: $s_message");
      }
      if ($f_output_it === 'kpr') {
        $f_output_it('Debug Output: ');
        $f_output_it($thing_to_output);
      }
      else {
        print "<pre>\n";
        $f_output_it('Debug Output: ');
        $f_output_it($thing_to_output);
        print "</pre>\n";
      }
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Writes a memory report to the php error log and returns a memory data object.
 *
 * @param string $s_message
 *   Any message you want sent to the log.
 *
 * @param int $i_mb_threshold
 *   The threshhold in MB that if exceeded will write to the log (default:NULL).
 *   If unspecified, it will always write to log.
 *
 * @return array
 *   Containing the following elements
 *    ['memory_in_use'] - the memory used in bytes at the instant called.
 *    ['memory_in_use_friendly'] - The amount of memory in use in specfic
 *    appropriate quantities like mb or b, kb..
 *    ['memory_use_peak'] - The peak memory used in bytes by the script
 *    ['memory_use_peak_friendly'] - The peak memory used by the script in
 *    specfic appropriate quantities like mb or b, kb.
 *    ['threshold_exceeded'] - boolean TRUE if the threshold was exceeded,
 *    FALSE if not exceeded or NULL if not set.
 *    ['logged'] - boolean TRUE if an error message was logged.
 *    ['from_file'] - The file this funciton was called from.
 *    ['from_function'] - The function this funciton was called from.
 *     ['from_line'] -  The line this function was called from.
 */
function codit_memorytracker($s_message = '', $i_mb_threshold = NULL) {
  $a_return = array();
  $a_units = array('b', 'kb', 'mb', 'gb', 'tb', 'pb');
  // Convert threshold to bytes.
  $i_mb_threshold = (!empty($i_mb_threshold) && (is_numeric($i_mb_threshold))) ? $i_mb_threshold * 1049000 : NULL;
  $i_mem_in_use = memory_get_usage(TRUE);
  $s_friendly_mem_in_use = round($i_mem_in_use / pow(1024, ($i = floor(log($i_mem_in_use, 1024)))), 2) . ' ' . $a_units[$i];
  $i_mem_peak_use = memory_get_peak_usage(TRUE);
  $s_friendly_mem_peak_use = round($i_mem_peak_use / pow(1024, ($i = floor(log($i_mem_peak_use, 1024)))), 2) . ' ' . $a_units[$i];
  // Determine what file, function and line called for this check.
  $s_file = 'n/a';
  $s_function = 'n/a';
  $n_line = 'n/a';
  $debug_trace = debug_backtrace(FALSE);
  if (isset($debug_trace[1])) {
    $s_file = $debug_trace[1]['file'] ? $debug_trace[1]['file'] : 'n/a';
    $n_line = $debug_trace[1]['line'] ? $debug_trace[1]['line'] : 'n/a';
  }
  if (isset($debug_trace[2])) {
    $s_function = $debug_trace[2]['function'] ? $debug_trace[2]['function'] : 'n/a';
  }

  $s_message_to_log = "RAM CHECK| NOW: $s_friendly_mem_in_use | PEAK: $s_friendly_mem_peak_use  in $s_function line $n_line \n of $s_file \n ----Message: $s_message";
  // Check to see if memory use is above the threshold.
  if ($i_mem_in_use > $i_mb_threshold) {
    // Threshold exceeded, log it.
    error_log($s_message_to_log, 0);
    $a_return['threshold_exceeded'] = ($i_mb_threshold === NULL) ? NULL : TRUE;
    $a_return['logged'] = TRUE;
  }
  else {
    // Threshold not exceeded, log nothing.
    $a_return['threshold_exceeded'] = FALSE;
    $a_return['logged'] = FALSE;
  }

  $a_return['memory_in_use'] = $i_mem_in_use;
  $a_return['memory_in_use_friendly'] = $s_friendly_mem_in_use;
  $a_return['memory_use_peak'] = $i_mem_peak_use;
  $a_return['memory_use_peak_friendly'] = $s_friendly_mem_peak_use;
  $a_return['from_file'] = $s_file;
  $a_return['from_function'] = $s_function;
  $a_return['from_line'] = $n_line;

  return $a_return;
}


/**
 * Accepts bool or quasi-bool, returns sanitized value or other un-altered.
 *
 * @param string $quasi_bool
 *   Can be a variety of items (1, true, T, True, TRUE, 0, false, F, False,
 *   FALSE) that resemble true or false.
 *
 * @param string $s_type
 *   Indicates what type of item you want returned (string, boolean, or binary)
 *   string returns 'true'/'false' or passthrough unless $b_return_strict = true
 *   boolean returns true / false or passthrough unless $b_return_strict = true
 *   binary returns 1/0 or passthrough unless $b_return_strict = true
 *
 * @param bool $b_return_strict
 *   If true, any non-quasi boolean will return false. If set to false,
 *   non-quasi boolean will just be passed through and returned.
 *
 * @return string|boolean|binary
 *   Depending on the $s_type setting.
 */
function codit_sanitize_boolean($quasi_bool, $s_type = 'binary', $b_return_strict = FALSE) {
  if ($quasi_bool === FALSE) {
    $scrubbed_bool  = FALSE;
  }
  else {
    $scrubbed_bool = strtolower(trim($quasi_bool));
  }

  if (($scrubbed_bool === 'true') || ((int) $scrubbed_bool === 1) || ($scrubbed_bool === 't') || ($scrubbed_bool === TRUE)) {
    $sanitized = 1;
  }
  elseif (($scrubbed_bool !== '') && (($scrubbed_bool === 'false') || ($scrubbed_bool === '0') || ($scrubbed_bool === 0) || ($scrubbed_bool === 'f') || ($scrubbed_bool === FALSE))) {
    $sanitized = 0;
  }
  else {
    if ($b_return_strict) {
      $sanitized = 0;
    }
    else {
      $sanitized = $quasi_bool;
    }
  }

  switch ($s_type) {
    case 'string':
      if ($sanitized === 1) {
        $m_return = 'true';
      }
      elseif ($sanitized === 0) {
        $m_return = 'false';
      }
      else {
        $m_return = $quasi_bool;
      }
      break;

    case 'boolean':
    case 'bool':
      if ($sanitized === 1) {
        $m_return = TRUE;
      }
      elseif ($sanitized === 0) {
        $m_return = FALSE;
      }
      else {
        $m_return = $quasi_bool;
      }
      break;

    case 'binary':
    case 'integer':
    case 'int':
    case 'numeric':
    case 'number':
    default:
      $m_return = $sanitized;
      break;
  }

  return $m_return;
}


/**
 * When passed a 1 or 0 will return 'TRUE' or 'FALSE'.
 *
 * This also returns properly if passed 'true' or 'false' by mistake
 *
 * @param bool $binary_value
 *   1 or 0 but also accepts string 'true'
 *
 * @return string
 *   'TRUE' or 'FALSE'
 */
function codit_flip_boolean_to_string($binary_value) {
  if (($binary_value == 1) || ($binary_value === 'true') || ($binary_value === TRUE)) {
    return 'TRUE';
  }
  else {
    return 'FALSE';
  }
}
