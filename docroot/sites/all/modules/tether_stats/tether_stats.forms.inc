<?php
/**
 * @file
 * Form callbacks for the tether_stats module.
 */

/**
 * Define the settings form.
 *
 * @see tether_stats_settings_form_validate()
 * @see tether_stats_settings_form_submit()
 */
function tether_stats_settings_form($form, &$form_state) {
  global $databases;

  $form = array();

  $form['database'] = array(
    '#type' => 'fieldset',
    '#title' => 'Database Settings',
    '#collapsible' => FALSE,
  );

  $options = array();

  foreach (array_keys($databases) as $key) {

    $options[$key] = $key;
  }

  $form['database']['db'] = array(
    '#type' => 'select',
    '#title' => t('Database Identifier'),
    '#description' => t('Defines which database definition the stats suite should use. The tables must be generated manually if using an alternative database.'),
    '#options' => $options,
    '#default_value' => variable_get('tether_stats_db', 'default'),
  );

  $form['page_filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Tracking Filter'),
    '#description' => t('Create path filter rules to exclude or include pages you wish to track stats on.'),
  );

  $form['page_filter']['tether_stats_exclusion_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Use Inclusion or Exclusion Mode'),
    '#description' => t('Choose to use the set of filter rules to either include or exclude matching pages for hit tracking. Inclusion mode will enforce that only urls matching one or more of the filter will be tracked. Contrarily, exclusion mode will enforce that all page urls will be tracked except those matched by the rules.'),
    '#options' => array(
      'exclude' => t('Exclude'),
      'include' => t('Include'),
    ),
    '#default_value' => variable_get('tether_stats_exclusion_mode', 'exclude'),
  );

  $default_filters = _tether_stats_filter_rules();

  $filter_rules_description = t('Enter a list of url paths matching rules, one per line, that will match pages you wish to include/exclude from stat tracking and element creation.') .
    '<ul><li>' . t('Wildcards "%" and "#" can be used to match any one url part respectively. "%" will match any string whereas "#" will only match numeric values.') .
    '</li><li><strong>' . t('Examples') .
    '</strong>:<br /> <ul><li><em>admin/config</em> - ' .
    t('Will match %example_base and any page under %example_base such as %example_url',
      array('%example_base' => 'admin/config', '%example_url' => 'admin/config/system/cron')) .
    '</li><li><em>admin/%/test</em> - ' .
    t('Will match %example_a and %example_b', array('%example_a' => 'admin/config/test', '%example_b' => 'admin/123/test')) .
    '</li><li><em>admin/#/test</em> - ' .
    t('Will match %example_a but not %example_b', array('%example_a' => 'admin/123/test', '%example_b' => 'admin/config/test')) .
    '</li></ul></li></ul>';

  $form['page_filter']['filter_rules'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages to Exclude/Include'),
    '#description' => $filter_rules_description,
    '#rows' => 5,
    '#default_value' => implode("\n", $default_filters),
  );

  $form['tether_stats_earliest_activation_time'] = array(
    '#type' => 'textfield',
    '#title' => 'Earliest Activation Time',
    '#description' => t('The unix timestamp of when stat collection is first activated. This value is set automatically and is used to determine how far back iterable charts are allowed to go. This may be modified here if importing data or to manually set a start boundary for charts with iteration buttons.'),
    '#default_value' => variable_get('tether_stats_earliest_activation_time', 0),
  );

  $form['tether_stats_allow_query_string_to_define_new_elements'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Query String To Define New Elements'),
    '#description' => t('By default, when a query string is passed to a page it is ignored. Setting this variable will modify this behavior and collects stats independently if a query string present. Each unique query string would define a new element as if it were a different page. This adds granularity but makes data mining more complex.'),
    '#default_value' => variable_get('tether_stats_allow_query_string_to_define_new_elements', FALSE),
  );

  $form['tether_stats_element_ttl'] = array(
    '#type' => 'textfield',
    '#title' => t('Element TTL'),
    '#description' => t('The amount of time, in seconds, that an element in the database is allowed to persist before updating. This is intended to correct any elements not matching up properly with URLs because of untracked edits or other conditions.'),
    '#default_value' => variable_get('tether_stats_element_ttl', 432000),
    '#element_validate' => array('element_validate_integer_positive'),
    '#required' => TRUE,
  );

  $form['tether_stats_track_404'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create Elements And Track Non-existent Or 404 Pages'),
    '#description' => t('If true, Tether Stats will generate a stat element and track activity for a page even if it resolves to a 404.'),
    '#default_value' => variable_get('tether_stats_track_404', FALSE),
  );

  $form['active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate Stat Data Collection'),
    '#description' => t('Toggles the collection of data into the stat tables. When this is false, no activity will be recorded.'),
    '#default_value' => variable_get('tether_stats_active', FALSE),
  );

  $form['#validate'] = array('tether_stats_settings_form_validate');
  $form['#submit'] = array('tether_stats_settings_form_submit');

  return system_settings_form($form);
}

