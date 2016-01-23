<?php
/**
 * @file
 * Administrative forms.
 */

use Drupal\GitClone\GitClone;

/**
 * Helper function for retrieving the correct default value.
 *
 * @param string $name
 *   The name of the GitClone property.
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param string|mixed $default_value
 *   The default value to provide if nothing set on GitClone or the $form_state
 *   values.
 *
 * @return mixed
 *   The default value.
 */
function _git_clone_form_default_value($name, array $form, array $form_state, $default_value = '') {
  $clone = entity_ui_form_submit_build_entity($form, $form_state);
  if ($name === 'reference') {
    if (!empty($clone->refType) && !empty($clone->ref)) {
      $value = $clone->refType . '|' . $clone->ref;
    }
  }
  else {
    $value = $clone->$name;
  }
  if (isset($value)) {
    return is_array($default_value) && !is_array($value) ? array($value) : $value;
  }
  return $default_value;
}

/**
 * Helper function for retrieving the correct default setting value.
 *
 * @param string $name
 *   The name of the GitClone property.
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param string|mixed $default_value
 *   The default value to provide if nothing set on GitClone or the $form_state
 *   values.
 *
 * @return mixed
 *   The default value.
 */
function _git_clone_form_default_setting_value($name, array $form, array $form_state, $default_value = '') {
  $settings = _git_clone_form_default_value('settings', $form, $form_state, array());
  $value = isset($settings[$name]) ? $settings[$name] : NULL;
  if (isset($value)) {
    return is_array($default_value) && !is_array($value) ? array($value) : $value;
  }
  return $default_value;
}

/**
 * Implements ENTITY_form().
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param GitClone $clone
 *   The entity to be associated with this form.
 * @param string $op
 *   The current operation being performed.
 *
 * @return array
 *   The constructed form array.
 */
function git_clone_form(array $form, array &$form_state, GitClone $clone, $op) {
  // Ensure there is always a "values" key in $form_state.
  if (!isset($form_state['values'])) {
    $form_state['values'] = array();
  }

  $form_steps = array(
    array('form' => 'git_clone_form_identity'),
    array('form' => 'git_clone_form_reference'),
    array('form' => 'git_clone_form_url'),
  );

  // Clone.
  if ($op === 'clone') {
    foreach (array('name', 'title') as $key) {
      $clone->$key = NULL;
    }
    $op = 'add';
  }

  // Add the actions container.
  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );

  // Add.
  $save = TRUE;
  if ($op === 'add') {
    // Initialize a description of the steps for the wizard.
    if (empty($form_state['step'])) {
      $form_state['step'] = 0;
      $form_state['step_information'] = array_reverse($form_steps);
    }
    $step = &$form_state['step'];
    $step_info = &$form_state['step_information'][$step];
    $save = $step >= (count($form_state['step_information']) - 1);

    // Merge in stored values from other completed steps, if multi-step form.
    foreach ($form_state['step_information'] as $step_index => $info) {
      if ($step_index <= $step && isset($info['stored_values'])) {
        $form_state['values'] = array_merge($form_state['values'], $info['stored_values']);
      }
    }

    // Rebuild the entity with stored values.
    entity_ui_form_submit_build_entity($form, $form_state);

    // Retrieve the current step's form.
    $args = array($form, &$form_state, $clone, $op);
    $form = call_user_func_array($step_info['form'], $args);

    // Provide the necessary form actions.
    if ($step > 0) {
      $form['actions']['prev'] = array(
        '#type' => 'submit',
        '#value' => t('Previous'),
        '#limit_validation_errors' => array(),
      );
    }
    if (!$save) {
      $form['actions']['next'] = array(
        '#type' => 'submit',
        '#value' => t('Next'),
      );
    }

    // Validate steps, if they exist.
    if (function_exists($step_info['form'] . '_validate')) {
      $form['#validate'][] = $step_info['form'] . '_validate';
    }
  }
  else {
    // Retrieve each step's form.
    foreach ($form_steps as $step_info) {
      $args = array($form, &$form_state, $clone, $op);
      $form = call_user_func_array($step_info['form'], $args);
    }
  }

  // Add in the settings and "save" button.
  if ($save) {
    $args = array($form, &$form_state, $clone, $op);
    $form = call_user_func_array('git_clone_form_settings', $args);

    // Provide the necessary form actions.
    $form['actions']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save git clone'),
    );
  }

  return $form;
}

