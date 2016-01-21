<?php

/**
 * @file
 * Implementations of Xtools hooks.
 */

/**
 * Implements hook_xtools_blueprint_info().
 */
function xtools_xtools_blueprint_info() {
  // hook_aggregator_fetch_info() implementation return value.
  $blueprints['hook_aggregator_fetch_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintString,
  ), 'integer');

  // hook_aggregator_parse_info() implementation return value.
  $blueprints['hook_aggregator_parse_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintString,
  ), 'integer');

  // hook_admin_paths() implementation return value.
  $blueprints['hook_admin_paths'] = new XtoolsBlueprintArray(array(), array(new XtoolsBlueprintBoolean), 'string');

  // hook_block_info() implementation return value.
  $blueprints['hook_block_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'cache' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
      'info' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'pages' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'properties' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
        'administrative' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
      ))),
      'region' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'status' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
      'visibility' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
      'weight' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
    ), array(), 'string'),
  ), 'string');

  // hook_cron_queue_info() implementation return value.
  $blueprints['hook_cron_queue_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'worker callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'time' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
    ), array(), 'string'),
  ), 'string');

  // hook_dashboard_regions() implementation return value.
  $blueprints['hook_dashboard_regions'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintString
  ), 'string');

  // hook_element_info() implementation return value.
  $blueprints['hook_element_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintPlaceholder('renderable_array'),
  ), 'string');

  // hook_entity_info() implementation return value.
  $blueprints['hook_entity_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'base table' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'bundle keys' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
        'bundle' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      ), array(), 'string')),
      'bundles' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
        new XtoolsBlueprintArray(array(
          'admin' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
            'access arguments' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'integer')),
            'access callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
            'bundle argument' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
            'path' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
            'real path' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          ), array(), 'string')),
          'label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
          'uri callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        ), array(), 'string'),
      ), 'string')),
      'controller class' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'entity keys' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
        'bundle' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'id' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'revision' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      ), array(), 'string')),
      'fieldable' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
      'label callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'load hook' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'revision table' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'static cache' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
      'translation' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'string')),
      'uri callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'view modes' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
        new XtoolsBlueprintArray(array(
          'label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
          'custom settings' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean, TRUE),
        ), array(), 'string'),
      ), 'string')),
    )),
  ), 'string');

  // hook_field_extra_fields() implementation return value.
  $field_blueprint = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'weight' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
    ), array(), 'string'),
  ), 'string');
  // The entity type.
  $blueprints['hook_field_extra_fields'] = new XtoolsBlueprintArray(array(), array(
    // The bundle.
    new XtoolsBlueprintArray(array(), array(
      // "form"/"display".
      new XtoolsBlueprintArray(array(
        'display' => new XtoolsBlueprintChildElement($field_blueprint),
        'form' => new XtoolsBlueprintChildElement($field_blueprint),
      )),
    ), 'string'),
  ), 'string');

  // hook_field_info() implementation return value.
  $blueprints['hook_field_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'default_formatter' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'default_widget' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'instance_settings' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'string')),
      'label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'no_ui' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
      'settings' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'string')),
    ), array(), 'string'),
  ), 'string');

  // hook_field_formatter_info() implementation return value.
  $blueprints['hook_field_formatter_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'field types' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'integer')),
      'label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'settings' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'string')),
    ), array(), 'string'),
  ), 'string');

  // hook_field_storage_info() implementation return value.
  $blueprints['hook_field_storage_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'settings' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'string')),
    ), array(), 'string'),
  ), 'string');

  // hook_field_widget_info() implementation return value.
  $blueprints['hook_field_widget_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'behaviors' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
        'default value' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array(FIELD_BEHAVIOR_DEFAULT, FIELD_BEHAVIOR_NONE))),
        'multiple values' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array(FIELD_BEHAVIOR_CUSTOM, FIELD_BEHAVIOR_DEFAULT))),
      ), array(), 'string')),
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'field types' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'integer')),
      'label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'settings' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'string')),
    ), array(), 'string'),
  ), 'string');

  // hook_forms() implementation return value.
  $blueprints['hook_forms'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'callback arguments' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'integer')),
      'wrapper callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
    ), array(), 'string'),
  ), 'string');

  // hook_flush_caches() implementation return value.
  $blueprints['hook_flush_caches'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintString,
  ), 'integer');

  // hook_hook_info() implementation return value.
  $blueprints['hook_hook_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'group' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
    ), array(), 'string'),
  ), 'string');

  // hook_image_toolkits() implementation return value.
  $blueprints['hook_image_toolkits'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'available' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean, TRUE),
    ), array(), 'string'),
  ), 'string');

  // hook_library() implementation return value.
  $blueprints['hook_library'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'css' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
        new XtoolsBlueprintArray(array(
          'basename' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'browsers' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
            new XtoolsBlueprintBoolean,
            new XtoolsBlueprintString,
          ), 'string')),
          'every_page' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
          'group' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array(CSS_SYSTEM, CSS_DEFAULT, CSS_THEME))),
          'media' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array('file', 'inline', 'external'))),
          'weight' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
        ), array(), 'string'),
      ), 'string')),
      'js' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
        new XtoolsBlueprintString,
        new XtoolsBlueprintArray(array(
          'cache' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
          'defer' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
          'every_page' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
          'group' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array(JS_LIBRARY, JS_DEFAULT, JS_THEME))),
          'preprocess' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
          'scope' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array('file', 'inline', 'external', 'setting'))),
          'weight' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
        ), array(), 'string'),
      ))),
      'dependencies' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
        new XtoolsBlueprintArray(array(), array(
          new XtoolsBlueprintString,
        ), 'integer'),
      ), 'integer')),
      'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'version' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'website' => new XtoolsBlueprintChildElement(new XtoolsBlueprintURL),
    ), array(), 'string'),
  ), 'string');

  // hook_menu() implementation return value.
  $blueprints['hook_menu'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'access arguments' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray),
      'access callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOr(array(
        new XtoolsBlueprintBoolean,
        new XtoolsBlueprintString,
      ))),
      'delivery callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'file' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'file path' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'load arguments' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray),
      'menu_name' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'options' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
        'attributes' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'string')),
        'html' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
        'language' => new XtoolsBlueprintChildElement(new XtoolsBlueprintObject),
      ), array(), 'string')),
      'page arguments' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray),
      'page callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'position' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array('left', 'right'))),
      'tab_parent' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'tab_root' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'theme arguments' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray),
      'theme callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'title arguments' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray),
      'title callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'weight' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
    ), array(), 'string'),
  ), 'string');

  // hook_node_info() implementation return value.
  $blueprints['hook_node_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'base' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'has_title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
      'help' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'locked' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
      'name' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'title_label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
    ), array(), 'string'),
  ), 'string');

  // hook_node_operations() implementation return value.
  $blueprints['hook_node_operations'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'callback arguments' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray),
    ), array(), 'string'),
  ), 'string');

  // hook_ranking() implementation return value.
  $blueprints['hook_ranking'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'arguments' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray),
      'join' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'score' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
    ), array(), 'string'),
  ), 'string');

  // hook_permission() implementation return value.
  $blueprints['hook_permission'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'restrict access' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
      'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'warning' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
    ), array(), 'string'),
  ), 'string');

  // hook_requirements() return value.
  $blueprints['hook_requirements'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'severity' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
      'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'value' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
    ), array(), 'string'),
  ), 'string');

  // hook_schema() implementation return value.
  $key_column_specifiers = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintString,
    new XtoolsBlueprintArray(array(
      0 => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      1 => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
    ), array(), 'integer'),
  ), 'integer');
  $blueprints['hook_schema'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'fields' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
        new XtoolsBlueprintArray(array(
          'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'length' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
          'not null' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
          'precision' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
          'scale' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
          'serialize' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
          'size' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
          'unsigned' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
        ), array(), 'string'),
      ), 'string'), TRUE),
      'foreign keys' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
        new XtoolsBlueprintArray(array(
          'table' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
          'columns' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
            new XtoolsBlueprintString,
          ), 'string'), TRUE),
        ), array(), 'string'),
      ), 'string')),
      'indexes' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array($key_column_specifiers), 'string')),
      'mysql_suffix' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'primary key' => new XtoolsBlueprintChildElement($key_column_specifiers),
      'unique keys' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array($key_column_specifiers), 'string')),
    ), array(), 'string'),
  ), 'string');

  // hook_stream_wrappers() return value.
  $blueprints['hook_stream_wrappers'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'class' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'name' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      'type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
    ), array(), 'string'),
  ), 'string');

  // hook_system_theme_info() return value.
  $blueprints['hook_system_theme_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintString,
  ), 'string');

  // hook_update_dependencies() return value.
  $blueprints['hook_update_dependencies'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintArray(array(), array(
        new XtoolsBlueprintInteger,
      ), 'string'),
    ), 'integer'),
  ), 'string');

  // hook_update_last_removed() return value.
  $blueprints['hook_update_last_removed'] = new XtoolsBlueprintInteger;

  // hook_user_categories() return value.
  $blueprints['hook_user_categories'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'access arguments' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'integer')),
      'access callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'name' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'weight' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
    ), array(), 'string'),
  ), 'integer');

  // hook_views_api() return value.
  $blueprints['hook_views_api'] = new XtoolsBlueprintArray(array(
    'api' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
    'path' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
    'template path' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
  ), array(), 'string');

  // hook_views_data() return value.
  $blueprints['hook_views_data'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'moved to' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      // Table definition.
      'table' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
        'base' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
          'database' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'field' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'help' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'weight' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger),
        ), array(), 'string')),
        'group' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'help' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'join' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
          'extra' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOr(array(
            new XtoolsBlueprintString,
            new XtoolsBlueprintArray(array(), array(
              new XtoolsBlueprintArray(array(
                'field' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
                'numeric' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
                'operator' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
              ), array(), 'string'),
            ), 'integer'),
          ))),
          'extra type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array('AND', 'OR'))),
          'field' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'handler' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'left_field' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'left_table' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'table' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array('INNER', 'LEFT'))),
        ), array(), 'string')),
        'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      ), array(), 'string')),
    ), array(
      // Column definitions.
      new XtoolsBlueprintArray(array(
        'argument' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
          'handler' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        ), array(), 'string')),
        'field' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
          'handler' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
          'moved to' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
            new XtoolsBlueprintString,
          ), 'integer')),
        ), array(), 'string')),
        'filter' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
          'handler' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        ), array(), 'string')),
        'help' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'moved to' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
          new XtoolsBlueprintString,
        ), 'integer')),
        'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'relationship' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
          'handler' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        ), array(), 'string')),
        'real field' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'sort' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
          'handler' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        ), array(), 'string')),
      )),
    ), 'string'),
  ));

  // hook_views_plugins() return value.
  $blueprints['hook_views_plugins'] = new XtoolsBlueprintArray(array(
    // @todo The "module" property isn't documented anywhere, but used by
    // Views itself.
    'module' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
  ), array(
    new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintArray(array(
        'base' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
          new XtoolsBlueprintString,
        ), 'integer')),
        'handler' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'help' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'parent' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'path' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'theme' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
        'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      ), array(), 'string'),
    ), 'string'),
  ), 'string');

  // hook_views_default_views() return value.
  $blueprints['hook_views_default_views'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintChildElement(new XtoolsBlueprintObject('view')),
  ), 'string');

  // hook_webform_select_options_info() return value.
  $blueprints['hook_webform_select_options_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'file' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
      'options arguments' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'integer')),
      'options callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString(), TRUE),
      'title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString(), TRUE),
    ), array(), 'string'),
  ), 'string');

  // hook_webform_component_info() return value.
  $blueprints['hook_webform_component_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintArray(array(
      'description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString(), TRUE),
      'features' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'string')),
      'file' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
      'label' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString(), TRUE),
    ), array(), 'string'),
  ), 'string');
  $features  = array('attachment', 'conditional', 'csv', 'default_value', 'description', 'email', 'email_address', 'email_name', 'group', 'required', 'spam_analysis', 'title', 'title_display', 'title_inline');
  foreach ($features as $feature) {
    $blueprints['hook_webform_component_info']->or->allowed_blueprints[0]->blueprints_named['features']->child_blueprint->blueprints_named[$feature] = new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean());
  }

  // hook_xmlrpc() return value.
  $blueprints['hook_xmlrpc'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintString,
    new XtoolsBlueprintArray(array(
      0 => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      1 => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
      2 => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
        new XtoolsBlueprintOrValue(array('array', 'base64', 'boolean', 'date', 'double', 'int', 'string', 'struct')),
      )), TRUE),
      3 => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
    ), array(), 'integer'),
  ));

  // hook_xtools_callable_type_info() return value.
  $blueprints['hook_xtools_callable_type_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintPlaceholder('XtoolsCallableType'),
  ), 'integer');

  // hook_xtools_blueprint_info() return value.
  $blueprints['hook_xtools_blueprint_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintPlaceholder('XtoolsBlueprint'),
  ), 'string');

  // Class XtoolsBlueprint.
  // Modules that want to add their own blueprint types should take care to
  // keep the "XtoolsBlueprintInterface" as the last item in the array.
  $blueprints['XtoolsBlueprint'] = new XtoolsBlueprintOr(array(
    // Test a few specific blueprint types first.
    new XtoolsBlueprintPlaceholder('XtoolsBlueprintOr'),
    new XtoolsBlueprintPlaceholder('XtoolsBlueprintOrValue'),
    new XtoolsBlueprintPlaceholder('XtoolsBlueprintArray'),
    new XtoolsBlueprintPlaceholder('XtoolsBlueprintObject'),
    new XtoolsBlueprintPlaceholder('XtoolsBlueprintPrimitive'),
    // At the very least, blueprints should implement XtoolsBlueprintInterface.
    new XtoolsBlueprintObject('XtoolsBlueprintInterface'),
  ));

  // Class XtoolsBlueprintArray.
  $blueprints['XtoolsBlueprintArray'] = new XtoolsBlueprintObject('XtoolsBlueprintArray', array(
    'blueprints_named' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintPlaceholder('XtoolsBlueprintChildElement'),
    ))),
    'key_type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array('integer', 'string', NULL)), TRUE),
    'or' => new XtoolsBlueprintChildElement(new XtoolsBlueprintPlaceholder('XtoolsBlueprintOr')),
  ));

  // Class XtoolsBlueprintChildElement.
  $blueprints['XtoolsBlueprintChildElement'] = new XtoolsBlueprintObject('XtoolsBlueprintChildElement', array(
    'child_blueprint' => new XtoolsBlueprintChildElement(new XtoolsBlueprintPlaceholder('XtoolsBlueprint'), TRUE),
    'required' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
  ));

  // Class XtoolsBlueprintObject.
  $blueprints['XtoolsBlueprintObject'] = new XtoolsBlueprintObject('XtoolsBlueprintObject', array(
    'instanceof' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
    'object_properties' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintChildElement(new XtoolsBlueprintPlaceholder('XtoolsBlueprintChildElement')),
    ), 'string'), TRUE),
  ));

  // Class XtoolsBlueprintOr.
  $blueprints['XtoolsBlueprintOr'] = new XtoolsBlueprintObject('XtoolsBlueprintOr', array(
    'allowed_blueprints' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintChildElement(new XtoolsBlueprintPlaceholder('XtoolsBlueprint')),
    ), 'integer'), TRUE),
  ));

  // Class XtoolsBlueprintOrValue.
  $blueprints['XtoolsBlueprintOrValue'] = new XtoolsBlueprintObject('XtoolsBlueprintOrValue', array(
    'allowed_values' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintChildElement(new XtoolsBlueprintScalar),
      new XtoolsBlueprintChildElement(new XtoolsBlueprintNULL),
    ), 'integer'), TRUE),
  ));

  // Class XtoolsBlueprintPrimitive.
  $blueprints['XtoolsBlueprintPrimitive'] = new XtoolsBlueprintObject('XtoolsBlueprintPrimitive', array(
    'type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString, TRUE),
  ));

  // Class XtoolsCallableType.
  $blueprints['XtoolsCallableType'] = new XtoolsBlueprintObject('XtoolsCallableType', array(
    'blueprint' => new XtoolsBlueprintChildElement(new XtoolsBlueprintPlaceholder('XtoolsBlueprint')),
    'module' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
    'signature' => new XtoolsBlueprintChildElement(new XtoolsBlueprintPlaceholder('XtoolsSignature')),
  ));

  // Class XtoolsSignature.
  $blueprints['XtoolsSignature'] = new XtoolsBlueprintObject('XtoolsSignature', array(
    'parameters' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintPlaceholder('XtoolsSignatureParameter'),
    ), 'integer'), TRUE),
  ));

  // Class XtoolsSignatureParameter.
  $blueprints['XtoolsSignatureParameter'] = new XtoolsBlueprintObject('XtoolsSignatureParameter', array(
    'definition' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
      'name' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'reference' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
      'type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
    ), array(), 'string'), TRUE),
  ));

  // hook_xtools_signature_info() return value.
  $blueprints['hook_xtools_signature_info'] = new XtoolsBlueprintArray(array(), array(
    new XtoolsBlueprintObject('XtoolsSignature'),
  ), 'string');

  // Renderable arrays.
  // @todo Describe #attached and #options.
  $blueprints['renderable_array'] = new XtoolsBlueprintArray(array(
    '#access' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
    '#action' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
    '#after_build' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintString,
    ), 'integer')),
    '#ajax' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
      'callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'effect' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'event' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'keypress' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean),
      'method' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array('after', 'append', 'before', 'prepend', 'replace'))),
      'path' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'prevent' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
      'progress' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
        'interval' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
        'message' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
        'type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array('bar', 'throbber'))),
        'url' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
      ), array(), 'string')),
      'trigger_as' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(
        'name' => new XtoolsblueprintChildElement(new XtoolsBlueprintString),
        'value' => new XtoolsblueprintChildElement(new XtoolsBlueprintString),
      ), array(), 'string')),
      'wrapper' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString),
    ), array(), 'string')),
    '#array_parents' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(new XtoolsBlueprintString()), 'integer')),
    '#attributes' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(new XtoolsBlueprintString()), 'integer')),
    '#autocomplete_path' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#built' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#button_type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#children' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#collapsed' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#collapsible' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#cols' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger(0)),
    '#default_tab' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#delta' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger(0)),
    '#description' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#disabled' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#element_validate' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintString,
    ), 'integer')),
    '#empty' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#empty_option' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#error' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#executes_submit_callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#field_prefix' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#field_suffix' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#group' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#header' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintString(),
    ), 'string')),
    '#id' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#input' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#js_select' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#limit_validation_errors' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray()),
    '#markup' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#maxlength' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger(0)),
    '#method' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array('get', 'post'))),
    '#multiple' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#name' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#parents' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintString,
    ), 'integer')),
    '#post_render' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintString,
    ), 'integer')),
    '#prefix' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#pre_render' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintString,
    ), 'integer')),
    '#printed' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#process' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintString,
    ), 'integer')),
    '#processed' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#required' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#resizable' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#rows' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger(0)),
    '#size' => new XtoolsBlueprintChildElement(new XtoolsBlueprintInteger(0)),
    '#src' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#states' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintArray(array(), array(
        new XtoolsBlueprintArray(array(), array(), 'string'),
      ), 'string'),
    ), 'string')),
    '#submit' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintString,
    ), 'integer')),
    '#suffix' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#theme' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOr(array(
      new XtoolsBlueprintArray(array(), array(
        new XtoolsBlueprintString(),
      ), 'integer'),
      new XtoolsBlueprintString(),
    ))),
    '#theme_wrappers' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(), 'integer')),
    '#title' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#tree' => new XtoolsBlueprintChildElement(new XtoolsBlueprintBoolean()),
    '#type' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#title_display' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOrValue(array('before', 'after', 'attribute', 'invisible', 'none'))),
    '#validate' => new XtoolsBlueprintChildElement(new XtoolsBlueprintArray(array(), array(
      new XtoolsBlueprintString,
    ), 'integer')),
    '#value_callback' => new XtoolsBlueprintChildElement(new XtoolsBlueprintString()),
    '#weight' => new XtoolsBlueprintChildElement(new XtoolsBlueprintOr(array(new XtoolsBlueprintFloat(), new XtoolsBlueprintInteger()))),
  ), array(), 'string');

  return $blueprints;
}

