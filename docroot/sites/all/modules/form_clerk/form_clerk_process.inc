<?php

/**
 * @file
 * Process form_clerk input files.
 *
 */

/**
 * Input file processing driver.
 *
 * @param $form_state
 *  The multiform state argument.
 */
function form_clerk_process_input(&$form_state) {
  $form_state['storage']['report'] .= 'input file(s): ' .
    $form_state['storage']['input']['input_file'] . "\n";
  // Ensure we have at least one input file.
  if ($form_state['storage']['input']['input_file'] == '') {
      drupal_set_message(t('You must provide one or more input files.'), 'error');
      return;
    }
  $input_files = explode(',', $form_state['storage']['input']['input_file']);

  if (count($input_files) == 0) {
    drupal_set_message(t('You must provide one or more input files'), 'error');
    return;
  }

  foreach ($input_files as $input_file) {

    // Input files are relative to 'input_dir'.
    $input_file = $form_state['storage']['input_dir'] . trim($input_file);
    if ($input_file == '') {
      continue;
    }
    $form_state['storage']['report'] .= "Input: $input_file\n";
    if (! file_exists($input_file)) {
      drupal_set_message(t('File not found: @input_file',
                           array('@input_file' => check_plain($input_file))), 'error');
      return;
    }
    $input = fopen($input_file, "rb");
    if (! $input) {
      drupal_set_message(t('Unable to open file: @input_file',
                           array('!input_file' => check_plain($input_file))), 'error');
      return;
    }

    $line_number = 0;
    // Process lines one at a time.
    while (($line = fgets($input)) !== FALSE) {
      $line_number++;
      // Skip blank lines.
      if (trim($line) == '') {
        continue;
      }
      // Skip comments.
      if (substr($line, 0, 1) == $form_state['storage']['comment_flag']) {
        continue;
      }
      // Handle 'info_only' commands.
      elseif (preg_match('/^FC_info_only/', $line)) {
        $form_state['storage']['info_only'] = 'info_only';
        form_clerk_process_data('', $line_number, $input_file, $form_state);
      }
      // Handle configuration commands.
      elseif (substr($line, 0, 3) == 'FC_') {
        form_clerk_process_config($line, $line_number, $input_file, $form_state);
      }
      else {
        // Must be form input, process it.
        form_clerk_process_data($line, $line_number, $input_file, $form_state);
      }
      // Stop processing if an error has been reported.
      $messages = drupal_get_messages('error', FALSE);
      if (count($messages) > 0) {
        $errors = $messages['error'];
        if ((count($errors) > 0) && $form_state['storage']['stop_on_error'] === TRUE) {
          // We want to ignore DOM error that is really a warning.
          foreach ($errors as $error) {
            if (!preg_match('/.*Warning.*/', $error)) {
              $form_state['storage']['report'] .=
                "Stopped on error. Line $line_number in input file $input_file\n";
              fclose($input);
              return;
            }
          }
        }
      }

      // Stop processing on 'exit' command.
      if ($form_state['storage']['exit'] == TRUE) {
        $form_state['storage']['report'] .=
                "Stopping on 'exit' command. Line $line_number in input file $input_file\n";
        fclose($input);
        return;
      }

      // Stop processing after info_only.
      if ($form_state['storage']['info_only'] == TRUE) {
        $form_state['storage']['report'] .=
                "Stopping on 'info_only' command. Line $line_number in input file $input_file\n";
        fclose($input);
        return;
      }
    }
    fclose($input);
  }
  return;
}


/**
 * form_clerk input file data line processor.
 *
 * @param $line
 *  String containing the current line.
 * @$line_number
 *  The line number of the line within the current file.
 * @param $input_file
 *  Human readable name of the file.
 * @param $form_state
 *  The multiform state argument.
 */
function form_clerk_process_data($line, $line_number, $input_file, &$form_state) {
  if (!form_clerk_process_ready($line, $line_number, $input_file, $form_state)) {
    return;
  }
  // Break the line into individual values.
  $delimiter = $form_state['storage']['delimiter'];
  $values = explode($delimiter, $line);
  $user_fields = array();
  $names = $form_state['storage']['names'];
  if (count($values) != count($names)) {
    drupal_set_message(t('Number of values does not match number of names: Line #@line in file: @file',
      array('@line' => check_plain($line_number), '@file' => check_plain($input_file))), 'error');
    return FALSE;
  }
  for ($i = 0; $i < count($values); $i++) {
    $name  = trim($names[$i]);
    $value = trim($values[$i]);
    if ($form_state['storage']['url_decode']) {
      $value = urldecode($value);
    }

    // Use the password given at startup, so passwords aren't stored in input files..
    if (($value == $form_state['storage']['password_indicator']) && ($name == 'pass')) {
      $value = $form_state['storage']['input']['password'];
    }

    $user_fields[$name] = $value;
  }
  $form_state['storage']['user_fields'] = $user_fields;

  // Input the form data.
  $form_state['storage']['input_file'] = $input_file;
  $form_state['storage']['line_number'] = $line_number;
  form_clerk_curl_do_form($form_state);

  return;
}

/**
 * form_clerk input file configuration line processor.
 *
 * @param $line
 *  String containing the current line.
 * @$line_number
 *  The line number of the line within the current file.
 * @param $input_file
 *  Human readable name of the file.
 * @param $form_state
 *  The multiform state argument.
 */