/**
 * The "settings" fieldset.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param GitClone $clone
 *   The current entity.
 *
 * @return array
 *   The constructed form.
 */
function git_clone_form_settings(array $form, array &$form_state, GitClone $clone) {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
  );

  $fetch_threshold = drupal_map_assoc(array(
    60, 180, 300, 600, 900, 1800, 3600, 7200, 10800, 21600, 32400,
    43200, 64800, 86400, 172800, 259200, 604800, 1209600, 2419200,
  ), 'format_interval');
  $form['settings']['fetch_threshold'] = array(
    '#type' => 'select',
    '#title' => t('Fetch Threshold'),
    '#description' => t('The amount of time that must pass before the git clone will be queued and fetched during a cron run. This time is relative to when the git clone was last fetched successfully, not the time cron was initiated.'),
    '#options' => $fetch_threshold,
    '#default_value' => _git_clone_form_default_setting_value('fetch_threshold', $form, $form_state, array(3600)),
  );
  return $form;
}

/**
 * Implements ENTITY_form_submit().
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function git_clone_form_submit(array $form, array &$form_state) {
  $op = $form_state['values']['op'];
  form_state_values_clean($form_state);

  $step = &$form_state['step'];
  $step_info = &$form_state['step_information'][$step];
  $step_info['stored_values'] = $form_state['values'];

  // Handle "previous" step submit.
  if ($op === t('Previous') && $step > 1) {
    $clone = entity_ui_form_submit_build_entity($form, $form_state);
    foreach (element_children($form) as $key) {
      if (isset($clone->$key)) {
        $clone->$key = NULL;
      }
    }
    $step--;
    $step_info = &$form_state['step_information'][$step];
    $form_state['values'] = $step_info['stored_values'];
    $form_state['rebuild'] = TRUE;
    return;
  }

  // Handle "next" step submit.
  if ($op === t('Next') && $step < count($form_state['step_information'])) {
    $step++;
    $step_info = &$form_state['step_information'][$step];
    if (!empty($step_info['stored_values'])) {
      $form_state['values'] = $step_info['stored_values'];
    }
    else {
      $form_state['values'] = array();
    }
    $form_state['rebuild'] = TRUE;
    return;
  }

  // Handle final form submit.
  if ($op === t('Save git clone')) {
    $values = array();
    if (isset($form_state['step_information'])) {
      foreach ($form_state['step_information'] as $index => $_step) {
        foreach ($_step['stored_values'] as $key => $value) {
          $values[$key] = $value;
        }
      }
      $form_state['values'] = $values;
    }

    /** @var GitClone $clone */
    $clone = entity_ui_form_submit_build_entity($form, $form_state);

    // Save the entity.
    $ret = $clone->save();
    if ($ret === SAVED_NEW || $ret = SAVED_UPDATED) {
      // Display the correct message and go to the correct redirect path.
      drupal_set_message(t('Successfully @action the "@label" git clone.', array(
        '@action' => $ret === SAVED_NEW ? t('created') : t('updated'),
        '@label' => $clone->label(),
      )));
    }
    else {
      drupal_set_message(t('An unknown error occurred. Could not create the git clone.'), 'error');
    }
    $form_state['redirect'] = GIT_CLONE_ADMIN_PATH;
  }
}

/**
 * The "url" step callback.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param GitClone $clone
 *   The current entity.
 *
 * @return array
 *   The constructed form.
 */
function git_clone_form_url(array $form, array &$form_state, GitClone $clone) {
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Repository URL'),
    '#description' => t('The URL to a publicly hosted git repository.'),
    '#default_value' => _git_clone_form_default_value('url', $form, $form_state),
    '#required' => TRUE,
  );
  return $form;
}

/**
 * The "url" step validation callback.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function git_clone_form_url_validate(array $form, array &$form_state) {
  /** @var GitClone $clone */
  $clone = entity_ui_form_submit_build_entity($form, $form_state);

  // Determine if URL is valid by attempting to retrieve its references.
  if (!$clone->getRefs(TRUE)) {
    $message = t('The provided URL is not a valid public remote git repository.');
    if (strpos($form_state['values']['url'], '@') !== FALSE) {
      $message .= ' ' . t('Please provide a URL that does not need user authentication.');
    }
    form_set_error('url', $message);
  }
}