/**
 * Implements hook_xtools_callable_type_info().
 */
function xtools_xtools_callable_type_info() {
  return array(
    // hook_aggregator_fetch().
    new XtoolsCallableTypeHook('aggregator_fetch', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_aggregator_fetch'),
      'module' => 'aggregator',
    )),

    // hook_aggregator_fetch_info().
    new XtoolsCallableTypeHook('aggregator_fetch_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_aggregator_fetch_info'),
      'module' => 'aggregator',
    )),

    // hook_aggregator_parse().
    new XtoolsCallableTypeHook('aggregator_parse', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_aggregator_parse'),
      'module' => 'aggregator',
    )),

    // hook_aggregator_parse_info().
    new XtoolsCallableTypeHook('aggregator_parse_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_aggregator_parse_info'),
      'module' => 'aggregator',
    )),

    // hook_aggregator_process().
    new XtoolsCallableTypeHook('aggregator_process', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_aggregator_process'),
      'module' => 'aggregator',
    )),

    // hook_aggregator_process_info().
    new XtoolsCallableTypeHook('aggregator_process_info', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_aggregator_process_info'),
      'module' => 'aggregator',
    )),

    // hook_aggregator_remove().
    new XtoolsCallableTypeHook('aggregator_remove', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_aggregator_remove'),
      'module' => 'aggregator',
    )),

    // hook_block_configure().
    new XtoolsCallableTypeHook('block_configure', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_block_configure'),
      'module' => 'block',
    )),

    // hook_block_info().
    new XtoolsCallableTypeHook('block_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_block_info'),
      'module' => 'block',
    )),

    // hook_block_info_alter().
    new XtoolsCallableTypeHook('block_info_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_block_info_alter'),
      'module' => 'block',
    )),

    // hook_block_list_alter().
    new XtoolsCallableTypeHook('block_list_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_block_list_alter'),
      'module' => 'block',
    )),

    // hook_block_save().
    new XtoolsCallableTypeHook('block_save', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_block_save'),
      'module' => 'block',
    )),

    // hook_block_view().
    new XtoolsCallableTypeHook('block_view', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_block_view'),
      'module' => 'block',
    )),

    // hook_block_view_alter().
    new XtoolsCallableTypeHook('block_view_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_block_view_alter'),
      'module' => 'block',
    )),

    // hook_comment_delete().
    new XtoolsCallableTypeHook('comment_delete', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_comment_delete'),
      'module' => 'comment',
    )),

    // hook_comment_insert().
    new XtoolsCallableTypeHook('comment_insert', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_comment_insert'),
      'module' => 'comment',
    )),

    // hook_comment_load().
    new XtoolsCallableTypeHook('comment_load', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_comment_load'),
      'module' => 'comment',
    )),

    // hook_comment_presave().
    new XtoolsCallableTypeHook('comment_presave', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_comment_presave'),
      'module' => 'comment',
    )),

    // hook_comment_publish().
    new XtoolsCallableTypeHook('comment_publish', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_comment_publish'),
      'module' => 'comment',
    )),

    // hook_comment_unpublish().
    new XtoolsCallableTypeHook('comment_unpublish', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_comment_unpublish'),
      'module' => 'comment',
    )),
    // hook_comment_update().
    new XtoolsCallableTypeHook('comment_update', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_comment_update'),
      'module' => 'comment',
    )),
    // hook_comment_view().
    new XtoolsCallableTypeHook('comment_view', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_comment_view'),
      'module' => 'comment',
    )),
    // hook_comment_view_alter().
    new XtoolsCallableTypeHook('comment_view_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_comment_view_alter'),
      'module' => 'comment',
    )),

    // hook_contextual_links_view_alter().
    new XtoolsCallableTypeHook('contextual_links_view_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_contextual_links_view_alter'),
      'module' => 'contextual',
    )),

    // hook_dashboard_regions().
    new XtoolsCallableTypeHook('dashboard_regions', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_dashboard_regions'),
      'module' => 'dashboard',
    )),

    // hook_dashboard_regions_alter().
    new XtoolsCallableTypeHook('dashboard_regions_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_dashboard_regions_alter'),
      'module' => 'dashboard',
    )),

    // hook_field_extra_fields().
    new XtoolsCallableTypeHook('field_extra_fields', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_field_extra_fields'),
      'module' => 'field',
    )),

    // hook_field_info().
    new XtoolsCallableTypeHook('field_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_field_info'),
      'module' => 'field',
    )),

    // hook_field_formatter_info().
    new XtoolsCallableTypeHook('field_formatter_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_field_formatter_info'),
      'module' => 'field',
    )),

    // hook_field_storage_info().
    new XtoolsCallableTypeHook('field_storage_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_field_storage_info'),
      'module' => 'field',
    )),

    // hook_field_widget_info().
    new XtoolsCallableTypeHook('field_widget_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_field_widget_info'),
      'module' => 'field',
    )),

    // hook_file_download_access().
    new XtoolsCallableTypeHook('file_download_access', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_file_download_access'),
      'module' => 'file',
    )),

    // hook_file_download_access_alter().
    new XtoolsCallableTypeHook('file_download_access_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_file_download_access_alter'),
      'module' => 'file',
    )),

    // hook_node_info().
    new XtoolsCallableTypeHook('node_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_node_info'),
      'module' => 'node',
    )),

    // hook_node_operations().
    new XtoolsCallableTypeHook('node_operations', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_node_operations'),
      'module' => 'node',
    )),

    // hook_ranking().
    new XtoolsCallableTypeHook('ranking', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_ranking'),
      'module' => 'node',
    )),

    // hook_admin_paths().
    new XtoolsCallableTypeHook('admin_paths', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_admin_paths'),
      'module' => 'system',
    )),

    // hook_cron_queue_info().
    new XtoolsCallableTypeHook('cron_queue_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_cron_queue_info'),
      'module' => 'system',
    )),

    // hook_element_info().
    new XtoolsCallableTypeHook('element_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_element_info'),
      'module' => 'system',
    )),

    // hook_element_info() callbacks.
    new XtoolsCallableTypeHookElementInfoCallbacks(array(
      'signature' => new XtoolsSignaturePlaceholder('hook_element_info_callback'),
      'module' => 'system',
    )),

    // hook_entity_info().
    new XtoolsCallableTypeHook('entity_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_entity_info'),
      'module' => 'system',
    )),

    // hook_forms().
    new XtoolsCallableTypeHook('forms', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_forms'),
      'module' => 'system',
    )),

    // hook_flush_caches().
    new XtoolsCallableTypeHook('flush_caches', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_flush_caches'),
      'module' => 'system',
    )),

    // hook_hook_info().
    new XtoolsCallableTypeHook('hook_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_hook_info'),
      'module' => 'system',
    )),

    // hook_image_toolkits().
    new XtoolsCallableTypeHook('image_toolkits', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_image_toolkits'),
      'module' => 'system',
    )),

    // hook_library().
    new XtoolsCallableTypeHook('library', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_library'),
      'module' => 'system',
    )),

    // hook_menu().
    new XtoolsCallableTypeHook('menu', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_menu'),
      'module' => 'system',
    )),

    // hook_permission().
    new XtoolsCallableTypeHook('permission', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_permission'),
      'module' => 'system',
    )),

    // hook_requirements().
    new XtoolsCallableTypeHook('requirements', array(
      'argument_groups' => array(
        array('install'),
        array('update'),
        array('runtime'),
      ),
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_requirements'),
      'signature' => new XtoolsSignaturePlaceholder('hook_requirements'),
      'module' => 'system',
    )),

    // hook_schema().
    new XtoolsCallableTypeHook('schema', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_schema'),
      'module' => 'system',
    )),

    // hook_stream_wrappers().
    new XtoolsCallableTypeHook('stream_wrappers', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_stream_wrappers'),
      'module' => 'system',
    )),

    // hook_system_theme_info().
    new XtoolsCallableTypeHook('system_theme_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_system_theme_info'),
      'module' => 'system',
    )),

    // hook_update_dependencies().
    new XtoolsCallableTypeHook('update_dependencies', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_update_dependencies'),
      'module' => 'system',
    )),

    // hook_update_last_removed().
    new XtoolsCallableTypeHook('update_last_removed', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_update_last_removed'),
      'module' => 'system',
    )),

    // hook_xmlrpc().
    new XtoolsCallableTypeHook('xmlrpc', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_xmlrpc'),
      'module' => 'system',
    )),

    // hook_user_categories().
    new XtoolsCallableTypeHook('user_categories', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_user_categories'),
      'module' => 'user',
    )),

    // hook_views_ajax_data_alter().
    new XtoolsCallableTypeHookViews('views_ajax_data_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_ajax_data_alter'),
      'module' => 'views',
    )),

    // hook_views_api().
    new XtoolsCallableTypeHook('views_api', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_views_api'),
      'module' => 'views',
    )),

    // hook_views_data().
    new XtoolsCallableTypeHookViews('views_data', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_views_data'),
      'module' => 'views',
    )),

    // hook_views_data_alter().
    new XtoolsCallableTypeHookViews('views_data_alter', array(
      'module' => 'views',
      'signature' => new XtoolsSignaturePlaceholder('hook_views_data_alter'),
    )),

    // hook_views_default_views().
    new XtoolsCallableTypeHookViews('views_default_views', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_views_default_views'),
      'module' => 'views',
    )),

    // hook_views_default_views_alter().
    new XtoolsCallableTypeHookViews('views_default_views_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_default_views_alter'),
      'module' => 'views',
    )),

    // hook_views_form_validate().
    new XtoolsCallableTypeHookViews('views_form_validate', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_form_valite'),
      'module' => 'views',
    )),

    // hook_views_form_submit().
    new XtoolsCallableTypeHookViews('views_form_submit', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_form_submit'),
      'module' => 'views',
    )),

    // hook_views_plugins().
    new XtoolsCallableTypeHookViews('views_plugins', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_views_plugins'),
      'module' => 'views',
    )),

    // hook_views_plugins_alter().
    new XtoolsCallableTypeHookViews('views_plugins_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_plugins_alter'),
      'module' => 'views',
    )),

    // hook_views_post_build().
    new XtoolsCallableTypeHookViews('views_post_build', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_post_build'),
      'module' => 'views',
    )),

    // hook_views_post_execute().
    new XtoolsCallableTypeHookViews('views_post_execute', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_pre_view'),
      'module' => 'views',
    )),

    // hook_views_post_render().
    new XtoolsCallableTypeHookViews('views_post_render', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_post_render'),
      'module' => 'views',
    )),

    // hook_views_pre_build().
    new XtoolsCallableTypeHookViews('views_pre_build', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_pre_build'),
      'module' => 'views',
    )),

    // hook_views_pre_execute().
    new XtoolsCallableTypeHookViews('views_pre_execute', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_pre_execute'),
      'module' => 'views',
    )),

    // hook_views_pre_render().
    new XtoolsCallableTypeHookViews('views_pre_render', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_pre_render'),
      'module' => 'views',
    )),

    // hook_views_preview_info_alter().
    new XtoolsCallableTypeHookViews('views_preview_info_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_preview_info_alter'),
      'module' => 'views',
    )),

    // hook_views_pre_view().
    new XtoolsCallableTypeHookViews('views_pre_view', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_pre_view'),
      'module' => 'views',
    )),

    // hook_views_query_alter().
    new XtoolsCallableTypeHookViews('views_query_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_query_alter'),
      'module' => 'views',
    )),

    // hook_views_ui_display_top_links_alter().
    new XtoolsCallableTypeHookViews('views_ui_display_top_links_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_views_ui_display_top_links_alter'),
      'module' => 'views',
    )),

    // hook_webform_select_options_info().
    new XtoolsCallableTypeHook('webform_select_options_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_webform_select_options_info'),
      'module' => 'webform',
    )),

    // hook_webform_select_options_info_alter().
    new XtoolsCallableTypeHook('webform_select_options_info_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_webform_select_options_info_alter'),
      'module' => 'webform',
    )),

    // hook_webform_component_presave().
    new XtoolsCallableTypeHook('webform_component_presave', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_webform_component_presave'),
      'module' => 'webform',
    )),

    // hook_webform_component_insert().
    new XtoolsCallableTypeHook('webform_component_insert', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_webform_component_insert'),
      'module' => 'webform',
    )),

    // hook_webform_component_update().
    new XtoolsCallableTypeHook('webform_component_update', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_webform_component_update'),
      'module' => 'webform',
    )),

    // hook_webform_component_delete().
    new XtoolsCallableTypeHook('webform_component_delete', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_webform_component_delete'),
      'module' => 'webform',
    )),

    // hook_webform_component_info().
    new XtoolsCallableTypeHook('webform_component_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_webform_component_info'),
      'module' => 'webform',
    )),

    // hook_webform_component_info_alter().
    new XtoolsCallableTypeHook('webform_component_info_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_webform_component_info_alter'),
      'module' => 'webform',
    )),

    // hook_xtools_blueprint_info().
    new XtoolsCallableTypeHook('xtools_blueprint_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_xtools_blueprint_info'),
    )),

    // hook_xtools_blueprint_info_alter().
    new XtoolsCallableTypeHook('xtools_blueprint_info_alter', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_xtools_blueprint_info_alter'),
    )),

    // hook_xtools_callable_type_info().
    new XtoolsCallableTypeHook('xtools_callable_type_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_xtools_callable_type_info'),
    )),

    // hook_xtools_signature_info().
    new XtoolsCallableTypeHook('xtools_signature_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_xtools_signature_info'),
    )),
  );
}

