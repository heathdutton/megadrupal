<?php

function asaf_pagecache_form_callback() {
  require_once DRUPAL_ROOT . '/includes/form.inc';

  list($form, $form_state) = asaf_pagecache_get_form();
  drupal_process_form($form['#form_id'], $form, $form_state);

  // We need to return the part of the form (or some other content) that needs
  // to be re-rendered so the browser can update the page with changed content.
  // Since this is the generic menu callback used by many Ajax elements, it is
  // up to the #ajax['callback'] function of the element (may or may not be a
  // button) that triggered the Ajax request to determine what needs to be
  // rendered.
  if (!empty($form_state['triggering_element'])) {
    $callback = $form_state['triggering_element']['#ajax']['callback'];
  }
  if (!empty($callback) && function_exists($callback)) {
    return $callback($form, $form_state);
  }
}

function asaf_pagecache_get_form() {
  $form_state = form_state_defaults();

  $form_build_id = $_POST['form_build_id'];

  // Get the form from the cache.
  $form = form_get_cache($form_build_id, $form_state);
  if (!$form) {
    $asaf_token = $_POST['asaf_form'];

    if ($asaf_token && $asaf_token === asaf_get_security_token($_POST['form_id'], $_POST['form_build_id'])) {
      // Trying to get from cache and load files for the case when file with form constructor hasn't loaded yet.
      $options_cache = cache_get('asaf_form_' . $_POST['form_id'] . '_options', 'cache_form');
      $form_state['build_info']['files'] = isset($options_cache->data['needed_files']) && is_array($options_cache->data['needed_files']) ? $options_cache->data['needed_files'] : array();
      asaf_load_needed_files($form_state['build_info']['files']);

      if (isset($_POST['asaf_form_path'])) {
        $form_state['asaf']['form_path'] = $_POST['asaf_form_path'];
      }

      if (isset($_POST['asaf_id_prefix'])) {
        $form_state['asaf']['id_prefix'] = $_POST['asaf_id_prefix'];
      }

      // For asaf owns forms, which has asaf security token, rebuild form if it
      // doesn't have cache. It allows asaf works when page cache is enabled.
      $form = drupal_build_form($_POST['form_id'], $form_state);
    }
    else {
      // If $form cannot be loaded from the cache, the form_build_id in $_POST
      // must be invalid, which means that someone performed a POST request onto
      // system/ajax without actually viewing the concerned form in the browser.
      // This is likely a hacking attempt as it never happens under normal
      // circumstances, so we just do nothing.
      watchdog('ajax', 'Invalid form POST data.', array(), WATCHDOG_WARNING);
      drupal_exit();
    }
  }

  // Since some of the submit handlers are run, redirects need to be disabled.
  $form_state['no_redirect'] = TRUE;

  // When a form is rebuilt after Ajax processing, its #build_id and #action
  // should not change.
  // @see drupal_rebuild_form()
  $form_state['rebuild_info']['copy']['#build_id'] = TRUE;
  $form_state['rebuild_info']['copy']['#action'] = TRUE;

  // The form needs to be processed; prepare for that by setting a few internal
  // variables.
  $form_state['input'] = $_POST;
  $form_id = $form['#form_id'];

  return array($form, $form_state, $form_id, $form_build_id);
}