/**
 * Validation callback for the settings form.
 *
 * @see tether_stats_settings_form()
 */
function tether_stats_settings_form_validate($form, &$form_state) {

  $database_id = $form_state['values']['db'];

  if ($database_id) {

    $test = db_set_active($database_id);

    db_set_active();

    if (!isset($test)) {

      form_error($form['database']['db'], t('Database identifier not found.'));
    }
  }
}

/**
 * Submit handler for the settings form.
 *
 * @see tether_stats_settings_form()
 */
function tether_stats_settings_form_submit($form, &$form_state) {

  variable_set('tether_stats_db', $form_state['values']['db']);

  variable_set('tether_stats_active', $form_state['values']['active']);

  if (empty($form_state['values']['active'])) {

    drupal_set_message(t('Stat data collection is off. No event data is currently being recorded'), 'warning');
  }
  else {

    // If the earliest time from which to iterate back to has not been set yet,
    // set it to now as this is where the data collection will likely begin now
    // that we are active.
    if (!$form_state['values']['tether_stats_earliest_activation_time']) {

      $form_state['values']['tether_stats_earliest_activation_time'] = REQUEST_TIME;
    }

    drupal_set_message(t('Stat data collection has been activated.'));
  }

  $filter_rules = array();

  foreach (explode("\n", $form_state['values']['filter_rules']) as $rule) {

    $rule = trim($rule);

    if ($rule) {

      $filter_rules[] = $rule;
    }
  }

  _tether_stats_filter_rules($filter_rules);
}

/**
 * Form for reviewing and managing derivatives.
 */
function tether_stats_admin_manage_derivatives_form($form, &$form_state) {

  $form = array();

  // Does not include the page type which we will add manually at the end.
  $entity_types = _tether_stats_get_entity_type_options();
  $count = 1;

  $all_derivatives = _tether_stats_get_derivatives();

  foreach ($entity_types as $entity_name => $entity_info) {

    $form["entity_{$count}"] = array(
      '#type' => 'fieldset',
      '#title' => t('Derivatives for Entity Type %entity_type', array('%entity_type' => $entity_name)),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    foreach ($entity_info as $type_bundle => $bundle_name) {

      list($entity_type, $bundle_id) = explode(':', $type_bundle);

      $form["entity_{$count}"][$bundle_id] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#entity_type' => $entity_type,
        '#title' => ($bundle_id == '*') ? t('Derivatives of any %entity_type entity', array('%entity_type' => $entity_name)) : t('Derivatives of bundle %entity_bundle', array('%entity_bundle' => $bundle_id)),
        '#theme' => 'tether_stats_admin_derivative_entity_form',
      );

      if (!empty($all_derivatives[$entity_type][$bundle_id])) {

        foreach ($all_derivatives[$entity_type][$bundle_id] as $derivative) {

          $form["entity_{$count}"][$bundle_id][$derivative->name] = array(
            '#type' => 'item',
            '#value' => $derivative->id,
            '#name' => $derivative->name,
            '#description' => check_plain($derivative->description),
            '#derivative' => $derivative,
          );

          $form["entity_{$count}"]['#collapsed'] = FALSE;
          $form["entity_{$count}"][$bundle_id]['#collapsed'] = FALSE;
        }
      }
    }
    $count++;
  }

  $form["entity_page"] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#entity_type' => 'page',
    '#title' => t('Generic Derivatives That Are Not Entity Type Specific'),
    '#description' => t('These derivatives can apply to any element, even if the element is not bound to an entity.'),
    '#theme' => 'tether_stats_admin_derivative_entity_form',
  );

  if (!empty($all_derivatives['*']['*'])) {

    foreach ($all_derivatives['*']['*'] as $derivative) {

      $form["entity_page"][$derivative->name] = array(
        '#type' => 'item',
        '#value' => $derivative->id,
        '#name' => $derivative->name,
        '#description' => check_plain($derivative->description),
        '#derivative' => $derivative,
      );

      $form["entity_page"]['#collapsed'] = FALSE;
    }
  }
  return $form;
}