/**
 * Implements hook_xtools_signature_info().
 */
function xtools_xtools_signature_info() {
  // hook_aggregator_fetch().
  $signatures['hook_aggregator_fetch'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'feed',
    )),
  ));

  // hook_aggregator_parse().
  $signatures['hook_aggregator_parse'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'feed',
    )),
  ));

  // hook_aggregator_process().
  $signatures['hook_aggregator_process'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'feed',
    )),
  ));

  // hook_aggregator_process_info().
  $signatures['hook_aggregator_process_info'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'feed',
    )),
  ));

  // hook_aggregator_remove().
  $signatures['hook_aggregator_remove'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'feed',
    )),
  ));

  // hook_block_configure().
  $signatures['hook_block_configure'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'delta',
      'default_value' => '',
    )),
  ));

  // hook_block_info_alter().
  $signatures['hook_block_info_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'blocks',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'theme',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'code_blocks',
    )),
  ));

  // hook_block_list_alter().
  $signatures['hook_block_list_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'blocks',
      'reference' => TRUE,
    )),
  ));

  // hook_block_save().
  $signatures['hook_block_save'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'delta',
      'default_value' => '',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'edit',
      'default_value' => array(),
    )),
  ));

  // hook_block_view().
  $signatures['hook_block_view'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'delta',
      'default_value' => '',
    )),
  ));

  // hook_block_view_alter().
  $signatures['hook_block_view_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'data',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'block',
    )),
  ));

  // hook_element_info() callbacks.
  $signatures['hook_element_info_callback'] = new XtoolsSignature(array(
    // @todo Both parameters should be arrays and $form_state should be
    // passed on by reference. However, this is not defined in the
    // hook_element_info() documentation.
    new XtoolsSignatureParameter(array(
      'name' => 'element',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'form_state',
    )),
  ));

  // hook_comment_delete().
  $signatures['hook_comment_delete'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'comment',
    )),
  ));

  // hook_comment_insert().
  $signatures['hook_comment_insert'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'comment',
    )),
  ));

  // hook_comment_load().
  $signatures['hook_comment_load'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'comments',
    )),
  ));

  // hook_comment_presave().
  $signatures['hook_comment_presave'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'comment',
    )),
  ));

  // hook_comment_publish().
  $signatures['hook_comment_publish'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'comment',
    )),
  ));

  // hook_comment_unpublish().
  $signatures['hook_comment_unpublish'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'comment',
    )),
  ));

  // hook_comment_update().
  $signatures['hook_comment_update'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'comment',
    )),
  ));

  // hook_comment_view().
  $signatures['hook_comment_view'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'comment',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'view_mode',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'langcode',
    )),
  ));

  // hook_comment_view_alter().
  $signatures['hook_comment_view_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'build',
      'reference' => TRUE,
    )),
  ));

  // hook_contextual_links_view_alter().
  $signatures['hook_contextual_links_view_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'element',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'items',
    )),
  ));

  // hook_dashboard_regions_alter().
  // @todo The hook documentation doesn't mention the parameter should be
  // passed on by reference, but it looks like it should.
  $signatures['hook_dashboard_regions_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'regions',
    )),
  ));

  // hook_file_download_access().
  $signatures['hook_file_download_access'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'file_item',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'entity_type',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'entity',
    )),
  ));

  // hook_file_download_access_alter().
  $signatures['hook_file_download_access_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'grants',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'file_item',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'entity_type',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'entity',
    )),
  ));

  // hook_requirements().
  $signatures['hook_requirements'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'phase',
    )),
  ));

  // hook_views_ajax_data_alter().
  $signatures['hook_views_ajax_data_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'commands',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'view',
    )),
  ));

  // hook_views_data_alter().
  $signatures['hook_views_data_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'data',
      'reference' => TRUE,
    )),
  ));

  // hook_views_default_views_alter().
  $signatures['hook_views_default_views_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'views',
      'reference' => TRUE,
    )),
  ));

  // hook_views_form_validate().
  $signatures['hook_views_form_validate'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'form',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'form_state',
      'reference' => TRUE,
    )),
  ));

  // hook_views_form_submit().
  $signatures['hook_views_form_submit'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'form',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'form_state',
      'reference' => TRUE,
    )),
  ));

  // hook_views_post_build().
  $signatures['hook_views_post_build'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'view',
      'reference' => TRUE,
    )),
  ));

  // hook_views_post_execute().
  $signatures['hook_views_post_execute'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'view',
      'reference' => TRUE,
    )),
  ));

  // hook_views_post_render().
  $signatures['hook_views_post_render'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'view',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'output',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'cache',
      'reference' => TRUE,
    )),
  ));

  // hook_views_preview_info_alter().
  $signatures['hook_views_preview_info_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'rows',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'view',
    )),
  ));

  // hook_views_pre_build().
  $signatures['hook_views_pre_build'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'view',
      'reference' => TRUE,
    )),
  ));

  // hook_views_pre_execute().
  $signatures['hook_views_pre_execute'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'view',
      'reference' => TRUE,
    )),
  ));

  // hook_views_pre_render().
  $signatures['hook_views_pre_render'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'view',
      'reference' => TRUE,
    )),
  ));

  // hook_views_pre_view().
  $signatures['hook_views_pre_view'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'view',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'display_id',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'args',
      'reference' => TRUE,
    )),
  ));

  // hook_views_plugins_alter().
  $signatures['hook_views_plugins_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'plugins',
      'reference' => TRUE,
    )),
  ));

  // hook_views_query_alter().
  $signatures['hook_views_query_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'view',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'query',
      'reference' => TRUE,
    )),
  ));

  // hook_views_ui_display_top_links_alter().
  $signatures['hook_views_ui_display_top_links_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'links',
      'reference' => TRUE,
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'view',
    )),
    new XtoolsSignatureParameter(array(
      'name' => 'display_id',
    )),
  ));

  // hook_webform_component_delete().
  $signatures['hook_webform_component_delete'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'component',
    )),
  ));

  // hook_webform_component_info_alter().
  $signatures['hook_webform_component_info_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'components',
      'reference' => TRUE,
    )),
  ));

  // hook_webform_component_insert().
  $signatures['hook_webform_component_insert'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'component',
    )),
  ));

  // hook_webform_component_presave().
  $signatures['hook_webform_component_presave'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'component',
      'reference' => TRUE,
    )),
  ));

  // hook_webform_component_update().
  $signatures['hook_webform_component_update'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'component',
    )),
  ));

  // hook_webform_select_options_info_alter().
  $signatures['hook_webform_select_options_info_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'items',
      'reference' => TRUE,
    )),
  ));

  // hook_xtools_blueprint_info_alter().
  $signatures['hook_xtools_blueprint_info_alter'] = new XtoolsSignature(array(
    new XtoolsSignatureParameter(array(
      'name' => 'blueprints',
      'reference' => TRUE,
      'type' => 'array',
    )),
  ));

  return $signatures;
}