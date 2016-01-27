<?php

require_once 'm2.constants.inc';

/**
 * m2_admin_settings_page
 * m2_admin_settings_form
 */

function m2_admin_settings_page() {
  drupal_set_title(t('Administer Menu2'));
  $output = m2_form_render('m2_admin_settings_form');
  return theme('m2_content_wrapper', array('class' => array('m2-tab', 'm2-settings-tab', 'clearfix'), 'markup' => $output));
}


function m2_admin_settings_form($form, &$form_state) {
  m2_set_transaction_warning();
  $permissions = m2_permission();

/* block settings */
  $form['blocks_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Blocks settings'),
  );
  $form['blocks_settings']['m2_block_menu_num'] = array(
    '#type' => 'select',
    '#title' => t('Number of Menu2 blocks'),
    '#description' => t('The total number of independent Menu2 blocks you want.'),
    '#options' => range(0, M2_BLOCK_MENU_MAX_NUM),
    '#default_value' => variable_get('m2_block_menu_num', M2_BLOCK_MENU_DEFAULT_NUM),
    '#required' => true,
  );
  $form['blocks_settings']['m2_block_menu_multi_num'] = array(
    '#type' => 'select',
    '#title' => t('Number of multiple Menu2 blocks'),
    '#description' => t('The total number of independent multiple Menu2 blocks you want.'),
    '#options' => range(0, M2_BLOCK_MENU_MULTI_MAX_NUM),
    '#default_value' => variable_get('m2_block_menu_multi_num', M2_BLOCK_MENU_MULTI_DEFAULT_NUM),
    '#required' => true,
  );
  $form['blocks_settings']['m2_block_prev_next_links_num'] = array(
    '#type' => 'select',
    '#title' => t('Number of blocks with Previous and Next links'),
    '#description' => t('The total number of independent blocks with Previous and Next links you want.'),
    '#options' => range(0, M2_BLOCK_PREV_NEXT_LINKS_MAX_NUM),
    '#default_value' => variable_get('m2_block_prev_next_links_num', M2_BLOCK_PREV_NEXT_LINKS_DEFAULT_NUM),
    '#required' => true,
  );
  $form['blocks_settings']['m2_block_breadcrumb_num'] = array(
    '#type' => 'select',
    '#title' => t('Number of Breadcrumb blocks'),
    '#description' => t('The total number of independent Breadcrumb blocks you want.'),
    '#options' => range(0, M2_BLOCK_BREADCRUMB_MAX_NUM),
    '#default_value' => variable_get('m2_block_breadcrumb_num', M2_BLOCK_BREADCRUMB_DEFAULT_NUM),
    '#required' => true,
  );
  $form['blocks_settings']['m2_block_show_time_to_select'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Build time (for role %role)', array('%role' => $permissions['administer menu2']['title'])),
    '#default_value' => variable_get('m2_block_show_time_to_select', true),
  );
  $form['blocks_settings']['m2_hide_built_in_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide built-in breadcrumb'),
    '#default_value' => variable_get('m2_hide_built_in_breadcrumb', false),
  );

/* menu settings */
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu settings'),
  );
  $form['settings']['m2_show_system_menus_for_manage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show system menus in Manage tabs'),
    '#default_value' => variable_get('m2_show_system_menus_for_manage', false),
  );