/**
 * Form to add a new derivative.
 *
 * @see tether_stats_admin_derivatives_add_form_submit()
 */
function tether_stats_admin_derivatives_add_form($form, &$form_state) {

  $form = array();

  $options = _tether_stats_get_entity_type_options();

  // Add an option for generic derivatives which are not bound to entity
  // types.
  $options['Generic Derivative']['*:*'] = 'any entity_type or non-entity element';

  $form['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Entity Type'),
    '#description' => t('Select the entity type from which to derive a new tracking element set. A new derived element can be created for each entity of that entity_type.'),
    '#options' => $options,
    '#default_value' => 'node:*',
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Derivative Name'),
    '#description' => t('A unique machine name that identifies this derivative. If this value contains characters other than letter, numbers, "-" or "_" then it will be automatically converted into a proper machine name'),
    '#maxlength' => 32,
    '#size' => 32,
    '#required' => TRUE,
  );

  $form['machine-name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Derivative Name'),
    '#description' => t('A unique machine name that identifies this derivative.'),
    '#maxlength' => 32,
    '#required' => TRUE,
    '#machine_name' => array(
      'exists' => 'tether_stats_derivative_name_exists',
      'label' => 'Derivative',
      'source' => array('name'),
      'replace_pattern' => '[^a-z0-9_-]+',
    ),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add New Derivative'),
  );

  return $form;
}

/**
 * Submit handler for the add derviative form.
 *
 * @see tether_stats_admin_derivatives_add_form()
 */
function tether_stats_admin_derivatives_add_form_submit($form, &$form_state) {

  list($entity_type, $bundle_id) = explode(':', $form_state['values']['entity_type']);

  $fields = array(
    'name' => $form_state['values']['machine-name'],
    'description' => $form_state['values']['description'],
  );

  if ($entity_type != '*') {

    $fields['entity_type'] = $entity_type;
  }

  if ($bundle_id != '*') {

    $fields['entity_bundle'] = $bundle_id;
  }

  $old_db = db_set_active(tether_stats_set_db());
  db_insert('tether_stats_derivative')->fields($fields)->execute();
  db_set_active($old_db);

  $form_state['redirect'] = 'admin/config/system/tether_stats/derivatives';

  drupal_set_message(t('Added a new derivative %derivative for the entity type %entity_type. You may now use this derivative to define additional tracking elements.',
    array('%derivative' => $form_state['values']['machine-name'], '%entity_type' => $entity_type)));
}

/**
 * Form to confirm the deletion of a derivative.
 *
 * @see tether_stats_admin_derivatives_delete_confirm_form_submit()
 */
