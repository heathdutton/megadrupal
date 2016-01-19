<?php


/**
 * Implementation of hook_commerce_coupon_type_configure
 */
function _simple_contest_commerce_coupon_commerce_coupon_type_configure($bundle, $reset = FALSE) {

	// Skip all $bundles except the basic coupon type
	if ($bundle != 'sc_commerce_coupon') {
		return;
	}
	
	$entity_type = 'commerce_coupon';

	// If a field type we know should exist isn't found, clear the Field cache.
	if (!field_info_field_types('commerce_price')) {
		field_cache_clear();
	}

	// Look for or add the specified fixed amount
	$field_name = 'commerce_coupon_fixed_amount';
	$field = field_info_field($field_name);
	$instance = field_info_instance($entity_type, $field_name, $bundle);
	$weight = 0;

	if (empty($field) || $reset) {
		$field_data = array(
			'field_name' => $field_name,
			'type' => 'commerce_price',
			'label' => t('Fixed Amount'),
			'cardinality' => 1,
			'entity_types' => array($entity_type),
			'translatable' => FALSE,
			'locked' => FALSE,
		);
		
		if (empty($field)) {
			$field = field_create_field($field_data);
		}
		else {
			$field = field_update_field($field_data);
		}
	}

	if (empty($instance) || $reset) {
		$instance_data = array(
			'field_name' => $field_name,
			'entity_type' => $entity_type,
			'bundle' => $bundle,

			'label' => t('Fixed Amount'),
			'required' => FALSE,
			'settings' => array(),

			// Because this widget is locked, we need it to use the full price widget
			// since the currency option can't be adjusted at the moment.
			'widget' => array(
				'type' => 'commerce_price_full',
				'weight' => $weight,
				'settings' => array(
					'currency_code' => 'default',
				),
			),

			'display' => array(),
		);
		
		if (empty($instance)) {
			field_create_instance($instance_data);
		}
		else {
			field_update_instance($instance_data);
		}
	}

	// Look for or add the specified percentage amount
	$field_name = 'commerce_coupon_percent_amount';
	$field = field_info_field($field_name);
	$instance = field_info_instance($entity_type, $field_name, $bundle);

	if (empty($field) || $reset) {
		$field_data = array(
			'field_name' => $field_name,
			'type' => 'number_float',
			'label' => t('Percentage Amount'),
			'cardinality' => 1,
			'entity_types' => array($entity_type),
			'translatable' => FALSE,
			'locked' => FALSE,
			'settings' => array(
				'decimal_separator' => '.',
			),
		);
		
		if (empty($field)) {
			$field = field_create_field($field_data);
		}
		else {
			$field = field_update_field($field_data);
		}
	}

	if (empty($instance) || $reset) {
		$instance_data = array(
			'field_name' => $field_name,
			'entity_type' => $entity_type,
			'bundle' => $bundle,

			'label' => t('Percentage Amount'),
			'required' => FALSE,
			'display' => array(),
			'settings' => array(
				'min' => '0',
				'max' => '100',
			),
		);
		
		if (empty($instance)) {
			field_create_instance($instance_data);
		}
		else {
			field_update_instance($instance_data);
		}
	}	


	// Look for or add the specified percentage amount
	$field_name = 'commerce_coupon_number_of_uses';
	$field = field_info_field($field_name);
	$instance = field_info_instance($entity_type, $field_name, $bundle);

	if (empty($field) || $reset) {
		$field_data = array(
			'field_name' => $field_name,
			'type' => 'number_integer',
			'label' => t('Number of Uses'),
			'cardinality' => 1,
			'entity_types' => array($entity_type),
			'translatable' => FALSE,
			'locked' => FALSE,
			'settings' => array(),
		);
		
		if (empty($field)) {
			$field = field_create_field($field_data);
		}
		else {
			$field = field_update_field($field_data);
		}
	}

	if (empty($instance) || $reset) {
		$instance_data = array(
			'field_name' => $field_name,
			'entity_type' => $entity_type,
			'bundle' => $bundle,

			'label' => t('Number of Uses'),
			'required' => FALSE,
			'display' => array(),
			'settings' => array(
				'min' => '0',
			),
			'default_value' => array(
				0 => array( 
					'value' => 1
				)
			),
		);

		
		if (empty($instance)) {
			field_create_instance($instance_data);
		}
		else {
			field_update_instance($instance_data);
		}
	}	

	// Look for or add the coupon user user
	$field_name = 'sc_commerce_coupon_user';
	$field = field_info_field($field_name);
	$instance = field_info_instance($entity_type, $field_name, $bundle);

	if (empty($field) || $reset) {
		$field_data = array(
			'field_name' => $field_name,
			'type' => 'user_reference',
			'label' => t('Coupon user'),
			'cardinality' => 1,
			'entity_types' => array($entity_type),
			'translatable' => FALSE,
			'locked' => TRUE,
			'settings' => array(
        'referenceable_roles' => array(),
        'referenceable_status' => array(),
        'view' => array(
          'view_name' => '',
          'display_name' => '',
          'args' => array(),
        ),
      ),
      'widget' => 'user_reference_autocomplete',
		);
		
		if (empty($field)) {
			$field = field_create_field($field_data);
		}
		else {
			$field = field_update_field($field_data);
		}
	}

	if (empty($instance) || $reset) {
		$instance_data = array(
			'field_name' => $field_name,
			'entity_type' => $entity_type,
			'bundle' => $bundle,
			'label' => t('Coupon user'),
			'required' => FALSE,
			'display' => array(),
			'default_value' => array(
				0 => array( 
					//'uid' => ,
				)
			),
		);

		
		if (empty($instance)) {
			field_create_instance($instance_data);
		}
		else {
			field_update_instance($instance_data);
		}
	}	
  
  // Look for or add the coupon contest node
	$field_name = 'sc_commerce_coupon_contest';
	$field = field_info_field($field_name);
	$instance = field_info_instance($entity_type, $field_name, $bundle);

	if (empty($field) || $reset) {
		$field_data = array(
			'field_name' => $field_name,
			'type' => 'node_reference',
			'label' => t('Coupon contest'),
			'cardinality' => 1,
			'entity_types' => array($entity_type),
			'translatable' => FALSE,
			'locked' => TRUE,
			'settings' => array(
        'view' => array(
          'view_name' => '',
          'display_name' => '',
          'args' => array(),
        ),
      ),
      'widget' => 'node_reference_autocomplete',
		);
		
		if (empty($field)) {
			$field = field_create_field($field_data);
		}
		else {
			$field = field_update_field($field_data);
		}
	}

	if (empty($instance) || $reset) {
		$instance_data = array(
			'field_name' => $field_name,
			'entity_type' => $entity_type,
			'bundle' => $bundle,
			'label' => t('Coupon contest'),
			'required' => FALSE,
			'display' => array(),
			'default_value' => array(
				0 => array( 
					//'nid' => ,
				)
			),
		);

		
		if (empty($instance)) {
			field_create_instance($instance_data);
		}
		else {
			field_update_instance($instance_data);
		}
	}	
}