/* show external attributes */
  $external_attributes = m2_get_external_attributes();
  $form['ext_attr'] = array(
    '#type' => 'fieldset',
    '#title' => t('External Attributes (fields)'),
  );
  if (count($external_attributes)) {
    foreach ($external_attributes as $module_name => $c_schema) {
      $form['ext_attr'][$module_name]['title'] = array(
        '#markup' => theme('m2_title', array('type' => 'h3', 'markup' => t('Module <strong>!name</strong>', array('!name' => $module_name)))),
      );
      foreach ($c_schema as $table_name => $table_info) {
     /* show fields */
        $header_fields = array(
          array('class' => array('name'), 'data' => t('name')),
          array('class' => array('type'), 'data' => t('type')),
          array('class' => array('leng'), 'data' => t('length')),
          array('class' => array('null'), 'data' => t('not null')),
          array('class' => array('defl'), 'data' => t('def. val.')),
          array('class' => array('desc'), 'data' => t('description')),
          array('class' => array('inst'), 'data' => t('installed')),
        );
        $rows = array();
        foreach ($table_info['fields'] as $field_name => $field_info) {
          $is_installed = db_field_exists($table_name, $field_name);
          $rows[]= array('data' => array(
            array('class' => array('name'), 'data' => $field_name),
            array('class' => array('type'), 'data' => $field_info['type']),
            array('class' => array('leng'), 'data' => isset($field_info['length']) ? $field_info['length'] : t('default')),
            array('class' => array('null'), 'data' => $field_info['not null'] ? 'true' : 'false'),
            array('class' => array('defl'), 'data' => $field_info['default'] === null ? 'null' : ('"'.$field_info['default'].'"')),
            array('class' => array('desc'), 'data' => $field_info['description']),
            array('class' => array('inst'), 'data' => $is_installed ? t('yes') : t('no')),
          ));
        }
        $form['ext_attr'][$module_name][$table_name]['title_table'] = array(
          '#markup' => theme('m2_title', array('type' => 'h4', 'markup' => t('Table <strong>!name</strong>', array('!name' => $table_name)))),
        );
        $form['ext_attr'][$module_name][$table_name]['title_fields'] = array(
          '#markup' => theme('m2_title', array('type' => 'div', 'class' => array('title-fields'), 'markup' => t('Fields'))),
        );
        $form['ext_attr'][$module_name][$table_name]['fields'] = array(
          '#theme' => 'table',
          '#header' => $header_fields,
          '#rows' => $rows,
          '#empty' => t('No External Attributes.'),
          '#attributes' => array('class' => array('external-attributes')),
        );
     /* show unique keys */
        $header_indexes = array(
          array('class' => array('name'), 'data' => t('name')),
          array('class' => array('flds'), 'data' => t('fields')),
        );
        $rows = array();
        if (isset($table_info['unique keys'])) {
          foreach ($table_info['unique keys'] as $key_name => $key_fields) {
            $rows[]= array('data' => array(
              array('class' => array('name'), 'data' => $key_name),
              array('class' => array('flds'), 'data' => implode(', ', $key_fields)),
            ));
          }
        }
        $form['ext_attr'][$module_name][$table_name]['title_unique_keys'] = array(
          '#markup' => theme('m2_title', array('type' => 'div', 'class' => array('title-indexes'), 'markup' => t('Unique Keys'))),
        );
        $form['ext_attr'][$module_name][$table_name]['unique_keys'] = array(
          '#theme' => 'table',
          '#header' => $header_indexes,
          '#rows' => $rows,
          '#empty' => t('No Unique Keys.'),
          '#attributes' => array('class' => array('indexes')),
        );
     /* show indexes */
        $rows = array();
        if (isset($table_info['indexes'])) {
          foreach ($table_info['indexes'] as $idx_name => $idx_fields) {
            $rows[]= array('data' => array(
              array('class' => array('name'), 'data' => $idx_name),
              array('class' => array('flds'), 'data' => implode(', ', $idx_fields)),
            ));
          }
        }
        $form['ext_attr'][$module_name][$table_name]['title_indexes'] = array(
          '#markup' => theme('m2_title', array('type' => 'div', 'class' => array('title-indexes'), 'markup' => t('Indexes'))),
        );
        $form['ext_attr'][$module_name][$table_name]['indexes'] = array(
          '#theme' => 'table',
          '#header' => $header_indexes,
          '#rows' => $rows,
          '#empty' => t('No Indexes.'),
          '#attributes' => array('class' => array('indexes')),
        );
      }
    }
  } else {
    $form['ext_attr']['no_attr_message'] = array(
     '#markup' => theme('m2_content_wrapper', array('class' => array('no-items'), 'markup' => t('No External Attributes.'))),
    );
  }
  return system_settings_form($form);
}


/**
 * m2_get_mysql_tables_engine | return array(N) or null
 * m2_set_transaction_warning | return null
 */

function m2_get_mysql_tables_engine() {
  $conn = Database::getConnectionInfo();
  if (isset($conn['default']['driver']) && $conn['default']['driver'] == 'mysql') {
    $return = array();
    $prefix = isset($conn['default']['prefix']['default']) ? $conn['default']['prefix']['default'] : '';
    $result = db_query('SELECT TABLE_NAME as name, ENGINE as engine FROM information_schema.TABLES WHERE TABLE_SCHEMA = :name', array(':name' => $conn['default']['database']));
    foreach ($result->fetchAll() as $table) {
      $return[substr($table->name, strlen($prefix))] = $table->engine;
    }
    return $return;
  }
}


function m2_set_transaction_warning() {
  $engine_info = m2_get_mysql_tables_engine();
  if ($engine_info) {
    if (isset($engine_info['m2'])       && $engine_info['m2'] != 'InnoDB')       drupal_set_message(t('Table %table_name in database use engine %engine_cur but engine %engine_rec give more stability!', array('%table_name' => 'm2',       '%engine_cur' => $engine_info['m2'],       '%engine_rec' => 'InnoDB')), 'warning');
    if (isset($engine_info['m2_items']) && $engine_info['m2_items'] != 'InnoDB') drupal_set_message(t('Table %table_name in database use engine %engine_cur but engine %engine_rec give more stability!', array('%table_name' => 'm2_items', '%engine_cur' => $engine_info['m2_items'], '%engine_rec' => 'InnoDB')), 'warning');
  }
}

