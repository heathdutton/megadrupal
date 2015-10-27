<?php

/**
 * @file
 * Admin form handlers for SecKit Override.
 */

/**
 * Form generator for listing current override paths.
 */
function seckit_override_manage_paths($form, &$form_settings) {
  $overrides = seckit_override_load_paths();

  $form = array(
    '#tree' => TRUE,
    '#theme' => 'seckit_override_manage_paths',
    'overrides' => array(),
  );

  // Add some introductory text to explain the paths and inheritance.
  $form['intro'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('This table lists all existing override rules. Each
rule consists of a path pattern and a series of override settings. Each time a
page is visited on the site, it is compared against this list of patterns to
find all matching rules. Rules start at the root of the site, unless started
with the wildcard character, "*". The leading slash (/) character should not be
included in the path.') . '</p><p>' . t('The effective settings applied to any
page are based on a system of inheritance. First, the globlal settings
specified in !global are loaded. Then, each rule in this table which patches the
current request path is applied in order. If a given setting is set to %inherit
(or left blank in the case of text fields) then the existing setting is kept.
Otherwise, the new setting is applied. The process repeats for each matching
rule.',
      array(
        '%inherit' => t('Inherit'),
        '!global' => l(t('Security Kit Settings'), 'admin/config/system/seckit'),
      )
    ) . '<p></p>' . t('Drag rules and click "Save" to reorder the rules.') . '<p></p>',
  );

  // Populate the current rows, keeping track of the current max weight.
  $maxweight = 0;

  foreach ($overrides as $override) {
    $form['overrides'][$override->id] = array(
      'path' => array(
        '#type' => 'textfield',
        '#title' => t('Path pattern'),
        '#default_value' => $override->path,
        '#description' => t('The pattern to match for this override'),
        '#title_display' => 'invisible',
        '#required' => TRUE,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $override->weight,
        '#delta' => 50,
        '#title_display' => 'invisible',
      ),
    );

    if ($override->weight >= $maxweight) {
      $maxweight = $override->weight + 1;
    }
  }

  // Now add the new map section.
  $form['new'] = array(
    'path' => array(
      '#type' => 'textfield',
      '#title' => t('Path pattern'),
      '#title_display' => 'invisible',
      '#required' => FALSE,
    ),
    'weight' => array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => $maxweight,
      '#delta' => 50,
      '#title_display' => 'invisible',
    ),
  );

  // Action section.
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );

  return $form;
}

/**
 * Validation handler for seckit_override_manage_paths().
 */
function seckit_override_manage_paths_validate($form, &$form_settings) {
  if (isset($form_settings['values']['overrides'])) {
    foreach ($form_settings['values']['overrides'] as $pid => $override) {
      $errmsg = seckit_override_is_path_valid($override['path']);
      if ($errmsg) {
        form_error($form['overrides'][$pid]['path'], $errmsg);
      }
    }
  }

  // And now the new path.
  $errmsg = seckit_override_is_path_valid($form_settings['values']['new']['path']);
  if ($errmsg) {
    form_error($form['new']['path'], $errmsg);
  }
}

/**
 * Submit handler for seckit_override_manage_paths().
 */
function seckit_override_manage_paths_submit($form, &$form_settings) {
  if (isset($form_settings['values']['overrides'])) {
    foreach ($form_settings['values']['overrides'] as $pid => $override) {
      db_update('seckit_override_path')
        ->fields(array(
          'path' => $override['path'],
          'weight' => $override['weight'],
        ))
        ->condition('id', $pid)
        ->execute();
    }
  }

  // Now do the new path, if defined.
  $new = $form_settings['values']['new'];
  if ($new['path'] != '') {
    $pid = db_insert('seckit_override_path')
      ->fields(array(
        'path' => $new['path'],
        'weight' => $new['weight'],
    ))
    ->execute();

    // Redirect to the edit form.
    $form_settings['redirect']
      = 'admin/config/system/seckit/override/edit/' . $pid;
  }

  // Clear the cache.
  cache_clear_all('*', 'cache_seckit_override', TRUE);
}

/**
 * Returns HTML for a seckit_override_manage_paths form.
 *
 * @param array $vars
 *   An associative array containing:
 *   - form: the form to be formatte.
 *
 * @see seckit_override_manage_paths()
 *
 * @ingroup themeable
 */
