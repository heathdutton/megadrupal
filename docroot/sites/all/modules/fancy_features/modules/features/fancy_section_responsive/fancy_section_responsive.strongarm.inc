<?php
/**
 * @file
 * fancy_section_responsive.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function fancy_section_responsive_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_fancy_section_responsive';
  $strongarm->value = '1';
  $export['auto_entitylabel_fancy_section_responsive'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_pattern_fancy_section_responsive';
  $strongarm->value = 'Responsive section created by [current-user:name] at [current-user:created:short]';
  $export['auto_entitylabel_pattern_fancy_section_responsive'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_php_fancy_section_responsive';
  $strongarm->value = 0;
  $export['auto_entitylabel_php_fancy_section_responsive'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_fancy_section__responsive';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => FALSE,
      ),
      'inline_entity_form_preview' => array(
        'custom_settings' => TRUE,
      ),
      'view_mode_selector' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_grid_four_column' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_grid_three_column' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_grid_two_column' => array(
        'custom_settings' => TRUE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_ds_2' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_ds_2_fluid' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_ds_2_stack' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_ds_2_stack_fluid' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_ds_3' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_ds_3_stack' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_ds_3_stack_fluid' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_ds_3_weighted' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_ds_4' => array(
        'custom_settings' => FALSE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_grid_two_column_stacked' => array(
        'custom_settings' => TRUE,
      ),
      'fancy_grid_3_columns' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_grid_4_columns' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_grid_6_columns' => array(
        'custom_settings' => FALSE,
      ),
      'fancy_grid_12_columns' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(),
      'display' => array(
        'field_fancy_rel_contents_1' => array(
          'fancy_grid_four_column' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'fancy_grid_three_column' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'fancy_grid_two_column' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'default' => array(
            'weight' => '21',
            'visible' => FALSE,
          ),
          'fancy_grid_two_column_stacked' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'view_mode_selector' => array(
            'weight' => '21',
            'visible' => TRUE,
          ),
        ),
        'field_fancy_rel_contents_2' => array(
          'fancy_grid_four_column' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
          'fancy_grid_three_column' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
          'fancy_grid_two_column' => array(
            'weight' => '4',
            'visible' => FALSE,
          ),
          'default' => array(
            'weight' => '22',
            'visible' => FALSE,
          ),
          'fancy_grid_two_column_stacked' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
          'view_mode_selector' => array(
            'weight' => '22',
            'visible' => TRUE,
          ),
        ),
        'field_fancy_rel_contents_3' => array(
          'fancy_grid_four_column' => array(
            'weight' => '3',
            'visible' => TRUE,
          ),
          'fancy_grid_three_column' => array(
            'weight' => '3',
            'visible' => FALSE,
          ),
          'fancy_grid_two_column' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
          'default' => array(
            'weight' => '23',
            'visible' => FALSE,
          ),
          'fancy_grid_two_column_stacked' => array(
            'weight' => '3',
            'visible' => TRUE,
          ),
          'view_mode_selector' => array(
            'weight' => '23',
            'visible' => TRUE,
          ),
        ),
      ),
    ),
  );
  $export['field_bundle_settings_fancy_section__responsive'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_split_config_fancy_section__responsive';
  $strongarm->value = array(
    'field_fancy_rel_contents' => array(
      'enable' => 1,
      'values' => '4',
    ),
  );
  $export['field_split_config_fancy_section__responsive'] = $strongarm;

  return $export;
}
