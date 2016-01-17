<?php
/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_data_info().
 */
function ph_updates_rules_data_info() {
  // Define base info type.
  $info_type_base = array(
    'group' => t('Project update information'),
    'wrap' => TRUE,
    'is wrapped' => TRUE,
    'wrapper class' => 'PHUpdatesInfoWrapper',
  );
  $data_info['ph_updates_info'] = array(
    'label' => t('any update information'),
  ) + $info_type_base;

  // Define base tree type.
  $tree_type_base = array(
    'group' => t('Project update data tree'),
    'wrap' => TRUE,
    'is wrapped' => TRUE,
    'wrapper class' => 'PHUpdatesTreeWrapper',
  );
  $data_info['ph_updates_tree'] = array(
    'label' => t('any update data tree'),
  ) + $tree_type_base;

  // Populate individual types from update data info.
  foreach (ph_updates_get_info() as $type => $info) {
    $data_info["ph_updates_info<$type>"] = array(
      'label' => t('@type update information', array('@type' => $info['label'])),
      'parent' => 'ph_updates_info',
      'property info' => $info['property info'],
      'creation callback' => 'rules_action_data_create_array',
    ) + $info_type_base;
    $data_info["ph_updates_tree<$type>"] = array(
      'label' => t('@type update data tree', array('@type' => $info['label'])),
      'parent' => 'ph_updates_tree',
      'format info' => $info['format info'],
      'creation callback' => 'rules_action_data_create_array',
    ) + $tree_type_base;
  }

  return $data_info;
}

/**
 * Implements hook_rules_event_info().
 */
function ph_updates_rules_event_info() {
  return array(
    'ph_updates_project_info_build' => array(
      'label' => t('Building project update information'),
      'group' => t('Project Hosting'),
      'help' => t('Set update information properties to build update data for display in the update feed. The basic required properties for valid information are "Unique short name" and "API version".'),
      'variables' => array(
        'info' => array(
          'type' => ph_updates_get_data_type('info', 'project'),
          'label' => t('Project update information'),
        ),
        'name' => array(
          'type' => 'text',
          'label' => t('Requested project name'),
        ),
        'core_api' => array(
          'type' => 'ph_version',
          'label' => t('Requested API version'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function ph_updates_rules_action_info() {
  return array(
    'ph_updates_extract_tree' => array(
      'label' => t('Extract update data tree from information'),
      'group' => t('Project Hosting'),
      'parameter' => array(
        'info' => array(
          'label' => t('Update information'),
          'type' => 'ph_updates_info',
        ),
      ),
      'provides' => array(
        'tree' => array(
          'label' => t('Tree'),
          'type' => 'ph_updates_tree',
        ),
      ),
      'base' => 'ph_updates_rules_extract_tree',
    ),
  );
}

/**
 * Extracts tree from update info for Rules.
 */
function ph_updates_rules_extract_tree(PHUpdatesInfoWrapper $info) {
  $tree = ph_updates_extract_tree($info);
  return array('tree' => $tree);
}

/**
 * Alters extract tree action info based on parameter info type.
 */
function ph_updates_rules_extract_tree_info_alter(&$element_info, RulesPlugin $element) {
  $element->settings += array('info:select' => NULL);
  if ($wrapper = $element->applyDataSelector($element->settings['info:select'])) {
    if ($data_type = ph_updates_extract_data_type($type = $wrapper->type())) {
      $element_info['provides']['tree']['type'] = ph_updates_get_data_type('tree', $data_type);
    }
  }
}
