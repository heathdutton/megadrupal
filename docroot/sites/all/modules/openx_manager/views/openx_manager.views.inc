<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Views integration.
 */

// OpenX constants (on constants.php)
define('MAX_FINANCE_CPM',    1);
define('MAX_FINANCE_CPC',    2);
define('MAX_FINANCE_CPA',    3);
define('MAX_FINANCE_MT',     4); // Monthly Tennancy

/**
 * Implementation of hook_views_data().
 */
function openx_manager_views_data() {
  // Get the standard
  $data = _openx_manager_views_data();

  // Fields that values are on a list
  $data['openx_manager_banner']['btype']['field']['handler']                          = 'openx_manager_handler_field_list';
  $data['openx_manager_campaign']['contract_revenue_unit']['field']['handler']        = 'openx_manager_handler_field_list';
  $data['openx_manager_campaign']['priority_level_level']['field']['handler']         = 'openx_manager_handler_field_list';
  $data['openx_manager_campaign']['priority_distribution_level']['field']['handler']  = 'openx_manager_handler_field_list';
  $data['openx_manager_zone']['zone_type']['field']['handler']                        = 'openx_manager_handler_field_list';
//   $data['openx_manager_zone']['size_preset']['field']['handler']                      = 'openx_manager_handler_field_list';

  $data['openx_manager_banner']['btype']['filter']['handler']                         = 'openx_manager_handler_filter_list';
  $data['openx_manager_campaign']['contract_revenue_unit']['filter']['handler']       = 'openx_manager_handler_filter_list';
  $data['openx_manager_campaign']['priority_level_level']['filter']['handler']        = 'openx_manager_handler_filter_list';
  $data['openx_manager_campaign']['priority_distribution_level']['filter']['handler'] = 'openx_manager_handler_filter_list';
  $data['openx_manager_zone']['zone_type']['filter']['handler']                       = 'openx_manager_handler_filter_list';
//   $data['openx_manager_zone']['size_preset']['filter']['handler']                     = 'openx_manager_handler_filter_list';

  // Banner image
  $data['openx_manager_banner']['data_image']['field']['handler']                     = 'openx_manager_handler_image';
  unset($data['openx_manager_banner']['data_image']['filter']);
  unset($data['openx_manager_banner']['data_image']['sort']);
  unset($data['openx_manager_banner']['data_image']['argument']);

  return $data;
}

/**
 * Helper function for openx_manager_views_data(). It scans all fields
 * from the module DB tables and export a Views field list.
 *
 * @return
 *   Array. Each table and eachfield from hook_schema, in the way Views
 *   module use.
 */