function tether_stats_admin_derivatives_delete_confirm_form($form, &$form_state, $derivative) {

  $return_path = 'admin/config/system/tether_stats/derivatives';

  $form = array(
    'destination' => array(
      '#type' => 'hidden',
      '#value' => $return_path,
    ),
    '#derivative' => $derivative,
  );

  $title = t('Are you sure you want to delete this derivative?');
  $description = t('The derivative %name will be permanently removed from the system.', array('%name' => $derivative->name));

  return confirm_form($form, $title, $return_path, $description, t('Delete'));
}

/**
 * Submit handler to delete a derivative.
 *
 * @see tether_stats_admin_derivatives_delete_confirm_form()
 */
function tether_stats_admin_derivatives_delete_confirm_form_submit($form, &$form_state) {

  $old_db = db_set_active(tether_stats_set_db());
  db_delete('tether_stats_derivative')->condition('id', $form['#derivative']->id, '=')->execute();
  db_set_active($old_db);

  drupal_set_message(t('The derivative %name has been deleted from the database.', array('%name' => $form['#derivative']->name)));

  $form_state['redirect'] = $form_state['values']['destination'];
}

/**
 * Form to confirm enabling a derivative.
 *
 * @see tether_stats_admin_derivatives_enable_confirm_form_submit()
 */
function tether_stats_admin_derivatives_enable_confirm_form($form, &$form_state, $derivative) {

  $return_path = 'admin/config/system/tether_stats/derivatives';

  $form = array(
    'destination' => array(
      '#type' => 'hidden',
      '#value' => $return_path,
    ),
    '#derivative' => $derivative,
  );

  $title = t('Are you sure you want to enable this derivative?');
  $description = t('The derivative %name will be enabled for stat collection.', array('%name' => $derivative->name));

  return confirm_form($form, $title, $return_path, $description, t('Enable'));
}

/**
 * Submit handler to enable a derivative.
 *
 * @see tether_stats_admin_derivatives_enable_confirm_form()
 */
function tether_stats_admin_derivatives_enable_confirm_form_submit($form, &$form_state) {

  $old_db = db_set_active(tether_stats_set_db());
  db_update('tether_stats_derivative')
    ->fields(array('disabled' => 0))
    ->condition('id', $form['#derivative']->id, '=')
    ->execute();
  db_set_active($old_db);

  drupal_set_message(t('The derivative %name has been enabled.', array('%name' => $form['#derivative']->name)));

  $form_state['redirect'] = $form_state['values']['destination'];
}

/**
 * Form to confirm disabling a derivative.
 *
 * @see tether_stats_admin_derivatives_disable_confirm_form_submit()
 */
function tether_stats_admin_derivatives_disable_confirm_form($form, &$form_state, $derivative) {

  $return_path = 'admin/config/system/tether_stats/derivatives';

  $form = array(
    'destination' => array(
      '#type' => 'hidden',
      '#value' => $return_path,
    ),
    '#derivative' => $derivative,
  );

  $title = t('Are you sure you want to disable this derivative?');
  $description = t('The derivative %name will be disabled from stat collection.', array('%name' => $derivative->name));

  return confirm_form($form, $title, $return_path, $description, t('Disable'));
}

/**
 * Submit handler to disable a derivative.
 *
 * @see tether_stats_admin_derivatives_disable_confirm_form()
 */
function tether_stats_admin_derivatives_disable_confirm_form_submit($form, &$form_state) {

  $old_db = db_set_active(tether_stats_set_db());
  db_update('tether_stats_derivative')
    ->fields(array('disabled' => 1))
    ->condition('id', $form['#derivative']->id, '=')
    ->execute();
  db_set_active($old_db);

  drupal_set_message(t('The derivative %name has been disabled.', array('%name' => $form['#derivative']->name)));

  $form_state['redirect'] = $form_state['values']['destination'];
}

/**
 * This form will allow you to find an element id given a set of identifiers.
 *
 * @see tether_stats_element_finder_form_validate()
 * @see tether_stats_element_finder_form_submit()
 */
