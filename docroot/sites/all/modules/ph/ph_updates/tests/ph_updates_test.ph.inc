<?php
/**
 * @file
 * Test update feed implementations.
 */

/**
 * Implements hook_ph_updates_info().
 */
function ph_updates_test_ph_updates_info() {
  return array(
    'test' => array(
      'label' => t('Test'),
      'property info' => array(
        'value' => array(
          'type' => 'text',
          'label' => t('Value'),
        ),
        'struct' => array(
          'type' => 'ph_test_composite',
          'label' => t('Struct'),
          'property info' => ph_updates_test_composite_property_info(),
          'auto creation' => 'entity_property_create_array',
        ),
        'composite' => array(
          'type' => 'ph_test_composite',
          'label' => t('Composite'),
          'property info' => ph_updates_test_composite_property_info(),
          'auto creation' => 'entity_property_create_array',
          'tree mapping' => array(
            'composite' => 'value',
          ),
        ),
        'custom' => array(
          'type' => 'text',
          'label' => t('Custom'),
          'tree value callback' => 'ph_updates_test_custom_value_callback',
        ),
        'list' => array(
          'type' => 'list<text>',
          'label' => t('List of values'),
        ),
        'list_nested' => array(
          'type' => 'list<text>',
          'label' => t('List of nested values'),
          'tree list mapping' => 'value',
        ),
        'list_composite' => array(
          'type' => 'list<ph_test_composite>',
          'label' => t('List of composites'),
          'property info' => ph_updates_test_composite_property_info(),
          'auto creation' => 'entity_property_create_array',
          'tree list mapping' => array(
            'composite' => 'value',
          ),
        ),
        'list_custom' => array(
          'type' => 'list<text>',
          'label' => t('List of customs'),
          'tree list value callback' => 'ph_updates_test_custom_value_callback',
        ),
        'list_custom_nested' => array(
          'type' => 'list<text>',
          'label' => t('List of customs'),
          'tree list mapping' => 'value',
          'tree list value callback' => 'ph_updates_test_custom_value_callback',
        ),
        'subinfo' => array(
          'type' => 'ph_updates_info<test>',
          'label' => t('Sub-info'),
        ),
        'subtree' => array(
          'type' => 'ph_updates_tree<test>',
          'label' => t('Subtree'),
        ),
      ),
      'format info' => array(
        '' => array(
          '/' => array(
            'general option' => 'general value',
          ),
          '/test' => array(
            'property option' => 'property value',
          ),
        ),
        'test_format' => array(
          '/' => array(
            'test option' => 'test value',
            'general option' => 'overridden',
          ),
        ),
        'xml' => array(
          '/' => array(
            'xmlns' => array(
              'x' => 'http://xml.example.com/ns',
            ),
          ),
          '/value' => array(
            'xmlns' => array(
              'x' => 'http://xml.example.com/ns',
            ),
            'prefix' => 'x',
          ),
          '/custom' => array(
            'name' => 'custom_alt',
            'xmlns' => 'http://xml.example.com/ns',
          ),
          '/custom/container' => array(
            'xmlns' => array(
              '' => 'http://xml.example.com/ns2',
              'y' => 'http://xml.example.com/ns3',
            ),
            'prefix' => 'x',
          ),
          '/list' => array(
            'list item' => 'list_item',
            'xmlns' => array(
              'x' => 'http://xml.example.com/ns1',
            ),
            'prefix' => 'a',
          ),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_ph_updates_info_alter().
 */
function ph_updates_test_ph_updates_info_alter(&$info) {
  $info['test']['format info']['']['/']['additional option'] = 'additional value';
  // Alter project info for testing.
  $info['project']['property info']['test'] = array(
    'type' => 'list<ph_updates_tree<test>>',
    'label' => t('Test'),
    'auto creation' => 'entity_property_create_array',
  );
}

/**
 * Implements hook_ph_updates_project_info_build().
 */
function ph_updates_test_ph_updates_project_info_build(PHUpdatesInfoWrapper $info, $name, $core_api) {
  if ($name == 'test_project') {
    $info->short_name = $name;
  }
  if ($core_api == '7.x') {
    $info->api_version = $core_api;
  }

  if (variable_get('ph_updates_test_build_info')) {
    $test_info = ph_updates_data_create('info', 'test');
    ph_updates_test_build_project_info($test_info);
    $info->test[0] = $test_info;
  }
}

/**
 * Implements hook_ph_updates_format_info().
 */
function ph_updates_test_ph_updates_format_info() {
  return array(
    'test_format' => array(
      'label' => t('Test format'),
      'class' => 'PHUpdatesTestFormat',
      'mimetype' => 'text/plain',
    ),
  );
}