/**
 * The "reference" step callback.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param GitClone $clone
 *   The current entity.
 * @param string $op
 *   The current operation being performed.
 *
 * @return array
 *   The constructed form.
 */
function git_clone_form_reference(array $form, array &$form_state, GitClone $clone, $op) {
  $default = array();
  $options = array();
  $type_labels = array(
    'branch' => t('Branch'),
    'tag' => t('Tag'),
  );
  if (!empty($clone->refs)) {
    foreach ($clone->refs as $type => $refs) {
      foreach ($refs as $ref) {
        $key = $type . '|' . $ref['name'];
        $value = $ref['name'];
        if ($type === 'branch' && $clone->settings['default'] === $ref['name']) {
          $default = array($key);
          $value .= ' (default)';
        }
        $options[$type_labels[$type]][$key] = $value;
      }
    }
  }
  $form['reference'] = array(
    '#type' => 'select',
    '#title' => t('Repository reference'),
    '#description' => t('This list is cached and regenerated during cron. You can either wait for cron or manually clear the site cache.'),
    '#options' => $options,
    '#default_value' => _git_clone_form_default_value('reference', $form, $form_state, $default),
    '#process' => array_merge(element_info_property('select', '#process', array()), array('_git_clone_form_reference_process')),
  );
  return $form;
}

/**
 * The "reference" element process callback.
 *
 * @param array $element
 *   The element build array.
 *
 * @return array
 *   The modified element.
 */
function _git_clone_form_reference_process(array $element) {
  // Split the selected key into the correct entity properties.
  $value = $element['#value'];
  $ref_type = NULL;
  $ref = NULL;
  if (is_array($value)) {
    $value = reset($value);
  }
  if ($value) {
    list($ref_type, $ref) = explode('|', $value);
  }
  $element['refType'] = array(
    '#type' => 'hidden',
    '#value' => $ref_type,
  );
  $element['ref'] = array(
    '#type' => 'hidden',
    '#value' => $ref,
  );
  return $element;
}

/**
 * The "identity" step callback.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param GitClone $clone
 *   The current entity.
 *
 * @return array
 *   The constructed form.
 */
function git_clone_form_identity(array $form, array &$form_state, GitClone $clone) {
  // Machine name pattern.
  $pattern = '[^a-z0-9-_\.]+';

  // Extract the repository "name" from the last portion of the URL and also
  // remove the ".git" suffix (if applicable).
  $parts = explode('/', $clone->url);
  $name = array_pop($parts);
  $name = preg_replace('/\.git$/', '', $name);
  $name = drupal_strtolower(preg_replace("/$pattern/i", '_', $name));

  // Create the title from the machine name and append the reference.
  $title = ucwords(str_replace('_', ' ', $name)) . ': ' . $clone->ref;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Display title'),
    '#description' => t('A human readable title to identify the cloned git repository.'),
    '#default_value' => _git_clone_form_default_value('title', $form, $form_state, $title),
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#title' => t('Machine name'),
    '#type' => 'machine_name',
    '#description' => t('A unique machine-readable name to identify the cloned git repository.'),
    '#default_value' => _git_clone_form_default_value('name', $form, $form_state, ''),
    '#disabled' => !isset($clone->is_new),
    '#machine_name' => array(
      'exists' => '_git_clone_form_identity_validate_machine_name',
      'source' => array('title'),
      'label' => t('Machine name'),
      'replace_pattern' => $pattern,
      'replace' => '_',
    ),
  );
  return $form;
}

/**
 * The "name" machine name validation callback.
 *
 * @param string $name
 *   The machine name to validate.
 * @param array $element
 *   The element build array.
 * @param array $form_state
 *   The form state array.
 *
 * @return bool
 *   TRUE if the machine name exists, FALSE otherwise.
 */