function tether_stats_element_finder_form($form, &$form_state) {

  $form = array();

  $form['element_finder'] = array(
    '#type' => 'fieldset',
    '#title' => t('Element Finder'),
    '#description' => t('Use this form to find the element id matching an identifying set. Then you may review the statistics information tracked for that element.'),
  );

  $form['element_finder']['identity_type'] = array(
    '#type' => 'radios',
    '#options' => array(
      'find_by_entity' => t('Find by entity'),
      'find_by_url' => t('Find non-entity page by url'),
      'find_by_name' => t('Find by name'),
    ),
    '#default_value' => isset($form_state['values']['identity_type']) ? $form_state['values']['identity_type'] : 'find_by_entity',
  );

  $form['element_finder']['find_by_entity'] = array(
    '#type' => 'fieldset',
    '#title' => t('Find by Entity'),
    '#states' => array(
      'visible' => array(
        ':input[name="identity_type"]' => array('value' => 'find_by_entity'),
      ),
    ),
  );

  $options = array();

  foreach (entity_get_info() as $entity_type => $info) {

    $options[$entity_type] = $info['label'];
  }

  $entity_type = !empty($form_state['values']['entity_type']) ? $form_state['values']['entity_type'] : 'node';

  $form['element_finder']['find_by_entity']['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Entity Type'),
    '#description' => t('The entity_type for which the following entity_id applies.'),
    '#options' => $options,
    '#default_value' => $entity_type,
    '#ajax' => array(
      'callback' => 'tether_stats_element_finder_derivative_callback',
      'wrapper' => 'derivative-div',
      'method' => 'replace',
    ),
  );

  $form['element_finder']['find_by_entity']['entity_id'] = array(
    '#type' => 'textfield',
    '#title' => 'Entity Id',
    '#description' => t('The key for the entity of the specified entity type. This could be the nid, or uid etc depending on the context.'),
  );

  $options = array(0 => '- ' . t('none') . ' -');

  $all_derivatives = _tether_stats_get_derivatives();

  if (!empty($all_derivatives[$entity_type])) {

    foreach ($all_derivatives[$entity_type] as $bundle_derivatives) {
      foreach ($bundle_derivatives as $name => $derivative) {

        $options[$name] = $name;
      }
    }
  }

  $form['element_finder']['find_by_entity']['derivative'] = array(
    '#type' => 'select',
    '#title' => t('Derivative'),
    '#description' => t('The derivative identifier if you wish to find an element derived from another element.'),
    '#options' => $options,
    '#prefix' => '<div id="derivative-div">',
    '#suffix' => '</div>',
  );

  $form['element_finder']['find_by_name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Find by Name'),
    '#states' => array(
      'visible' => array(
        ':input[name="identity_type"]' => array('value' => 'find_by_name'),
      ),
    ),
  );

  $form['element_finder']['find_by_name']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('A unique string identifier that maps directly to an element. No other identity fields are required when an element is identified this way.'),
  );

  $form['element_finder']['find_by_url'] = array(
    '#type' => 'fieldset',
    '#title' => t('Find Non-entity Page by Url'),
    '#states' => array(
      'visible' => array(
        ':input[name="identity_type"]' => array('value' => 'find_by_url'),
      ),
    ),
  );

  $form['element_finder']['find_by_url']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => t('The url of the page to which an element is mapped to.'),
  );

  $form['element_finder']['query'] = array(
    '#type' => 'textfield',
    '#title' => t('Query String'),
    '#description' => t('The query string applied to the page.'),
  );

  $form['element_finder']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Find Element'),
  );

  $form['stats_element'] = array(
    '#type' => 'fieldset',
    '#title' => t('Element'),
    '#prefix' => '<div id="stats_element-div">',
    '#suffix' => '</div>',
  );

  if (isset($_SESSION['tether_stats']['elid'])) {

    if (module_exists('tether_stats_charts')) {

      $link = l(t('View report'), "admin/config/system/tether_stats/element/{$_SESSION['tether_stats']['elid']}");
    }
    else {

      $link = t("To view this element's stats, install the charts submodule");
    }

    $form['stats_element']['elid'] = array(
      '#type' => 'item',
      '#title' => t('Element Id'),
      '#markup' => "<p>{$_SESSION['tether_stats']['elid']}</p>",
      '#description' => $link,
    );
  }
  else {

    $form['stats_element']['elid'] = array(
      '#type' => 'item',
      '#markup' => '<p>' . t('Use the finder tool above find an element.') . '</p>',
    );
  }
  return $form;
}

