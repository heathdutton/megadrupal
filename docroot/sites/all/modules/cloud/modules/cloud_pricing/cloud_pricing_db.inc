<?php

/**
 * @file
 * Provides pricing for hourly rate configuration for Billing module.
 * Works with Cloud and Billing module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Get all instances db query
 */
function _cloud_pricing_get_instances($cloud_context = '') {

  $query = db_select(CLOUD_PRICING_INSTANCES_TABLE, 'a');
  $query->fields('a');

  if (!empty($cloud_context)) {
    $query->condition('cloud_type', $cloud_context);
  }

  return $query;
}

/**
 * Delete a price from the db
 */
function _cloud_pricing_delete($id) {

  //// add watchdog log
  $query = _cloud_pricing_get_instances();
  $query->condition('id', $id);

  $result = $query->execute();
  $count = 0;
  $pricing_obj    = $result->fetchObject();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('Pricing has been deleted: @pricing', array('@pricing' => $pricing_obj->instance_type)),
      'link' => '',
    )
  );

  $query = db_delete(CLOUD_PRICING_INSTANCES_TABLE)
  ->condition('id', $id)
  ->execute();
}

/**
 * Insert a pricing in database
 * This function insert entry in database and audit logs as well.
 *
 * @param $form_values
 *        This is the form-values submitted by new pricing form
 * @return return a last inserted pricing-id
 */
function _cloud_pricing_insert($form_values) {

  $insert_query_instance = db_insert(CLOUD_SERVER_TEMPLATES_INSTANCE_TYPE_TABLE)
  ->fields(array(
    'instance_type' => $form_values['instance_type_select'                 ],
    'description'   => $form_values['description_text'                 ],
    'cloud_type'    => (!empty($form_values['cloud_context'])
                     ?         $form_values['cloud_context'] : ''),
  ))
  ->execute();

  $insert_query = db_insert(CLOUD_PRICING_INSTANCES_TABLE)
  ->fields(array(
    'instance_type'       => $form_values['instance_type_select'    ],
    'description'         => $form_values['description_text'        ],
    'linux_or_unix_usage' => $form_values['linux_or_unix_usage_text'],
    'windows_usage'       => $form_values['windows_usage_text'      ],
    'cloud_type'          => (!empty($form_values['cloud_context']  )
                           ?         $form_values['cloud_context']
                           :         ''                             ),
    'created'             => date('c'),
    'updated'             => date('c'),
  ))
  ->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('New Pricing has been added: @pricing',
                   array('@pricing' => $form_values['instance_type_select'])),
      'link' => '',
    )
  );

  return;
}

/**
 * Update a price in the database
 */
function _cloud_pricing_update($form_values, $pricing_id = '') {

  $query_args = array();

  $update_query_instance = db_update(CLOUD_SERVER_TEMPLATES_INSTANCE_TYPE_TABLE)
    ->fields(array(
      'instance_type' => $form_values['instance_type_select'                 ],
    ))
    ->condition('instance_type', $form_values['instance_type_old'])
    ->execute();

  $query_args = array();

  $update_query = db_update(CLOUD_PRICING_INSTANCES_TABLE)
    ->fields(array(
      'instance_type'       => $form_values['instance_type_select'    ],
      'description'         => $form_values['description_text'        ],
      'linux_or_unix_usage' => $form_values['linux_or_unix_usage_text'],
      'windows_usage'       => $form_values['windows_usage_text'      ],
      'cloud_type'          => (!empty( $form_values['cloud_context'] )
                            ?           $form_values['cloud_context']
                            :           ''                            ),
      'updated'             => date('c'),
    ))
    ->condition('id', $pricing_id)
    ->execute();

  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('Pricing has been modified: @pricing', array('@pricing' => $pricing_id)),
      'link' => '',
    )
  );

  return;
}

/**
 * Delete pricing data from the database  
 */
function cloud_pricing_delete_data($cloud_context) {

  $query = db_delete(CLOUD_PRICING_INSTANCES_TABLE)
  ->condition('cloud_type', $cloud_context)
  ->execute();
}


/**
 * Delete all pricing data
 */
function cloud_pricing_delete_all_data() {

  $query = db_delete(CLOUD_PRICING_INSTANCES_TABLE)
  ->execute();
}

/**
 * Get pricing by id
 */
function _cloud_pricing_get_by_id($cloud_context, $instance_type) {

  $query_args = array();
  $query = db_select(CLOUD_PRICING_INSTANCES_TABLE, 'a');

  $result = $query
    ->fields('a')
    ->condition('instance_type', $instance_type)
    ->condition('cloud_type'   , $cloud_context)
    ->execute();

  $pricing_info = array();

  foreach ($result as $pricing_obj) {
    $pricing_info['instance_type'      ] = $pricing_obj->instance_type;
    $pricing_info['cloud_type'         ] = $pricing_obj->cloud_type;
    $pricing_info['linux_or_unix_usage'] = $pricing_obj->linux_or_unix_usage;
  }
  return $pricing_info;
}
