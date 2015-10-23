<?php

$plugin = array(
    'name' => 'Max Age',
    'module' => 'steroids',
    'admin_settings_form' => 'steroids_max_age_admin_settings_form',
    'hook_exit' => 'steroids_max_age_exit',
    'hook_node_view' => 'steroids_max_age_node_view',
    'hook_node_insert' => 'steroids_max_age_node_insert',
    'hook_node_update' => 'steroids_max_age_node_update',
    'node_form_alter' => 'steroids_max_age_node_form_alter',
);

/**
 * Implements the former hook_settings().
 */
function steroids_max_age_admin_settings_form() {
  $settings = variable_get('steroids_max_age', array());

  $form['steroids_max_age'] = array(
      '#type' => 'fieldset',
      '#title' => t('Max age'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
  );

  // pre-defined pages
  $form['steroids_max_age']['predefined'] = array(
      '#type' => 'fieldset',
      '#title' => t('Max ages for pre-defined pages'),
      '#description' => t('For a few special locations you can set a hard max-age that will take precedence over values set for a content-type. The default value is used when no max-age value has been set when hook_exit is reached.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
  );

  $predefined = array('frontpage', 'default');
  $default_settings = isset($settings['predefined']) ? $settings['predefined'] : array();
  foreach ($predefined as $type) {
    $form['steroids_max_age']['predefined']['max_age_predefined_' . $type] = array(
        '#type' => 'select',
        '#title' => t('Page cache maximum age for: @type', array('@type' => $type)),
        '#default_value' => (isset($default_settings['max_age_predefined_' . $type])) ? $default_settings['max_age_predefined_' . $type] : 0,
        '#options' => _max_age_periods(FALSE),
    );
  }

  // content-types
  $form['steroids_max_age']['defaults'] = array(
      '#type' => 'fieldset',
      '#title' => t('Max ages per content-type'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
  );

  $default_settings = isset($settings['defaults']) ? $settings['defaults'] : array();
  foreach (array_keys(node_type_get_types()) as $type) {
    $form['steroids_max_age']['defaults']['max_age_node_' . $type] = array(
        '#type' => 'select',
        '#title' => t('Page cache maximum age for node type: @type', array('@type' => $type)),
        '#default_value' => (isset($default_settings['max_age_node_' . $type])) ? $default_settings['max_age_node_' . $type] : 0,
        '#options' => _max_age_periods(FALSE),
    );
  }
  return $form;
}

/**
 * Delegation of node form alteration.
 */
function steroids_max_age_node_form_alter(&$form, &$form_state) {
  $form['cache_max_age'] = array(
      '#type' => 'fieldset',
      '#title' => t('Cache settings'),
      '#weight' => 5,
      '#group' => 'additional_settings',
  );

  $default_option = 'default';

  $max_age = NULL;
  if (!empty($form['#node']->nid)) {
    $max_age = db_select('node', 'n')
            ->fields('n', array('cache_max_age'))
            ->condition('nid', $form['#node']->nid)
            ->execute()
            ->fetchField();
  }
  
  if (!empty($max_age)) {
    $default_option = $max_age;
  }

  $form['cache_max_age']['cache_max_age'] = array(
      '#type' => 'select',
      '#title' => t('Cache timespan'),
      '#options' => _max_age_periods(),
      '#default_value' => $default_option,
      '#description' => t('The timespan of this content in the cache.'),
  );
}

/**
 * Delegation of hook_exit().
 */
function steroids_max_age_exit() {
  $max_age = _steroids_max_age_value();
  $settings = variable_get('steroids_max_age', array());

  // if this page is the frontpage, we overwrite other max-age settings
  if (drupal_is_front_page()) {
    if (isset($settings['predefined']['max_age_predefined_frontpage'])) {
      $max_age = $settings['predefined']['max_age_predefined_frontpage'];
    }
  }

  // if nothing has set a max-age so far, we revert to the default max-age
  if (empty($max_age)) {
    if (isset($settings['predefined']['max_age_predefined_default'])) {
      $max_age = $settings['predefined']['max_age_predefined_default'];
    }
  }

  if (!empty($max_age)) {
    drupal_add_http_header('Cache-Control', 'public, max-age=' . $max_age);
  }
}

/**
 * Delegation of hook_node_insert().
 */
function steroids_max_age_node_insert($node) {
  db_update('node')
          ->fields(array('cache_max_age' => $node->cache_max_age))
          ->condition('nid', $node->nid)
          ->execute();
}

/**
 * Delegation of hook_node_update().
 */
function steroids_max_age_node_update($node) {
  db_update('node')
          ->fields(array('cache_max_age' => $node->cache_max_age))
          ->condition('nid', $node->nid)
          ->execute();
}

function steroids_max_age_node_view($node) {
  // Only on detail pages.
  if (current_path() === 'node/' . $node->nid) {
    $max_age = '';
    $max_age = db_select('node', 'n')
      ->fields('n', array('cache_max_age'))
      ->condition('nid', $node->nid)
      ->execute()
      ->fetchField();

    if ((empty($max_age) || $max_age === 'default') && $max_age !== '0') {
      $settings = variable_get('steroids_max_age', array());

      if (isset($settings['defaults']) && isset($settings['defaults']['max_age_node_' . $node->type])) {
        $max_age = $settings['defaults']['max_age_node_' . $node->type];
      }
    }

    _steroids_max_age_value($max_age);
  }
}

/**
 * Static function to hold the max age of a specific entity.
 *
 * It gets picked up by hook_exit.
 */
function _steroids_max_age_value($value = NULL) {
  static $max_age;

  if (isset($max_age)) {
    return $max_age;
  }

  $max_age = $value;

  return $max_age;
}

/**
 *
 */
function _max_age_periods($add_default_option = TRUE) {
  $periods = drupal_map_assoc(array(0, 60, 180, 300, 600, 900, 1800, 2700, 3600, 10800, 21600, 32400, 43200, 86400, 86400 * 2, 86400 * 5, 86400 * 7, 86400 * 14, 86400 * 21, 86400 * 28), 'format_interval');
  $periods[0] = '<' . t('No cache') . '>';

  if ($add_default_option) {
    $periods['default'] = '<' . t('Default') . '>';
  }

  return $periods;
}
