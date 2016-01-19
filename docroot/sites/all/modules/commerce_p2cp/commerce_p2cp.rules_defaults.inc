<?php

/**
 * Implementation of hook_default_rules_configuration().
 */
function commerce_p2cp_default_rules_configuration() {
  $items = array();

  // Next rule needs to be called after the order has been assigned a user
  // (either new or existing).
  $weight = rules_config_load('commerce_checkout_order_convert')->weight;
  $weight = max($weight, rules_config_load('commerce_checkout_new_account')->weight);

  $rule = (object)array(
   'rules_commerce_p2cp_user_profile2' => (object)array(
      'LABEL' => 'Commerce Profile2 Checkout pane: Assign Profile2 to order owner',
      'PLUGIN' => 'reaction rule',
      'WEIGHT' => $weight + 1,
      'TAGS' => array (),
      'REQUIRES' => array (
        'rules',
        'commerce_checkout',
      ),
      'ON' => array ('commerce_checkout_complete'),
      'IF' => array(
        (object)array(
          'entity_is_of_type' => (object)array(
            'entity' => array('commerce-order:owner'),
            'type' => 'user',
          )
        ),
      ),
      'DO' => array(),
    ),
  );

  foreach (profile2_get_types() as $type_name => $type) {
    if ($field_name = variable_get(P2CP_PREFIX . $type_name . '_field', '')) {
      $rule->rules_commerce_p2cp_user_profile2->IF[] = (object)array(
        'entity_has_field' => (object)array(
          'entity' => array ('commerce-order'),
          'field' => $field_name,
        )
      );
      $field_name_dashed = str_replace('_', '-', $field_name);
      $rule->rules_commerce_p2cp_user_profile2->DO[] = (object)array(
        'data_set' => (object)array(
          'data' => array("commerce-order:$field_name_dashed:user"),
          'value' => array ('commerce-order:owner'),
        ),
      );
    }
  }
  $items['rules_commerce_p2cp_user_profile2'] = entity_import('rules_config', json_encode($rule));

  return $items;
}
