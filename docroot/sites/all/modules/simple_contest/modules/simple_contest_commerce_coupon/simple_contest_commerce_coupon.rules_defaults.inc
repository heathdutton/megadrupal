<?php



/**
 * Implements hook_default_rules_configuration().
 */
function simple_contest_commerce_coupon_default_rules_configuration() {
	$rules = array();


	// Create fixed amount rule
	$rule = rules_reaction_rule(array(), array(
		'coupon' => array(
			'type' => 'commerce_coupon',
			'label' => 'commerce coupon'
		),
		'commerce_order' => array(
			'type' => 'commerce_order',
			'label' => t('Commerce order'),
		), 
	));

	$rule->label = t('Coupon Validation: Set the coupon as invalid if coupin user is not order owner.');
	$rule->active = TRUE;

	$rule
		->event('commerce_coupon_validate')
		->condition('entity_has_field', array('entity:select' => 'coupon', 'field' => 'sc_commerce_coupon_user'))
		->condition(rules_condition('data_is', array('data:select' => 'coupon:sc-commerce-coupon-user', 'op' => '=', 'value:select' => 'commerce_order:owner'))->negate())
	;
	
	$rule->action('drupal_message', array(
		'message' => 'The coupon is owned by another user.',
		'type' => 'error',
	));
	
	$rule->action('commerce_coupon_action_is_invalid_coupon', array());
	
	$rules['simple_contest_commerce_coupon_validate_user_rule'] = $rule;
	
	return $rules;
}

//function simple_contest_commerce_coupon_default_rules_configuration_alter( &$configs ) {
	
//}
