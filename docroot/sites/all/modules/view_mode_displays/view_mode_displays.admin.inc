<?php

/**
 * @file
 * Admin area functions for view mode displays.
 */

/**
 * Menu callback; display listing of entity types to display view modes of.
 */
function view_mode_displays_overview() {
  $types = entity_get_info();
  $output = array();
  foreach (array_keys($types) as $type) {
    $output[] = view_mode_displays_type($type);
  }

  return $output;
}

/**
 * Menu callback; display an entity type's view modes and bundles.
 *
 * @param string $type
 *   Entity type to display.
 */
function view_mode_displays_type($type) {
  $info = entity_get_info($type);

  // Some entities should not be viewed but are used for storage, such as
  // wysiwyg_profile and taxonomy_vocabulary. These entities don't have a
  // "full" view mode.
  if (!isset($info['view modes']['full'])) {
    return;
  }

  $output = array();

  $output[$type] = array(
    '#type' => 'fieldset',
    '#title' => $info['label'],
    '#attached' => array('library' => array(array('system', 'drupal.collapse'))),
    '#attributes' => array('class' => array('collapsible')),
  );
  $empty_bundle = FALSE;

  $bundles = array();
  foreach ($info['bundles'] as $bundle => $bundle_info) {
    $bundles[] = l($bundle_info['label'], 'admin/structure/view-mode-displays/' . $type . '/bundles/' . $bundle);
  }
  if (empty($bundles)) {
    $bundles[] = t('No bundles');
    $empty_bundle = TRUE;
    // Collapse entity types that do not have bundles to reduce clutter.
    $output[$type]['#attributes']['class'][] = 'collapsed';
  }
  $output[$type]['bundles'] = array(
    '#items' => $bundles,
    '#theme' => 'item_list',
    '#title' => t('Bundles'),
  );

  $view_modes = array();
  foreach ($info['view modes'] as $view_mode => $view_mode_info) {
    // TODO: Remove TRUE condition and finish view mode form.
    if ($empty_bundle || TRUE) {
      $view_modes[] = $view_mode_info['label'];
    }
    else {
      $view_modes[] = l($view_mode_info['label'], 'admin/structure/view-mode-displays/' . $type . '/view-modes/' . $view_mode);
    }
  }
  $output[$type]['view_modes'] = array(
    '#items' => $view_modes,
    '#theme' => 'item_list',
    '#title' => t('View modes'),
  );

  return $output;
}

/**
 * Menu title callback; retrieve entity type name.
 *
 * @param string $type
 *   Entity type being displayed.
 */
function view_mode_displays_type_title($type) {
  $type_info = entity_get_info($type);
  $type_label = $type_info['label'];
  return "$type_label view modes";
}

/**
 * Menu callback; display a bundle's view mode displays.
 *
 * @param string $type
 *   Entity type to display.
 */
function view_mode_displays_type_bundle($type, $bundle) {
  $output = array();
  $user_supplied = isset($_GET['entity']) && !isset($_GET['random']);
  $entities = $entity_id = NULL;
  $type_info = entity_get_info($type);

  // Attach form to allow the user to specify the entity to render.
  $output['form'] = drupal_get_form('view_mode_displays_type_bundle_form', $type, $bundle);

  $query = db_select($type_info['base table'], 'b');
  $query->addField('b', $type_info['entity keys']['id'], 'id');
  $query->orderRandom();
  $query->range(0, 1);

  // If user supplied validate the entry.
  if ($user_supplied) {
    $query->condition('b.' . $type_info['entity keys']['id'], $_GET['entity']);
  }

  // Limit query to the bundle if the entity info gives us what we need. Some
  // entities do not have bundle keys as they only have one bundle, such as
  // users.
  if (array_key_exists('bundle keys', $type_info) &&
      array_search($type_info['bundle keys']['bundle'], $type_info['schema_fields_sql']['base table']) !== FALSE) {
    $query->condition('b.' . $type_info['bundle keys']['bundle'], $bundle);
  }

  // Allow modules to alter the query before it is used. Necessary as some
  // entities don't have their bundle field available in their base table, and
  // fail to supply another table in the entity info (taxonomy_term).
  $context = array(
    'type' => $type,
    'bundle' => $bundle,
  );
  drupal_alter('view_mode_displays_bundle_query', $query, $context);

  // Retrieve the entity id.
  $entity_id = $query->execute()->fetchField();

  if ($entity_id !== FALSE) {
    $entities = entity_load($type, array($entity_id));
  }
  if (empty($entities)) {
    if (!$user_supplied) {
      $output['empty']['#markup'] = t('The %bundle bundle does not contain any entities for display.',
          array('%bundle' => $type_info['bundles'][$bundle]['label']));
    }
    else {
      $output['empty']['#markup'] = t('%value does not match an entity identifier from the %bundle bundle. Please try another value.',
          array('%value' => $_GET['entity'], '%bundle' => $type_info['bundles'][$bundle]['label']));
    }
    return $output;
  }

  foreach ($type_info['view modes'] as $mode => $info) {
    $output[$mode] = array(
      '#type' => 'fieldset',
      '#title' => $info['label'],
      '#attached' => array('library' => array(array('system', 'drupal.collapse'))),
      '#attributes' => array('class' => array('collapsible')),
    );
    // Collapse view modes that do not have custom settings, from what we can
    // tell. Note that modules like Panelizer could obviously make this
    // assumption erroneous, but it is a sane default.
    if ($mode !== 'full' && !$info['custom settings']) {
      $output[$mode]['#attributes']['class'][] = 'collapsed';
    }
    $output[$mode]['entity'] = entity_view($type, $entities, $mode);
  }

  return $output;
}