function theme_seckit_override_manage_paths(array $vars) {
  $form = $vars['form'];

  $header = array(
    t('Path'),
    t('Weight'),
    array(
      'data' => t('Actions'),
      'colspan' => 2,
    ));

  $rows = array();

  foreach (element_children($form['overrides']) as $pid) {
    $form['overrides'][$pid]['weight']['#attributes']['class']
      = array('seckit-override-path-weight');

    $rows[] = array(
      'data' => array(
        drupal_render($form['overrides'][$pid]['path']),
        drupal_render($form['overrides'][$pid]['weight']),
        l(t('Edit'), 'admin/config/system/seckit/override/edit/' . $pid, array('query' => drupal_get_destination())),
        l(t('Delete'), 'admin/config/system/seckit/override/delete/' . $pid, array('query' => drupal_get_destination())),
      ),
      'class' => array('draggable'),
    );
  }

  // And now the new row.
  $form['new']['weight']['#attributes']['class']
    = array('seckit-override-path-weight');
  $rows[] = array(
    'data' => array(
      drupal_render($form['new']['path']),
      drupal_render($form['new']['weight']),
    ),
    'class' => array('draggable'),
  );

  $table_id = 'seckit-override-path-table';
  $output = drupal_render($form['intro']);

  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));

  $output .= drupal_render_children($form);

  drupal_add_tabledrag($table_id, 'order', 'sibling', 'seckit-override-path-weight');

  return $output;
}

/**
 * Form generator for managing overrides on a path.
 */
function seckit_override_edit_path($form, &$form_settings, $pid) {
  // Get seckit's form function.
  module_load_include('inc', 'seckit', 'includes/seckit.form');
  $form = array();

  // Get the existing settings.
  $overrides = seckit_override_load_override($pid);

  // First, capture any messaages we need to keep.
  $messages = drupal_get_messages(NULL, TRUE);

  // Load the seckit form as a template.
  $raw = seckit_admin_form();

  // Clear the message set by seckit.
  drupal_get_messages(NULL, TRUE);

  // Put back the messages that were there before.
  foreach ($messages as $type => $list) {
    foreach ($list as $message) {
      drupal_set_message(filter_xss($message), $type);
    }
  }

  // Use that template to build a values array structure.
  $values = array();
  seckit_override_values_array($raw, $values);

  // Now store our loaded values into the values array.
  foreach ($overrides['settings'] as $tree => $value) {
    $tree_array = explode(':', $tree);
    $element =& seckit_override_find_reference($values, $tree_array);
    $element = $value;
  }

  // Update the form for our use.
  seckit_override_modify_form($raw, $values);

  // Change the submit and validate handler to ours.
  $raw['#submit'] = array('seckit_override_edit_path_submit');
  $raw['#validate'] = array('seckit_override_edit_path_validate');

  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $pid,
  );

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('The pattern to match for this override.'),
    '#required' => TRUE,
    '#default_value' => $overrides['path'],
  );

  $form = array_merge($form, $raw);

  return $form;
}

/**
 * Validation handler for seckit_override_edit_path().
 */
function seckit_override_edit_path_validate($form, &$form_settings) {
  $errmsg = seckit_override_is_path_valid($form_settings['values']['path']);
  if ($errmsg) {
    form_error($form['path'], check_plain($errmsg));
  }
}

/**
 * Submit handler for seckit_override_edit_path().
 */
function seckit_override_edit_path_submit($form, &$form_settings) {
  $values = $form_settings['values'];
  $pid = $values['pid'];
  $path = $values['path'];

  unset($values['pid']);
  unset($values['path']);
  unset($values['submit']);
  unset($values['op']);
  unset($values['form_build_id']);
  unset($values['form_id']);
  unset($values['form_token']);

  $newdata = array();
  seckit_override_extract_values($values, $newdata);

  // Save the data.
  seckit_override_save_override($pid, array(
    'path' => $path,
    'overrides' => $newdata,
  ));

  // Clear the cache.
  cache_clear_all('*', 'cache_seckit_override', TRUE);

  // Redirect back to the main end page.
  $form_settings['redirect'] = 'admin/config/system/seckit/override';
}

/**
 * Form generation function to delete a mapping.
 */
function seckit_override_delete_path($form, &$form_settings, $pid) {
  $override = seckit_override_load_override($pid);
  $form = array(
    'pid' => array(
      '#type' => 'hidden',
      '#default_value' => $pid,
    ),
    'path' => array(
      '#type' => 'textfield',
      '#title' => t('Path'),
      '#default_value' => $override['path'],
      '#disabled' => TRUE,
    ),
  );

  return confirm_form(
    $form,
    t('Are you sure you wish to delete this override?'),
    'admin/config/system/seckit/override');
}

/**
 * Submit handler for seckit_override_delete_path().
 */
function seckit_override_delete_path_submit($form, &$form_settings) {
  $pid = $form_settings['values']['pid'];

  // Delete all settings first.
  db_delete('seckit_override_details')
    ->condition('pid', $pid)
    ->execute();

  // And now the main row.
  db_delete('seckit_override_path')
    ->condition('id', $pid)
    ->execute();

  // Clear the cache.
  cache_clear_all('*', 'cache_seckit_override', TRUE);

  // Redirect back to the main admin form.
  $form_settings['redirect'] = 'admin/config/system/seckit/override';
}

