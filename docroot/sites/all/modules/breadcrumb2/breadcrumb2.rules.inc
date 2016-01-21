<?php

/**
 * @file
 * Rules integration for breadcrumb2.
 *
 * @addtogroup rules
 * @{
 */
 
 /**
 * Implements hook_rules_file_info().
 */
function breadcrumb2_rules_file_info() {
  return array(
    'rules/breadcrumb2.taxonomy',
    'rules/breadcrumb2.eval',
    'rules/breadcrumb2.menu',
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function breadcrumb2_rules_condition_info() {
  return array(
    'breadcrumb2_breadcrumb_exist' => array(
      'label' => t('Breadcrumb object already exist.'),
      'parameter' => array(
        'path' => array('type' => 'text', 'label' => t('Path'), 'wrapped' => FALSE),
      ),
      'group' => t('Breadcrumb2'),
      'access callback' => 'breadcrumb2_rules_integration_access',
      'base' => 'breadcrumb2_rules_breadcrumb_exist',
    ),
    'breadcrumb2_internal_path_is' => array(
      'label' => t('Internal path is.'),
      'parameter' => array(
        'path' => array('type' => 'text', 'label' => t('Internal path'), 'wrapped' => FALSE),
      ),
      'group' => t('Breadcrumb2'),
      'access callback' => 'breadcrumb2_rules_integration_access',
      'base' => 'breadcrumb2_rules_internal_path_is',
    ),
  );
}

 /**
 * Implements hook_rules_action_info().
 */
function breadcrumb2_rules_action_info() {
  $items = array();

  $items['breadcrumb2_append_breadcrumb_trail'] = array(
    'label' => t('Append breadcrumb trail'),
    'group' => t('Breadcrumb2'),
    'parameter' => array(
      'breadcrumb' => array(
        'type' => 'breadcrumb2',
        'label' => t('Breadcrumb'),
        'description' => t('The breadcrumb object who will be appended.'),
        'wrapped' => TRUE,
        'save' => FALSE,
      ),
      'title' => array(
        'type' => 'text',
        'label' => t('title'),
        'wrapped' => FALSE,
      ),
      'url' => array(
        'type' => 'text',
        'label' => t('URL'),
        'wrapped' => FALSE,
      ),
    ),
    'base' => 'breadcrumb2_rules_append_breadcrumb_trail',
    'access callback' => 'breadcrumb2_rules_integration_access',
  );

  $items['breadcrumb2_fetch_lightest_term_from_node'] = array(
    'label' => t('Fetch lightest term from node'),
    'group' => t('Breadcrumb2'),
    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
        'wrapped' => TRUE,
        'save' => FALSE,
      ),
    ),
    'base' => 'breadcrumb2_rules_fetch_lightest_term_from_node',
    'access callback' => 'breadcrumb2_rules_integration_access',
    'provides' => array(
      'lightest_term' => array(
        'type' => 'taxonomy_term',
        'label' => t('First term'),
      ),
    ),
  );
  
  $items['breadcrumb2_taxonomy_get_parents_all'] = array(
    'label' => t('Taxonomy get parents all'),
    'group' => t('Breadcrumb2'),
    'parameter' => array(
      'taxonomy_term' => array(
        'type' => 'taxonomy_term',
        'label' => t('Taxonomy term'),
        'wrapped' => TRUE,
        'save' => FALSE,
      ),
    ),
    'base' => 'breadcrumb2_rules_taxonomy_get_parents_all',
    'access callback' => 'breadcrumb2_rules_integration_access',
    'provides' => array(
      'parent_terms' => array(
        'type' => 'list<taxonomy_term>',
        'label' => t('Parent terms'),
      ),
    ),
  );

  $items['breadcrumb2_get_argument_from_path'] = array(
    'label' => t('Get argument from Drupal path'),
    'group' => t('Breadcrumb2'),
    'parameter' => array(
      'index' => array(
        'type' => 'integer',
        'label' => t('The index of the path component'),
        'description' => t('The index of the component, where each component is separated by a "/", and where the first component has an index of 0.'),
        'restriction' => 'input',
		'default value' => 0,
      ),
    ),
    'base' => 'breadcrumb2_rules_get_argument_from_path',
    'access callback' => 'breadcrumb2_rules_integration_access',
    'provides' => array(
      'component' => array(
        'type' => 'text',
        'label' => t('Component of the current Drupal path'),
      ),
    ),
  );

  $items['breadcrumb2_delete_breadcrumb_by_path'] = array(
    'label' => t('Delete breadcrumb by path'),
    'group' => t('Breadcrumb2'),
    'parameter' => array(
      'path' => array(
        'type' => 'text',
        'label' => t('Internal path'),
      ),
    ),
    'base' => 'breadcrumb2_rules_delete_breadcrumb_by_path',
    'access callback' => 'breadcrumb2_rules_integration_access',
  );

  $items['breadcrumb2_disable_breadcrumb'] = array(
    'label' => t('Disable breadcrumb'),
    'group' => t('Breadcrumb2'),
    'base' => 'breadcrumb2_rules_disable_breadcrumb',
    'access callback' => 'breadcrumb2_rules_integration_access',
  );

  $items['breadcrumb2_menu_get_active_trail'] = array(
    'label' => t('Menu get active trail'),
    'group' => t('Breadcrumb2'),
    'base' => 'breadcrumb2_rules_menu_get_active_trail',
    'access callback' => 'breadcrumb2_rules_integration_access',
    'provides' => array(
      'active_trail' => array(
        'type' => 'list<menu_link>',
        'label' => t('Active trail'),
      ),
    ),
  );

  $items['breadcrumb2_menu_set_active_menu_names'] = array(
    'label' => t('Menu set active menu names'),
    'group' => t('Breadcrumb2'),
    'parameter' => array(
      'menu_names' => array(
        'type' => 'text',
        'label' => t('menu names'),
		'description' => t('Menu names seperate by comma ",".'),
        'restriction' => 'input',
		'default value' => 'navigation,main-menu',
      ),
    ),
    'base' => 'breadcrumb2_rules_menu_set_active_menu_names',
    'access callback' => 'breadcrumb2_rules_integration_access',
  );
  return $items;
}

/**
 * Breadcrumb2 rules integration access callback.
 */
function breadcrumb2_rules_integration_access($type, $name) {
  return TRUE;
}

/**
 * Implements hook_rules_data_info().
 */
function breadcrumb2_rules_data_info() {
  return array(
    'menu_link' => array(
      'label' => t('Menu link'),
      'wrap' => TRUE,
      'property info' => _breadcrumb2_rules_menu_link_info(),
    ),
  );
}

/**
 * Defines property info for menu link entries, used by the menu link data
 * type to provide an useful metadata wrapper.
 */
function _breadcrumb2_rules_menu_link_info() {
  return array(
    'title' => array(
      'type' => 'text',
      'label' => t('Title'),
    ),
    'href' => array(
      'type' => 'text',
      'label' => t('Href'),
    ),
    'link_path' => array(
      'type' => 'text',
      'label' => t('Link path'),
    ),
    'localized_options' => array(
      'type' => 'list',
      'label' => t('Localized options'),
    ),
    'type' => array(
      'type' => 'integer',
      'label' => t('Type'),
    ),
  );
}