/**
 * Validation handler for the element finder form.
 *
 * @see tether_stats_element_finder_form()
 */
function tether_stats_element_finder_form_validate($form, &$form_state) {

  $values =& $form_state['values'];

  switch ($form_state['values']['identity_type']) {

    case 'find_by_entity':
      if ($values['entity_id']) {

        if (!is_numeric($values['entity_id'])) {

          form_error($form['element_finder']['find_by_entity']['entity_id'], t('The <em>Entity Id</em> must be numeric.'));
        }
      }
      else {

        form_error($form['element_finder']['find_by_entity']['entity_id'], t('The <em>Entity Id</em> field is required.'));
      }
      break;

    case 'find_by_name':
      if (!$values['name']) {

        form_error($form['element_finder']['find_by_name']['name'], t('A <em>Name</em> identifying the element is required.'));
      }
      break;

    case 'find_by_url':
      if ($values['url']) {

        if (!drupal_valid_path($values['url'])) {

          form_error($form['element_finder']['find_by_url']['url'], t('The specified path is invalid or not recognized.'));
        }
      }
      else {

        form_error($form['element_finder']['find_by_url']['url'], t('The <em>Url</em> field is required.'));
      }
      break;

  }
}

/**
 * Submit handler for the element finder form.
 *
 * @see tether_stats_element_finder_form()
 */
function tether_stats_element_finder_form_submit($form, &$form_state) {

  $old_db = db_set_active(tether_stats_set_db());
  $query = db_select('tether_stats_element', 'e')->fields('e', array('elid'));

  switch ($form_state['values']['identity_type']) {

    case 'find_by_entity':
      $query = $query->condition('e.entity_type', $form_state['values']['entity_type'], '=')
        ->condition('e.entity_id', $form_state['values']['entity_id'], '=');
      break;

    case 'find_by_name':
      $query = $query->condition('e.name', $form_state['values']['name'], '=');
      break;

    case 'find_by_url':
      $query = $query->condition('e.url', $form_state['values']['url'], '=');
      break;

  }

  if (!empty($form_state['values']['derivative'])) {

    $query->join('tether_stats_derivative', 'd', 'e.derivative_id=d.id');
    $query->condition('d.name', $form_state['values']['derivative']);
  }
  else {

    $query->isNull('e.derivative_id');
  }

  if (!empty($form_state['values']['query'])) {

    $query->condition('e.query', $form_state['values']['query']);
  }
  else {

    $query->isNull('e.query');
  }

  $elid = $query->execute()->fetchField(0);
  db_set_active($old_db);

  if ($elid) {

    $_SESSION['tether_stats']['elid'] = $elid;
    drupal_set_message(t('The element id %elid has been found and added to the %fieldset fieldset below.', array('%elid' => $elid, '%fieldset' => 'Element')));
  }
  else {

    unset($_SESSION['tether_stats']['elid']);
    drupal_set_message(t('No matching element found.'), 'warning');
  }
}

/**
 * Callback to dynamically update the derivative select field.
 *
 * Called via AJAX in the form tether_stats_element_finder_form.
 *
 * @see tether_stats_element_finder_form()
 */
function tether_stats_element_finder_derivative_callback($form, $form_state) {

  return $form['element_finder']['find_by_entity']['derivative'];
}

/**
 * Callback to dynamically retrieve the found elid.
 *
 * Called via AJAX when the form tether_stats_element_finder_form is submitted
 * to dynamically add the found elid if applicable.
 *
 * @see tether_stats_element_finder_form()
 */
function tether_stats_element_finder_submit_callback($form, $form_state) {

  return $form['stats_element'];
}