function _git_clone_form_identity_validate_machine_name($name, array $element, array $form_state) {
  $exists = FALSE;

  // Only validate if we're actually attempting to save the entity.
  if ($form_state['values']['op'] === t('Save git clone')) {
    /** @var GitClone $clone */
    $clone = entity_ui_form_submit_build_entity($form_state['complete form'], $form_state);
    // Ensure the top level git clone directory can be created.
    $exists = $clone->getPath(FALSE);
    if ($exists) {
      if (entity_access('edit', 'git_clone')) {
        drupal_set_message(t('If there are no visible git clones matching this machine name in the UI, visit the <a href="!url" target="_blank">advanced</a> tab to purge possible artifact directories.', array(
          '!url' => url(GIT_CLONE_ADMIN_PATH . '/advanced'),
        )), 'info');
      }
      else {
        watchdog('git_clone', 'Unable to create git clone due to existing machine name: @name', array(
          '@name' => $clone->name,
        ), WATCHDOG_INFO);
      }
    }
  }

  return $exists;
}

/**
 * The "advanced" tab menu callback.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 *
 * @return array
 *   The constructed form.
 */
function git_clone_advanced_form(array $form, array &$form_state) {
  $form['directories'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manage Directories'),
  );
  $form['directories']['check'] = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array(),
    0 => array(
      '#type' => 'submit',
      '#value' => t('Verify directories'),
      '#description' => t('Verifies existing git clone directories and displays a message showing current entity validity.'),
    ),
  );
  $form['directories']['purge'] = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array(),
    0 => array(
      '#type' => 'submit',
      '#value' => t('Purge artifacts'),
      '#description' => t('Does the same as the verify action above, but will also remove any artifact directories found.'),
    ),
  );
  return $form;
}

/**
 * The "advanced" tab menu validation callback.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function git_clone_advanced_form_submit(array $form, array &$form_state) {
  $op = $form_state['values']['op'];
  if ($op === t('Verify directories')) {
    $op = 'verify';
  }
  elseif ($op === t('Purge artifacts')) {
    $op = 'purge';
  }

  if ($op === 'verify' || $op === 'purge') {
    $clones = array();
    $artifacts = array();
    $entities = entity_load('git_clone');
    $branches = file_scan_directory('gitclone://branch', '/.+/', array('recurse' => FALSE));
    $tags = file_scan_directory('gitclone://tag', '/.+/', array('recurse' => FALSE));
    $directories = array_merge($branches, $tags);
    foreach ($directories as $directory) {
      /** @var Drupal\GitClone\StreamWrapper $stream_wrapper */
      $stream_wrapper = file_stream_wrapper_get_instance_by_uri($directory->uri);
      $realpath = $stream_wrapper->realpath();

      $found = FALSE;
      /** @var GitClone $clone */
      foreach ($entities as $clone) {
        if ($clone->getPath(FALSE) === $realpath) {
          $found = TRUE;
          break;
        }
      }

      if ($found) {
        $entity_info = entity_get_info($clone->entityType());
        $clones[] = l($clone->label(), $entity_info['admin ui']['path'] . '/manage/' . entity_id($clone->entityType(), $clone)) . ' (' . $stream_wrapper->realpath() . ')';
      }
      else {
        if ($op === 'purge') {
          file_unmanaged_delete_recursive($realpath);
        }
        $artifacts[] = $realpath;
      }
    }

    if ($clones) {
      $count = count($clones);
      drupal_set_message(format_plural($count, 'Git clone directory found:<br/>!directories', 'Git clone directories found:<br/>!directories', array(
        '!directories' => theme('item_list', array('items' => $clones)),
      )));
    }
    else {
      drupal_set_message(t('No git clones found.'));
    }

    if ($op === 'purge') {
      if ($artifacts) {
        $count = count($artifacts);
        drupal_set_message(format_plural($count, 'Artifact directory found and removed:<br/>!directories', 'Artifact directories found and removed:<br/>!directories', array(
          '!directories' => theme('item_list', array('items' => $artifacts)),
        )));
      }
      else {
        drupal_set_message(t('No artifact directories found.'));
      }
    }
    else {
      if ($artifacts) {
        $count = count($artifacts);
        drupal_set_message(format_plural($count, 'Artifact directory found:<br/>!directories', 'Artifact directories found:<br/>!directories', array(
          '!directories' => theme('item_list', array('items' => $artifacts)),
        )));
      }
      else {
        drupal_set_message(t('No artifact directories found.'));
      }
    }
  }
}
