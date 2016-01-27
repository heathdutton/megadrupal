<?php
/**
 * @file
 * one_date.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function one_date_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-one_site_content-field_one_site_date_partial'
  $field_instances['node-one_site_content-field_one_site_date_partial'] = array(
    'bundle' => 'one_site_content',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'partial_date',
        'settings' => array(
          'component_settings' => array(),
          'format' => 'medium',
          'use_override' => 'none',
        ),
        'type' => 'partial_date_default',
        'weight' => 10,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'partial_date',
        'settings' => array(
          'component_settings' => array(
            'components' => array(
              'approx' => array(
                'value' => '',
                'weight' => -1,
              ),
              'c1' => array(
                'value' => '',
                'weight' => 7,
              ),
              'c2' => array(
                'value' => '',
                'weight' => 8,
              ),
              'c3' => array(
                'value' => '',
                'weight' => 9,
              ),
              'day' => array(
                'empty' => '',
                'format' => 'j-S',
                'weight' => 2,
              ),
              'hour' => array(
                'empty' => '',
                'format' => 'h',
                'weight' => 3,
              ),
              'minute' => array(
                'empty' => '',
                'format' => 'i',
                'weight' => 4,
              ),
              'month' => array(
                'empty' => '',
                'format' => 'M',
                'weight' => 1,
              ),
              'second' => array(
                'empty' => '',
                'format' => 's',
                'weight' => 5,
              ),
              'timezone' => array(
                'empty' => '',
                'format' => 'T',
                'weight' => 6,
              ),
              'year' => array(
                'empty' => '',
                'format' => 'Y',
                'weight' => 0,
              ),
            ),
            'display' => array(
              'day' => 'estimate_label',
              'hour' => 'estimate_label',
              'minute' => 'estimate_label',
              'month' => 'estimate_label',
              'second' => 'estimate_label',
              'timezone' => 'date_only',
              'year' => 'estimate_label',
            ),
            'meridiem' => 'a',
            'separator' => array(
              'date' => ' ',
              'datetime' => ' ',
              'other' => ' ',
              'range' => '',
              'time' => ':',
            ),
            'year_designation' => 'bc',
          ),
          'format' => 'medium',
          'use_override' => 'short',
        ),
        'type' => 'partial_date_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_one_site_date_partial',
    'label' => 'Date',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'partial_date',
      'settings' => array(
        0 => FALSE,
        'estimates' => array(
          'from' => array(
            'day' => 0,
            'hour' => 0,
            'minute' => 0,
            'month' => 0,
            'second' => 0,
            'year' => 0,
          ),
          'to' => array(
            'day' => 0,
            'hour' => 0,
            'minute' => 0,
            'month' => 0,
            'second' => 0,
            'year' => 0,
          ),
        ),
        'granularity' => array(
          'from' => array(
            'day' => 'day',
            'hour' => 'hour',
            'minute' => 'minute',
            'month' => 'month',
            'second' => 0,
            'timezone' => 0,
            'year' => 'year',
          ),
          'to' => array(
            'day' => 'day',
            'hour' => 'hour',
            'minute' => 'minute',
            'month' => 'month',
            'second' => 0,
            'timezone' => 0,
            'year' => 'year',
          ),
        ),
        'help_txt' => array(
          'und' => array(
            '_remove' => '',
            'check_approximate' => '',
            'components' => '',
            'txt_long' => 'Longer description of date',
            'txt_short' => 'Short description of date',
          ),
        ),
        'hide_remove' => 0,
        'increments' => array(
          'minute' => 15,
          'second' => 1,
        ),
        'theme_overrides' => array(
          'check_approximate' => 0,
          'range_inline' => 0,
          'txt_long' => 0,
          'txt_short' => 1,
        ),
        'tz_handling' => 'none',
      ),
      'type' => 'partial_date',
      'weight' => 7,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Date');

  return $field_instances;
}
