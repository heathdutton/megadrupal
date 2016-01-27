<?php

function commerce_price_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return devel_generate_multiple('_commerce_price_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _commerce_price_devel_generate($object, $field, $instance, $bundle);
  }
}

function _commerce_price_devel_generate($object, $field, $instance, $bundle) {
  $object_field = array();
  $min = !empty($object->commerce_devel_generate['min_price']) ? $object->commerce_devel_generate['min_price'] : 1;
  $max = !empty($object->commerce_devel_generate['max_price']) ? $object->commerce_devel_generate['max_price'] : 500;
  $object_field['currency_code'] = !empty($object->commerce_devel_generate['currency_code']) ? $object->commerce_devel_generate['currency_code'] : commerce_default_currency();
  $min = commerce_currency_decimal_to_amount($min, $object_field['currency_code']);
  $max = commerce_currency_decimal_to_amount($max, $object_field['currency_code']);
  $object_field['amount'] = mt_rand($min, $max);
  return $object_field;
}