/**
 * Loads the list of overrides.
 */
function seckit_override_load_paths() {
  $rows = db_select('seckit_override_path', 'p')
    ->fields('p', array('id', 'path', 'weight'))
    ->orderBy('weight', 'ASC')
    ->execute()
    ->fetchAll();

  return $rows;
}

/**
 * Loads all of the data for a given override.
 */
function seckit_override_load_override($pid) {
  // Get the path.
  $rows = db_select('seckit_override_path', 'p')
    ->fields('p', array('path'))
    ->condition('id', $pid)
    ->execute()
    ->fetchAll();

  $return['path'] = $rows[0]->path;

  $rows = db_select('seckit_override_details', 'd')
    ->fields('d', array('tree', 'value'))
    ->condition('pid', $pid)
    ->execute()
    ->fetchAllKeyed();

  $return['settings'] = $rows;

  return $return;
}

/**
 * Modifies the source form to add 'Inherit' options and change defaults.
 */
function seckit_override_modify_form(&$form, $values) {
  // Look at each control at this level.
  foreach ($form as $name => $contents) {
    // Skip if it starts with a # sign.
    if (drupal_substr($name, 0, 1) == '#') {
      continue;
    }

    // Process based on the type.
    switch ($form[$name]['#type']) {
      case 'checkbox':
        // Change it to a select.
        $form[$name]['#type'] = 'select';
        // Add the options.
        $form[$name]['#options'] = array(
          SECKIT_OVERRIDE_INHERIT => t('Inherit'),
          '0' => t('Disable'),
          '1' => t('Enable'),
        );

        if (isset($values[$name])) {
          $form[$name]['#default_value'] = $values[$name];
        }
        else {
          $form[$name]['#default_value'] = SECKIT_OVERRIDE_INHERIT;
        }
        break;

      case 'select':
      case 'radio';
        // Add inherit as the first option.
        $form[$name]['#options'] = array(
          SECKIT_OVERRIDE_INHERIT => t('Inherit'),
        ) + $form[$name]['#options'];

        if (isset($values[$name])) {
          $form[$name]['#default_value'] = $values[$name];
        }
        else {
          $form[$name]['#default_value'] = SECKIT_OVERRIDE_INHERIT;
        }
        break;

      default:
        if (isset($values[$name]) && $values[$name] != SECKIT_OVERRIDE_INHERIT) {
          $form[$name]['#default_value'] = $values[$name];
        }
        else {
          unset($form[$name]['#default_value']);
        }
        break;
    }

    // Now process any child elements.
    if (isset($values[$name])) {
      seckit_override_modify_form($form[$name], $values[$name]);
    }
  }
}

/**
 * Reads the structure of the form and builds a corresponding array for values.
 */
function seckit_override_values_array($form, &$values) {
  foreach ($form as $key => $contents) {
    if (drupal_substr($key, 0, 1) != '#') {
      switch ($form[$key]['#type']) {
        case 'textfield':
        case 'textarea':
        case 'checkbox':
        case 'select':
          $values[$key] = SECKIT_OVERRIDE_INHERIT;
          break;

        case 'fieldset':
        case 'item':
          $values[$key] = array();

          // Now process any child controls.
          seckit_override_values_array($form[$key], $values[$key]);

          break;
      }
    }
  }
}

/**
 * Pulls data from a values array into a form usable in the database.
 */
function seckit_override_extract_values($values, &$newdata, $parent = '') {
  foreach ($values as $key => $data) {
    if (is_array($data)) {
      seckit_override_extract_values($data, $newdata, $parent . $key . ':');
    }
    elseif ($data !== '' && $data != SECKIT_OVERRIDE_INHERIT) {
      $newdata[$parent . $key] = $data;
    }
  }
}

/**
 * Saves all of the data for one override.
 */
function seckit_override_save_override($pid, $data) {
  $path = $data['path'];
  $overrides = $data['overrides'];

  db_update('seckit_override_path')
    ->fields(array('path' => $path))
    ->condition('id', $pid)
    ->execute();

  // Delete any existing overrides.
  db_delete('seckit_override_details')
    ->condition('pid', $pid)
    ->execute();

  // Put in the new overrides.
  foreach ($overrides as $tree => $value) {
    db_insert('seckit_override_details')
      ->fields(array(
        'pid' => $pid,
        'tree' => $tree,
        'value' => $value,
      ))
      ->execute();
  }
}

/**
 * Determines if the path is valid, and returns an error message if not.
 */
function seckit_override_is_path_valid($path) {
  if (drupal_substr($path, 0, 1) == '/') {
    return t('The path must not begin with a slash (/)');
  }

  // Make sure no < or > characters are present.
  if (strpbrk($path, '<>') !== FALSE) {
    return t('Invalid characters in path.');
  }
}
