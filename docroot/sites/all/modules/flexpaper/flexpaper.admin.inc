<?php
/**
 * @file
 * Contains administrator's settings form and validators
 */

/**
 * Settings page form callback.
 */
function flexpaper_settings_page_form($form, &$form_state) {
  $form['flexpaper_path_to_pdf2swf'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to pdf2swf'),
    '#description' => t('For example /usr/local/bin/pdf2swf'),
    '#default_value' => variable_get('flexpaper_path_to_pdf2swf', 'pdf2swf'),
    '#size' => 100,
    '#maxlength' => 300,
    '#required' => TRUE,
  );
  $form['flexpaper_path_to_swfrender'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to swfrender'),
    '#description' => t('For example /usr/local/bin/swfrender'),
    '#default_value' => variable_get('flexpaper_path_to_swfrender', 'swfrender'),
    '#size' => 100,
    '#maxlength' => 300,
    '#required' => TRUE,
  );
  $form['flexpaper_path_to_pdf2json'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to pdf2json'),
    '#description' => t('For example /usr/local/bin/pdf2json'),
    '#default_value' => variable_get('flexpaper_path_to_pdf2json', 'pdf2json'),
    '#size' => 100,
    '#maxlength' => 300,
    '#required' => TRUE,
  );
  $form['flexpaper_license_key'] = array(
    '#type' => 'textfield',
    '#title' => t('License key'),
    '#description' => t('License key for the commercial version of the viewer.'),
    '#default_value' => variable_get('flexpaper_license_key', ''),
    '#size' => 100,
    '#maxlength' => 300,
  );
  return system_settings_form($form);
}

/**
 * Validation handler for settings page form.
 */
function flexpaper_settings_page_form_validate($form, &$form_state) {
  $path_to_pdf2swf = $form_state['values']['flexpaper_path_to_pdf2swf'];
  $path_to_swfrender = $form_state['values']['flexpaper_path_to_swfrender'];
  $path_to_pdf2json = $form_state['values']['flexpaper_path_to_pdf2json'];
  if (!flexpaper_pdf2swf_enabled($path_to_pdf2swf)) {
    form_set_error('flexpaper_path_to_pdf2swf', t('Path to pdf2swf tool is not properly set.'));
  }
  if (!flexpaper_swfrender_enabled($path_to_swfrender)) {
    form_set_error('flexpaper_path_to_swfrender', t('Path to swfrender tool is not properly set.'));
  }
  if (!flexpaper_pdf2json_enabled($path_to_pdf2json)) {
    form_set_error('flexpaper_path_to_pdf2json', t('Path to pdf2json tool is not properly set.'));
  }
}


/**
 * Check if pdf2swf tool is available.
 */
function flexpaper_pdf2swf_enabled($path_to_pdf2swf) {
  try {
    if (function_exists('exec') && (DIRECTORY_SEPARATOR == '/' || (DIRECTORY_SEPARATOR == '\\' && $path_to_pdf2swf != 'pdf2swf'))) {
      if (PHP_OS == "WIN32" || PHP_OS == "WINNT") {
        exec('"' . $path_to_pdf2swf . '"' . ' --version 2>&1', $out);
      }
      else {
        exec($path_to_pdf2swf . ' --version 2>&1', $out);
      }
      if (empty($out)) {
        return FALSE;
      }
      else {
        if (strpos($out[0], 'part of swftools') !== FALSE) {
          return TRUE;
        }
        else {
          return FALSE;
        }
      }
    }
    else {
      return FALSE;
    }
  }
  catch (Exception $ex) {
    return FALSE;
  }
}

/**
 * Check if swfrender tool is available.
 */
function flexpaper_swfrender_enabled($path_to_swf_render) {
  try {
    if (function_exists('exec') && (DIRECTORY_SEPARATOR == '/' || (DIRECTORY_SEPARATOR == '\\' && $path_to_swf_render != 'swfrender'))) {
      if (PHP_OS == "WIN32" || PHP_OS == "WINNT") {
        exec('"' . $path_to_swf_render . '" -help 2>&1', $out);
      }
      else {
        exec($path_to_swf_render . ' --version 2>&1', $out);
      }
      if (empty($out)) {
        return FALSE;
      }
      else {
        if (strpos($out[0], 'part of swftools') !== FALSE) {
          return TRUE;
        }
        else {
          return FALSE;
        }
      }
    }
    else {
      return FALSE;
    }
  }
  catch (Exception $ex) {
    return FALSE;
  }
}

/**
 * Check if pdf2json tool is available.
 */
function flexpaper_pdf2json_enabled($path_to_pdf2json) {
  try {
    if (function_exists('exec') && (DIRECTORY_SEPARATOR == '/' || (DIRECTORY_SEPARATOR == '\\' && $path_to_pdf2json != 'pdf2json'))) {
      if (PHP_OS == "WIN32" || PHP_OS == "WINNT") {
        exec('"' . $path_to_pdf2json . '" -help 2>&1', $out);
      }
      else {
        exec($path_to_pdf2json . ' -help 2>&1', $out);
      }
      if (empty($out)) {
        return FALSE;
      }
      else {
        if (strpos($out[0], 'devaldi') !== FALSE) {
          return TRUE;
        }
        else {
          return FALSE;
        }
      }
    }
    else {
      return FALSE;
    }
  }
  catch (Exception $ex) {
    return FALSE;
  }
}

/**
 * Helper function. Check that particular command tool exist.
 */
function _flexpaper_check_tool_exist($path, $check_word) {
  try {
    if (function_exists('exec')) {
      if (PHP_OS == "WIN32" || PHP_OS == "WINNT") {
        exec('"' . $path . '" -help 2>&1', $out);
      }
      else {
        exec($path . ' -help 2>&1', $out);
      }
      if (empty($out)) {
        return FALSE;
      }
      else {
        if (strpos($out[0], $check_word) !== FALSE) {
          return TRUE;
        }
        else {
          return FALSE;
        }
      }
    }
    else {
      return FALSE;
    }
  }
  catch (Exception $ex) {
    return FALSE;
  }
}