function form_clerk_process_config($line, $line_number, $input_file, &$form_state) {
  $delimiter = $form_state['storage']['delimiter'];
  if (preg_match('/^FC_names/', $line)) {
    // Might look like: FC_names = name, mail, pass[pass1], pass[pass2], status
    $line = substr($line, strpos($line, '=')+1);
    // Break the line into individual form field names.
    $names = explode($delimiter, $line);
    $form_state['storage']['names'] = $names;
  }
  else {
    preg_match('/FC_(\w*)\s*=(.*)\s*/', $line, $matches);
    $key = $matches[1];
    $value = trim($matches[2]);
    // Convert string pseudo-booleans to booleans.
    if ($value == 'TRUE') {
      $value = TRUE;
    }
    elseif ($value == 'FALSE') {
      $value = FALSE;
    }
    if (($key == 'delimiter') && ($value == '\t')) {
        $value = "\t";
    }

    // Assume that all valid keys have previously defined defaults.
    if (!isset($form_state['storage'][$key])) {
      drupal_set_message(t('Unknown FC_ key "@key" in line #@line in file: @file',
        array(
              '@key'  => check_plain($key),
              '@line' => check_plain($line_number),
              '@file' => check_plain($input_file),
              )
        ),
        'warning');
    }

    $form_state['storage'][$key] = $value;

    // FC_url resets FC_form_id and FC_names.
    if ($key == 'url') {
      $form_state['storage']['form_id'] = '';
      $form_state['storage']['names'] = '';
      // Apply FC_site value.
      $form_state['storage']['url'] = $form_state['storage']['site'] . $value;
    }

    // Handle 'info_only' commands.
    if (($key == 'info_only') && $form_state['storage']['info_only']) {
      form_clerk_process_data('', $line_number, $input_file, $form_state);
    }
  }
  return;
}

/**
 * Handle 'info-only' requests..
 *
 * @param $form_state
 *  The multiform state argument.
 */
function form_clerk_process_form_info(&$form_state) {
  // Act as though we had an input file that looked like this:
  //  FC_url=http://example.com/login
  //  FC_form_id=user-login-form
  //  FC_names = name, pass
  //  form_clerk_admin, my_password
  //  FC_url=http://example.com/page-with-form
  //  FC_form_id=*
  //  FC_info_only=TRUE

  if ($form_state['storage']['info']['login']['login_url'] != '') {
    $line = 'FC_url=' . $form_state['storage']['info']['login']['login_url'];
    form_clerk_process_config($line, 0, 'UI', $form_state);

    $line = 'FC_form_id=' . $form_state['storage']['info']['login']['login_form_id'];
    form_clerk_process_config($line, 0, 'UI', $form_state);

    $line = 'FC_names = name, pass';
    form_clerk_process_config($line, 0, 'UI', $form_state);

    $line = $form_state['storage']['info']['login']['user'] . ',' . $form_state['storage']['info']['login']['password'];
    form_clerk_process_data($line, 0, 'UI', $form_state);
  }

  $line = 'FC_url=' . $form_state['storage']['info']['form_url'];
  form_clerk_process_config($line, 0, 'UI', $form_state);

  $line = 'FC_form_id=*';
  form_clerk_process_config($line, 0, 'UI', $form_state);

  $line = 'FC_names=*';
  form_clerk_process_config($line, 0, 'UI', $form_state);

  $line = 'FC_info_only=TRUE';
  form_clerk_process_config($line, 0, 'UI', $form_state);

  return;
}


/**
 * form_clerk input file helper function. Check readiness.
 *
 * @param $line
 *  String containing the current line.
 * @$line_number
 *  The line number of the line within the current file.
 * @param $input_file
 *  Human readable name of the file.
 * @param $form_state
 *  The multiform state argument.
 */
function form_clerk_process_ready($line, $line_number, $input_file, &$form_state) {
  // We need to have already seen FC_fields to proceed.
  if ($form_state['storage']['names'] == '') {
    drupal_set_message(t('FC_names must come before form data: Line #@line in file: @file',
      array('@line' => check_plain($line_number), '@file' => check_plain($input_file))), 'error');
    return FALSE;
  }
  // We need to have a form_id to proceed.
  if ($form_state['storage']['form_id'] == '') {
    drupal_set_message(t('FC_form_id must come before form data: Line #@line in file: @file',
      array('@line' => check_plain($line_number), '@file' => check_plain($input_file))), 'error');
    return FALSE;
  }
  return TRUE;
}

/**
 * form_clerk configuration panel processor.
 *
 * @param $form_state
 *  The multiform state argument.
 */
function form_clerk_process_save_config(&$form_state) {
  // Validate the directory value.
  if (isset($form_state['storage']['config']['directory']) &&
     ($form_state['storage']['config']['directory'] != '')) {
    $dir = check_plain($form_state['storage']['config']['directory']);
    $valid = ( is_writable($dir)
            && is_readable($dir)
            && is_writable("$dir/reports")
            && is_readable("$dir/inputs")
          ) ? TRUE : FALSE;
    if ($valid) {
      variable_set("form_clerk_directory", $dir);
    }
    else {
      drupal_set_message(t("Invalid directory value: '@dir'. Ensure that directory and subdirectories 'inputs' and 'reports' exist and are readable and writeable.",
        array('@dir' => check_plain($dir))), 'error');
      variable_set("form_clerk_directory", '/tmp');
      return FALSE;
    }
  }

  // This checkbox is always set one way or the other.
  variable_set("form_clerk_watchdog", $form_state['storage']['config']['watchdog']);


  return;
}