function _openx_manager_views_data() {
  // Get all fields from hook_schema
  module_load_include('install', 'openx_manager');
  $sql_tables = openx_manager_schema();

  foreach ($sql_tables as $table => $sql_fields) {
    // The link table is not needed
    if ($table == 'openx_manager_link') {
      continue;
    }

    // Basic table definitions
    $data[$table]['table']['group']  = t('OpenX !type', array('!type' => $sql_fields['description']));
    $data[$table]['table']['join'] = array(
      'node' => array(
        'left_field'  => 'nid',
        'field'       => 'nid',
      ),
    );

    // Scan all fields
    foreach ($sql_fields['fields'] as $field_name => $field_data) {
      if ($field_name != 'nid') {
        if ($field_name == 'contract_expiration' or $field_name == 'contract_activation') {
          $handler_field  = 'views_handler_field_date';
          $handler_sort   = 'views_handler_sort_date';
          $handler_filter = 'views_handler_filter_date';
          $handler_arg    = 'views_handler_argument_date';
        }
        elseif ($field_data['type'] == 'int' and $field_data['size'] == 'tiny') {
          $handler_field  = 'views_handler_field_boolean';
          $handler_sort   = 'views_handler_sort';
          $handler_filter = 'views_handler_filter_boolean_operator';
          $handler_arg    = 'views_handler_argument';
        }
        elseif ($field_data['type'] == 'int') {
          $handler_field  = 'views_handler_field_numeric';
          $handler_sort   = 'views_handler_sort';
          $handler_filter = 'views_handler_filter_numeric';
          $handler_arg    = 'views_handler_argument_numeric';
        }
        else {
          $handler_field  = 'views_handler_field';
          $handler_sort   = 'views_handler_sort';
          $handler_filter = 'views_handler_filter';
          $handler_arg    = 'views_handler_argument_string';
        }

        // Use the default
        $data[$table][$field_name] = array(
          'title'     => (empty($field_data['title']) ? $field_name : $field_data['title']),
          'help'      => (empty($field_data['description']) ? ' ' : $field_data['description']),
          'field'     => array(
            'handler'         => $handler_field,
            'click sortable'  => TRUE,
          ),
          'sort'      => array(
            'handler'         => $handler_sort,
          ),
          'filter'    => array(
            'handler'         => $handler_filter,
          ),
          'argument'  => array(
            'handler'         => $handler_arg,
          ),
        );
      }
    }
  }

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function openx_manager_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'openx_manager') .'/views',
    ),
    'handlers' => array(
      'openx_manager_handler_field_list' => array(
        'parent' => 'views_handler_field',
      ),
      'openx_manager_handler_filter_list' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'openx_manager_handler_image' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Return a list of values from each of OpenX fields that are
 * based on a list.
 *
 * @param $field
 *   String. The field name, like the one on hook_shcema.
 * @return
 *   Array. A list of possible values for this field.
 */
function _openx_manager_views_list($field) {
  if ($field == 'contract_revenue_unit') {
    return array(
      MAX_FINANCE_CPM   => t('CPM'),
      MAX_FINANCE_CPC   => t('CPC'),
      MAX_FINANCE_CPA   => t('CPA'),
      MAX_FINANCE_MT    => t('Monthly Tenancy'),
    );
  }
  elseif ($field == 'btype') {
    return array(
      'sql'   => t('OpenX server banner (stored as a SQL data)'),
      'web'   => t('OpenX server banner (stored as a file)'),
      'url2'  => t('Local banner'),
      'url'   => t('External banner'),
      'html'  => t('HTML banner'),
      'txt'   => t('Text ad'),
    );
  }
  elseif ($field == 'priority_level_level') {
    $options[-1] = t('Exclusive (Overrides other linked campaigns)');
    for ($index = 10; $index > 0; $index--) {
      $options[$index] = t('High (Paid campaigns level !level)', array('!level' => $index));
    }
    $options[0] = t('Low (House and unpaid campaigns)');
    return $options;
  }
  elseif ($field == 'priority_distribution_level') {
    return array(
      t('Automatic - Distribute the remaining inventory evenly over the remaining number of days'),
      t('Manual'),
      t('None - Set the campaign weight to'),
    );
  }
  elseif ($field == 'zone_type') {
    return array(
      OX_ZONE_BANNER        => t('Banner, Button or Rectangle'),
      OX_ZONE_INTERSTITIAL  => t('Interstitial or Floating DHTML'),
      OX_ZONE_POPUP         => t('Popup'),
      OX_ZONE_TEXT          => t('Text'),
      OX_ZONE_EMAIL         => t('Email or Newsletter'),
    );
  }
  elseif ($field == 'size_preset') {
    return array(
      '468x60'  => t('IAB Full Banner (468 x 60)'),
      '120x600' => t('IAB Skyscraper (120 x 600)'),
      '728x90'  => t('IAB Leaderboard (728 x 90)'),
      '120x90'  => t('IAB Button 1 (120 x 90)'),
      '120x60'  => t('IAB Button 2 (120 x 60)'),
      '234x60'  => t('IAB Half Banner (234 x 60)'),
      '728x90'  => t('IAB Leader Board (728 x 90) *'),
      '88x31'   => t('IAB Micro Bar (88 x 31)'),
      '125x125' => t('IAB Square Button (125 x 125)'),
      '120x240' => t('IAB Vertical Banner (120 x 240)'),
      '180x150' => t('IAB Rectangle (180 x 150) *'),
      '300x250' => t('IAB Medium Rectangle (300 x 250) *'),
      '336x280' => t('IAB Large Rectangle (336 x 280)'),
      '240x400' => t('IAB Vertical Rectangle (240 x 400)'),
      '250x250' => t('IAB Square Pop-up (250 x 250)'),
      '160x600' => t('IAB Wide Skyscraper (160 x 600) *'),
      ''        => t('Custom'),
    );
  }
}
