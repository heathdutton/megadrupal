<?php
/**
 * @file
 * fancy_section_simple.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function fancy_section_simple_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_fancy_section_simple';
  $strongarm->value = '2';
  $export['auto_entitylabel_fancy_section_simple'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_pattern_fancy_section_simple';
  $strongarm->value = 'Section created by [current-user:name] at [current-user:created:short]';
  $export['auto_entitylabel_pattern_fancy_section_simple'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_php_fancy_section_simple';
  $strongarm->value = 0;
  $export['auto_entitylabel_php_fancy_section_simple'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_fancy_section__simple';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => FALSE,
      ),
      'view_mode_selector' => array(
        'custom_settings' => TRUE,
      ),
      'inline_entity_form_preview' => array(
        'custom_settings' => TRUE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_ds_4col' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_ds_2' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_ds_2_fluid' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_ds_2_stack' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_ds_2_stack_fluid' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_ds_3_weighted' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_ds_3' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_ds_3_stack' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_ds_3_stack_fluid' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_ds_4' => array(
        'custom_settings' => TRUE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(),
      'display' => array(
        'field_fancy_rel_contents_1' => array(
          'default' => array(
            'weight' => '21',
            'visible' => FALSE,
          ),
          'fancy_ds_2' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'fancy_ds_2_fluid' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'fancy_ds_2_stack' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'fancy_ds_2_stack_fluid' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'fancy_ds_3_weighted' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'fancy_ds_3' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'fancy_ds_3_stack' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'fancy_ds_3_stack_fluid' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'fancy_ds_4col' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'fancy_ds_4' => array(
            'weight' => '21',
            'visible' => TRUE,
          ),
        ),
        'field_fancy_rel_contents_2' => array(
          'default' => array(
            'weight' => '22',
            'visible' => FALSE,
          ),
          'fancy_ds_2' => array(
            'weight' => '3',
            'visible' => FALSE,
          ),
          'fancy_ds_2_fluid' => array(
            'weight' => '3',
            'visible' => FALSE,
          ),
          'fancy_ds_2_stack' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
          'fancy_ds_2_stack_fluid' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
          'fancy_ds_3_weighted' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
          'fancy_ds_3' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
          'fancy_ds_3_stack' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
          'fancy_ds_3_stack_fluid' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
          'fancy_ds_4col' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
          'fancy_ds_4' => array(
            'weight' => '22',
            'visible' => TRUE,
          ),
        ),
        'field_fancy_rel_contents_3' => array(
          'default' => array(
            'weight' => '23',
            'visible' => FALSE,
          ),
          'fancy_ds_2' => array(
            'weight' => '4',
            'visible' => FALSE,
          ),
          'fancy_ds_2_fluid' => array(
            'weight' => '4',
            'visible' => FALSE,
          ),
          'fancy_ds_2_stack' => array(
            'weight' => '3',
            'visible' => TRUE,
          ),
          'fancy_ds_2_stack_fluid' => array(
            'weight' => '3',
            'visible' => TRUE,
          ),
          'fancy_ds_3_weighted' => array(
            'weight' => '4',
            'visible' => FALSE,
          ),
          'fancy_ds_3' => array(
            'weight' => '4',
            'visible' => FALSE,
          ),
          'fancy_ds_3_stack' => array(
            'weight' => '3',
            'visible' => TRUE,
          ),
          'fancy_ds_3_stack_fluid' => array(
            'weight' => '3',
            'visible' => TRUE,
          ),
          'fancy_ds_4col' => array(
            'weight' => '3',
            'visible' => TRUE,
          ),
          'fancy_ds_4' => array(
            'weight' => '23',
            'visible' => TRUE,
          ),
        ),
        'field_fancy_rel_contents_4' => array(
          'default' => array(
            'weight' => '24',
            'visible' => FALSE,
          ),
          'fancy_ds_2' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
          'fancy_ds_2_fluid' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
          'fancy_ds_2_stack' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
          'fancy_ds_2_stack_fluid' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
          'fancy_ds_3_weighted' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
          'fancy_ds_3' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
          'fancy_ds_3_stack' => array(
            'weight' => '4',
            'visible' => TRUE,
          ),
          'fancy_ds_3_stack_fluid' => array(
            'weight' => '4',
            'visible' => TRUE,
          ),
          'fancy_ds_4col' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
          'fancy_ds_4' => array(
            'weight' => '24',
            'visible' => FALSE,
          ),
        ),
        'field_fancy_rel_contents_5' => array(
          'default' => array(
            'weight' => '25',
            'visible' => FALSE,
          ),
          'fancy_ds_2' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
          'fancy_ds_2_fluid' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
          'fancy_ds_2_stack' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
          'fancy_ds_2_stack_fluid' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
          'fancy_ds_3_weighted' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
          'fancy_ds_3' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
          'fancy_ds_3_stack' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
          'fancy_ds_3_stack_fluid' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
          'fancy_ds_4col' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
          'fancy_ds_4' => array(
            'weight' => '25',
            'visible' => FALSE,
          ),
        ),
      ),
    ),
  );
  $export['field_bundle_settings_fancy_section__simple'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_split_config_fancy_section__simple';
  $strongarm->value = array(
    'field_fancy_rel_contents' => array(
      'enable' => 1,
      'values' => '5',
    ),
  );
  $export['field_split_config_fancy_section__simple'] = $strongarm;

  return $export;
}
