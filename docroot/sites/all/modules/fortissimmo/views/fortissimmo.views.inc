<?php

/**
 * @file
 * This file defines Views data & handlers
 */

/**
 * Implements hook_views_data().
 * Describes the Fortissimmo tables to Views
 */
function fortissimmo_views_data() {
  $tables = array();
  $accounts = fortissimmo_get_accounts();
    if ($accounts) {
    foreach ($accounts as $bedrijfid => $name) {
      // PAND table
      $tables['tbl' . $bedrijfid . '_pand'] = array(
        'table' => array(
          'group' => t('Fortissimmo Property'),
          'base' => array(
            'field' => 'fortissimmo',
            'title' => 'Fortissimmo Property (' . check_plain($name) . ')',
            'help' => t('Exposes Fortissimmo property data'),
          ),
        ),
        'fortissimmo' => array(
          'title' => t('Fortissimmo ID'),
          'help' => t('The unique fortissimmo ID'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
          'argument' => array(
            'handler' => 'views_handler_argument_fortissimmo',
            'parent' => 'views_handler_argument_numeric',
            'name field' => 'reference', // the field to display in the summary.
            'numeric' => TRUE,
          ),
          'sort' => array(
            'handler' => 'views_handler_sort',
          ),
        ),
        'price' => array(
          'title' => t('Property price'),
          'help' => t('Price of the property'),
          'field' => array(
            'handler' => 'views_handler_field_price',
            'click sortable' => TRUE,
          ),
          'filter' => array(
            'handler' => 'views_handler_filter_numeric',
          ),
          'sort' => array(
            'handler' => 'views_handler_sort',
          ),
        ),
        'price_visible' => array(
          'title' => t('Price visible'),
          'help' => t('Price visible'),
          'filter' => array(
            'handler' => 'views_handler_filter_boolean_operator',
          ),
          'sort' => array(
            'handler' => 'views_handler_sort',
          ),
        ),
        'typevalue' => array(
          'title' => t('Type of property (obsolete)'),
          'help' => t('Type of property - This field is obsolete. Please replace with newer "Type of property".'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
        'type' => array(
          'title' => t('Type of property'),
          'help' => t('Type of property'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
          'filter' => array(
            'handler' => 'views_handler_filter_property_type',
          ),
        ),
        'city' => array(
          'title' => t('Address: City'),
          'help' => t('Address: City'),
          'field' => array(
            'handler' => 'views_handler_field_city',
            'click sortable' => TRUE,
          ),
          'filter' => array(
            'handler' => 'views_handler_filter_city',
            'help' => t('Address: City - WARNING: be sure to add "Address visible" filter to avoid privacy issues!'),
          ),
        ),
        'cityValue_Alias' => array(
          'title' => t('Address: City alias'),
          'help' => t('Address: City alias'),
          'field' => array(
            'handler' => 'views_handler_field_address',
            'click sortable' => TRUE,
          ),
        ),

        'transaction' => array(
          'title' => t('Transaction'),
          'help' => t('Type of transaction'),
          'field' => array(
            'handler' => 'views_handler_field_transaction',
            'click sortable' => TRUE,
          ),
          'filter' => array(
            'handler' => 'views_handler_filter_transaction',
          ),
          'sort' => array(
            'handler' => 'views_handler_sort',
          ),
        ),
        'date_created_fortissimmo' => array(
          'title' => t('Date: created in Fortissimmo'),
          'help' => t('Date on which the property was first created in Fortissimmo'),
          'field' => array(
            'handler' => 'views_handler_field_datetime',
            'click sortable' => TRUE,
          ),
          'filter' => array(
            'handler' => 'views_handler_filter_datetime',
          ),
          'sort' => array(
            'handler' => 'views_handler_sort_datetime',
          ),
        ),
        'date_created' => array(
          'title' => t('Date: first synchronized'),
          'help' => t('Date on which the property was first synchronized'),
          'field' => array(
            'handler' => 'views_handler_field_datetime',
            'click sortable' => TRUE,
          ),
          'filter' => array(
            'handler' => 'views_handler_filter_datetime',
          ),
          'sort' => array(
            'handler' => 'views_handler_sort_datetime',
          ),
        ),
        'date_changed' => array(
          'title' => t('Date: updated date'),
          'help' => t('Date on which the property was last updated'),
          'field' => array(
            'handler' => 'views_handler_field_datetime',
            'click sortable' => TRUE,
          ),
          'filter' => array(
            'handler' => 'views_handler_filter_datetime',
          ),
          'sort' => array(
            'handler' => 'views_handler_sort_datetime',
          ),
        ),
        'price_changed' => array(
          'title' => t('Date: updated price'),
          'help' => t('Date on which the property price was last updated'),
          'field' => array(
            'handler' => 'views_handler_field_datetime',
            'click sortable' => TRUE,
          ),
          'filter' => array(
            'handler' => 'views_handler_filter_datetime',
          ),
          'sort' => array(
            'handler' => 'views_handler_sort_datetime',
          ),
        ),

        'reference' => array(
          'title' => t('Reference'),
          'help' => t('Fortissimmo reference'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
        'box' => array(
          'title' => t('Address: Box'),
          'help' => t('Address: Box'),
          'field' => array(
            'handler' => 'views_handler_field_address',
            'click sortable' => TRUE,
          ),
        ),
        'postal_code' => array(
          'title' => t('Address: Postal code'),
          'help' => t('Address: Postal code'),
          'field' => array(
            'handler' => 'views_handler_field_address',
            'click sortable' => TRUE,
          ),
        ),
        'countryValue' => array(
          'title' => t('Address: Country'),
          'help' => t('Address: Country'),
          'field' => array(
            'handler' => 'views_handler_field_address',
            'click sortable' => TRUE,
          ),
        ),
        'number' => array(
          'title' => t('Address: Number'),
          'help' => t('Address: Number'),
          'field' => array(
            'handler' => 'views_handler_field_address',
            'click sortable' => TRUE,
          ),
        ),
        'address_visible' => array(
          'title' => t('Address visible'),
          'help' => t('Address visible'),
          'filter' => array(
            'handler' => 'views_handler_filter_boolean_operator',
          ),
          'sort' => array(
            'handler' => 'views_handler_sort',
          ),
        ),
        'street' => array(
          'title' => t('Address: Street'),
          'help' => t('Address: Street'),
          'field' => array(
            'handler' => 'views_handler_field_address',
            'click sortable' => TRUE,
          ),
        ),
        'description' => array(
          'title' => t('Description'),
          'help' => t('Property description'),
          'field' => array(
            'handler' => 'views_handler_field_description',
            'click sortable' => TRUE,
          ),
        ),
        'title' => array(
          'title' => t('Title'),
          'help' => t('Property title'),
          'field' => array(
            'handler' => 'views_handler_field_fortissimmo_title',
            'click sortable' => TRUE,
          ),
        ),
        'available' => array(
          'title' => t('Availability'),
          'help' => t('When is property available'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
          'sort' => array(
            'handler' => 'views_handler_sort',
          ),
        ),
        'area_build' => array(
          'title' => t('Area already build'),
          'help' => t('Area already build in m2'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'float' => TRUE,
          ),
        ),
        'area_buildable' => array(
          'title' => t('Area livable'),
          'help' => t('Area livable in m2'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'float' => TRUE,
          ),
        ),
        'area_ground' => array(
          'title' => t('Area total'),
          'help' => t('Total area in m2'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'float' => TRUE,
          ),
        ),
        'depth_ground' => array(
          'title' => t('Ground: depth'),
          'help' => t('Depth ground in m'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'float' => TRUE,
          ),
        ),
        'depth_house' => array(
          'title' => t('House: depth'),
          'help' => t('Depth house in m'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'float' => TRUE,
          ),
        ),
        'width_ground' => array(
          'title' => t('Ground: width'),
          'help' => t('Width ground in m'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'float' => TRUE,
          ),
        ),
        'width_house' => array(
          'title' => t('House: width'),
          'help' => t('Width house in m'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'float' => TRUE,
          ),
        ),
        'construction_year' => array(
          'title' => t('Construction year'),
          'help' => t('Construction year'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
        'renovation_year' => array(
          'title' => t('Renovation year'),
          'help' => t('Renovation year'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
        'renovation_year' => array(
          'title' => t('Renovation year'),
          'help' => t('Renovation year'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
        'elevator' => array(
          'title' => t('Elevator'),
          'help' => t('Elevator'),
          'field' => array(
            'handler' => 'views_handler_field_boolean',
            'click sortable' => TRUE,
          ),
        ),
        'floor' => array(
          'title' => t('Floor'),
          'help' => t('Floor'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
        ),
        'floor_id' => array(
          'title' => t('Floor ID'),
          'help' => t('Floor ID'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
        'floors_Total' => array(
          'title' => t('Total number of floors'),
          'help' => t('Total number of floors'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
        ),
        'indexed_ki' => array(
          'title' => t('KI: indexed'),
          'help' => t('Indexed KI'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'float' => TRUE,
          ),
        ),
        'non_indexed_ki' => array(
          'title' => t('KI: non indexed'),
          'help' => t('Non-indexed KI'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'float' => TRUE,
          ),
        ),
        'conditionvalue' => array(
          'title' => t('Condition'),
          'help' => t('Non-Condition KI'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
        'building_typevalue' => array(
          'title' => t('Building type'),
          'help' => t('Building type'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
        'town_planningvalue' => array(
          'title' => t('Licenses: Town planning'),
          'help' => t('Town planning'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
        'building_license' => array(
          'title' => t('Licenses: Building license'),
          'help' => t('Building license'),
          'field' => array(
            'handler' => 'views_handler_field_license',
            'click sortable' => TRUE,
          ),
        ),
        'allocation_license' => array(
          'title' => t('Licenses: Allocation license'),
          'help' => t('Allocation license'),
          'field' => array(
            'handler' => 'views_handler_field_license',
            'click sortable' => TRUE,
          ),
        ),
        'presale_right' => array(
          'title' => t('Licenses: Presale license'),
          'help' => t('Presale license'),
          'field' => array(
            'handler' => 'views_handler_field_license',
            'click sortable' => TRUE,
          ),
        ),
        'town_planning_violation' => array(
          'title' => t('Licenses: Town planning violation'),
          'help' => t('Town planning violation'),
          'field' => array(
            'handler' => 'views_handler_field_license',
            'click sortable' => TRUE,
          ),
        ),
        'direction_gardenvalue' => array(
          'title' => t('Garden direction'),
          'help' => t('Garden direction'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),

        'ref_parent' => array(
          'title' => t('Parent property'),
          'help' => t('Parent property Fortissimmo ID'),
          'field' => array(
            'handler' => 'views_handler_field_ref_parent',
            'click sortable' => TRUE,
          ),
          'argument' => array(
            'handler' => 'views_handler_argument_fortissimmo',
            'parent' => 'views_handler_argument_numeric',
            'name field' => 'reference', // the field to display in the summary.
            'numeric' => TRUE,
          ),
          'filter' => array(
            'title' => t('Has parent'),
            'handler' => 'views_handler_filter_boolean_operator_ref_parent',
          ),
        ),
        'registration_cost' => array(
          'title' => t('Registration cost'),
          'help' => t('Registration cost'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'float' => TRUE,
          ),
        ),
        'picture_0_file' => array(
          'field' => array(
            'title' => t('Main picture'),
            'help' => t('Fortissimmo property main picture'),
            'handler' => 'views_handler_field_picture_file',
            'click sortable' => TRUE,
          ),
        ),
        'longitude' => array(
          'title' => t('Coordinates: longitude'),
          'help' => t('Coordinates: longitude'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'float' => TRUE,
          ),
        ),
        'latitude' => array(
          'title' => t('Coordinates: latitude'),
          'help' => t('Coordinates: longitude'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'float' => TRUE,
          ),
        ),
        'Building_Name' => array(
          'title' => t('Building Name'),
          'help' => t('Building Name'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),

        'Bedrooms' => array(
          'title' => t('Number of bedrooms'),
          'help' => t('Number of bedrooms'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
        ),
        'certificate_EP' => array(
          'title' => t('EP Certificate'),
          'help' => t('EP Certificate'),
          'field' => array(
            'handler' => 'views_handler_field_license',
            'click sortable' => TRUE,
          ),
        ),
        'certificate_Electricity' => array(
          'title' => t('Certificate Electricity'),
          'help' => t('Certificate Electricity'),
          'field' => array(
            'handler' => 'views_handler_field_license',
            'click sortable' => TRUE,
          ),
        ),
        'E_Level' => array(
          'title' => t('E-Level'),
          'help' => t('E-Level'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
        ),
        'EPC_value' => array(
          'title' => t('EPC Value'),
          'help' => t('EPC Value'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
        ),
        'file_0_file' => array(
          'title' => t('File: 0'),
          'help' => t('First attachment file'),
          'field' => array(
            'handler' => 'views_handler_field_fortissimmo_file',
            'click sortable' => TRUE,
          ),
        ),
        'file_1_file' => array(
          'title' => t('File: 1'),
          'help' => t('Second attachment file'),
          'field' => array(
            'handler' => 'views_handler_field_fortissimmo_file',
            'click sortable' => TRUE,
          ),
        ),
        'file_2_file' => array(
          'title' => t('File: 2'),
          'help' => t('Third attachment file'),
          'field' => array(
            'handler' => 'views_handler_field_fortissimmo_file',
            'click sortable' => TRUE,
          ),
        ),
        'file_3_file' => array(
          'title' => t('File: 3'),
          'help' => t('Fourth attachment file'),
          'field' => array(
            'handler' => 'views_handler_field_fortissimmo_file',
            'click sortable' => TRUE,
          ),
        ),
        'file_4_file' => array(
          'title' => t('File: 4'),
          'help' => t('Fifth attachment file'),
          'field' => array(
            'handler' => 'views_handler_field_fortissimmo_file',
            'click sortable' => TRUE,
          ),
        ),
        'file_5_file' => array(
          'title' => t('File: 5'),
          'help' => t('Sixth attachment file'),
          'field' => array(
            'handler' => 'views_handler_field_fortissimmo_file',
            'click sortable' => TRUE,
          ),
        ),
      );

      // Facility fields
      $tblresult = db_query_range("SELECT * FROM {tbl" . $bedrijfid . "_pand}", 0, 1);
      if ($tblresult) {
        foreach ($tblresult as $record) {
          foreach ($record as $field => $value) {
            if (substr($field, 0, 9) == 'facility_' && strpos($field, '_name') !== FALSE) {
              $tables['tbl' . $bedrijfid . '_pand'][$field] = array(
                'title' => $field,
                'help' => t('Facility field:') . $field,
                'field' => array(
                  'handler' => 'views_handler_field',
                  'click sortable' => TRUE,
                ),
              );
            }
          }
        }
      }

      // Environment fields
      $tblresult = db_query_range("SELECT * FROM {tbl" . $bedrijfid . "_pand}", 0, 1);
      if ($tblresult) {
        foreach ($tblresult as $record) {
          foreach ($record as $field => $value) {
            if (substr($field, 0, 12) == 'environment_' && strpos($field, '_name') !== FALSE) {
              $tables['tbl' . $bedrijfid . '_pand'][$field] = array(
                'title' => $field,
                'help' => t('Environment field:') . $field,
                'field' => array(
                  'handler' => 'views_handler_field',
                  'click sortable' => TRUE,
                ),
              );
              $comment_field = str_replace('_name', '_comment', $field);
              $tables['tbl' . $bedrijfid . '_pand'][$comment_field] = array(
                'title' => $comment_field,
                'help' => t('Environment field:') . $comment_field,
                'field' => array(
                  'handler' => 'views_handler_field',
                  'click sortable' => TRUE,
                ),
              );
            }
          }
        }
      }

      //
      // PAND PICTURES table
      $tables['tbl' . $bedrijfid . '_pand_pictures'] = array(
        'table' => array(
          'group' => t('Fortissimmo property pictures'),
          'base' => array(
            'field' => 'id',
            'title' => 'Fortissimmo property pictures (' . check_plain($name) . ')',
            'help' => t('Exposes Fortissimmo property pictures'),
          ),
        ),
        'fortissimmo' => array(
          'title' => t('Fortissimmo ID'),
          'help' => t('The unique fortissimmo ID'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
          'argument' => array(
            'handler' => 'views_handler_argument_numeric',
            'numeric' => TRUE,
          ),
        ),
        'picture_index' => array(
          'title' => t('Picture index'),
          'help' => t('Picture index'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
          'sort' => array(
            'handler' => 'views_handler_sort',
          ),
        ),
        'picture_file' => array(
          'title' => t('Picture'),
          'help' => t('Fortissimmo property picture'),
          'field' => array(
            'handler' => 'views_handler_field_pand_picture_file',
            'click sortable' => TRUE,
          ),
        ),
        'picture_comment' => array(
          'title' => t('Picture comment'),
          'help' => t('Fortissimmo property picture comment text'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
      );

      // PAND LAYOUT table
      $tables['tbl' . $bedrijfid . '_pand_layout'] = array(
        'table' => array(
          'group' => t('Fortissimmo property layout'),
          'base' => array(
            'field' => 'id',
            'title' => 'Fortissimmo property layout info (' . check_plain($name) . ')',
            'help' => t('Exposes Fortissimmo property layout information'),
          ),
        ),
        'id' => array(
          'title' => t('Property layout ID'),
          'help' => t('Property layout ID'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
          'sort' => array(
            'handler' => 'views_handler_sort',
          ),
        ),
        'fortissimmo' => array(
          'title' => t('Fortissimmo ID'),
          'help' => t('The unique fortissimmo ID'),
          'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
          'argument' => array(
            'handler' => 'views_handler_argument_numeric',
            'numeric' => TRUE,
          ),
        ),
        'taal' => array(
          'title' => t('Language'),
          'help' => t('Language'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
        'name' => array(
          'title' => t('Layout item name'),
          'help' => t('Name of the layout item'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
        'value' => array(
          'field' => array(
            'title' => t('Value'),
            'help' => t('Value'),
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
        ),
        'count' => array(
          'field' => array(
            'title' => t('Count'),
            'help' => t('Count'),
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
        ),
        'floor' => array(
          'field' => array(
            'title' => t('Floor number'),
            'help' => t('Floor number'),
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
        ),
        'surface' => array(
          'field' => array(
            'title' => t('Surface'),
            'help' => t('Surface'),
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
          ),
        ),
        'comment' => array(
          'title' => t('Comment'),
          'help' => t('Layout item comment'),
          'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
          ),
        ),
      );
    }
  }
  return $tables;
}