/**
 * Autocomplete callback; allow user to search for an entity by bundle.
 */
function view_mode_displays_type_bundle_autocomplete($type, $bundle, $string) {
  $matches = array();

  // Query for matches.
  $type_info = entity_get_info($type);
  $query = db_select($type_info['base table'], 'b');
  $query->addField('b', $type_info['entity keys']['id'], 'id');
  $query->range(0, 10);

  // Conditions, see view_mode_displays_type_bundle() for details of why.
  if (isset($type_info['entity keys']['label'])) {
    $query->addField('b', $type_info['entity keys']['label'], 'label');
    $query->condition('b.' . $type_info['entity keys']['label'], '%' . db_like($string) . '%', 'LIKE');
  }
  if (array_key_exists('bundle keys', $type_info) &&
      array_search($type_info['bundle keys']['bundle'], $type_info['schema_fields_sql']['base table']) !== FALSE) {
    $query->condition('b.' . $type_info['bundle keys']['bundle'], $bundle);
  }
  $context = array(
    'type' => $type,
    'bundle' => $bundle,
    'autocomplete' => $string,
  );
  drupal_alter('view_mode_displays_bundle_query', $query, $context);

  $return = $query->execute();

  // Add matches to $matches and sanitize.
  foreach ($return as $row) {
    $matches[$row->id] = check_plain($row->label);
  }

  // Output result in JSON.
  drupal_json_output($matches);
}

/**
 * Form; allow user to specify entity ID to use for bundle rendering.
 */
function view_mode_displays_type_bundle_form($form, &$form_state) {
  list($type, $bundle) = $form_state['build_info']['args'];
  $form['#method'] = 'get';
  $form['entity'] = array(
    '#type' => 'textfield',
    '#title' => t('Entity to display'),
    '#autocomplete_path' => "ajax/view-mode-displays/bundle-autocomplete/$type/$bundle",
  );
  $form['submit'] = array('#type' => 'submit', '#name' => 'submit', '#value' => t('Display entity'));
  $form['random'] = array('#type' => 'submit', '#name' => 'random', '#value' => t('Display random'));
  return $form;
}

/**
 * Menu title callback; retrieve bundle page title.
 *
 * @param string $type
 *   Entity type being displayed.
 */
function view_mode_displays_type_bundle_title($type, $bundle) {
  $type_info = entity_get_info($type);
  $bundle_label = $type_info['bundles'][$bundle]['label'];
  return "$bundle_label bundle view modes";
}

/**
 * Menu callback; display an entity type's view mode displays.
 *
 * @param string $type
 *   Entity type to display.
 */
function view_mode_displays_type_view_mode($type, $view_mode) {
  $output = array();
  $type_info = entity_get_info($type);

  // TODO: Add form for specifying bundle ID or random.
  $entities = entity_load($type, array(5, 1));

  foreach ($entities as $entity_id => $entity) {
    $bundle = $entity->{$type_info['bundle keys']['bundle']};
    $output[$bundle] = array(
      '#type' => 'fieldset',
      '#title' => $type_info['bundles'][$bundle]['label'],
      '#attached' => array('library' => array(array('system', 'drupal.collapse'))),
      '#attributes' => array('class' => array('collapsible')),
    );
    $output[$bundle]['entity'] = entity_view($type, array($entity_id => $entity), $view_mode);
  }

  return $output;
}

/**
 * Menu title callback; retrieve view mode page title.
 *
 * @param string $type
 *   Entity type being displayed.
 */
function view_mode_displays_type_view_mode_title($type, $view_mode) {
  $type_info = entity_get_info($type);
  $view_label = $type_info['view modes'][$view_mode]['label'];
  return "$view_label view modes";
}
