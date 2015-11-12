<?php

/**
 * Implements hook_rules_action_info().
 */
function commerce_pricing_attributes_rules_action_info() {
  return array(
    'commerce_pricing_attributes_calculate_adjusment' => array(
      'label' => t('Calculate Adjusment depending on product attributes'),
      'parameter' => array(
        'commerce_line_item' => array(
          'type' => 'commerce_line_item',
          'label' => t('Line Item'),
          'wrapped' => TRUE,
        ),
      ),
      'provides' => array(
        'commerce_attribute_adjustment' => array(
          'type' => 'decimal',
          'label' => t('Attribute Adjustment'),
        ),
      ),
      'group' => t('Commerce Line Item'),
    ),
  );
}

/**
 * Rule action callback.
 */
function commerce_pricing_attributes_calculate_adjusment($line_item_wrapper){
  $commerce_attribute_adjustment = 0;

  // Load the default currency code.
  $default_currency_code = commerce_default_currency();

  // Set the convert to currency.
  $covert_to_currency = $line_item_wrapper->commerce_unit_price->currency_code->value();

  $attributes = array();
  $commerce_product_fields = field_info_instances('commerce_product', $line_item_wrapper->commerce_product->type->value());
  foreach ($commerce_product_fields as $field_name => $field_instance) {
    $field = field_info_field($field_name);
    if($field['type'] == 'commerce_pricing_attributes'){
      $attributes[$field_name] = $line_item_wrapper->commerce_product->{$field_name}->value();
    }
  }

  if(!empty($attributes)){
    $commerce_option_list = commerce_option_load_by_line_item($line_item_wrapper->line_item_id->value());
    if(isset($line_item_wrapper->value()->data['commerce_option_list'])){
      $commerce_option_list = $line_item_wrapper->value()->data['commerce_option_list'];
    }

    if(!empty($commerce_option_list)){
      foreach($commerce_option_list as $commerce_option){
        if(!array_key_exists($commerce_option->field_name, $attributes)){
          continue;
        }
        $commerce_option_wrapper = entity_metadata_wrapper('commerce_option', $commerce_option);
        $attribute = $attributes[$commerce_option->field_name];
        $option_fields = field_info_instances('commerce_option', $commerce_option->set_id);
        foreach ($option_fields as $field_name => $field) {
          $selected_option = $commerce_option_wrapper->{$field_name}->value();

          // If selected option is not empty and field type support options.
          if(!empty($selected_option) and isset($attribute['set_details'][$field_name]['options'])){
            $selected_options = is_array($selected_option)?$selected_option:array($selected_option);

            // Iterate through selected attributes.
            foreach($selected_options as $selected_option){
              if(!isset($attribute['set_details'][$field_name]['options'][$selected_option]['currency_code'])){
                $attribute['set_details'][$field_name]['options'][$selected_option]['currency_code'] = $default_currency_code;
              }

              // echo $attribute['set_details'][$field_name]['options'][$selected_option]['price'];
              // echo $attribute['set_details'][$field_name]['options'][$selected_option]['currency_code'];
              // echo $covert_to_currency;
              
              if($attribute['set_details'][$field_name]['options'][$selected_option]['currency_code']!=$covert_to_currency){
              // $price = $attribute['set_details'][$field_name]['options'][$selected_option]['price'];
                $price = commerce_currency_convert($attribute['set_details'][$field_name]['options'][$selected_option]['price'], $attribute['set_details'][$field_name]['options'][$selected_option]['currency_code'], $covert_to_currency);
              }
              else{
                $price=$attribute['set_details'][$field_name]['options'][$selected_option]['price'];
              }

              if($price > 0){
                // If the attribute price must be calculated per order then
                // divide the attribue price with the quantity of the line item.
                if(isset($attribute['set_details'][$field_name]['options'][$selected_option]['calculate']) and $attribute['set_details'][$field_name]['options'][$selected_option]['calculate'] == 'per_order'){
                  $quantity = $line_item_wrapper->quantity->value();
                  $price = $price/$quantity;
                }

                switch ($attribute['set_details'][$field_name]['options'][$selected_option]['price_op']) {
                  case 'minus':
                    $commerce_attribute_adjustment -= $price;
                    break;
                  case 'plus':
                  default:
                    $commerce_attribute_adjustment += $price;
                    break;
                }
              }
            }
          }
        }
      }
    }
  }

  return array('commerce_attribute_adjustment' => $commerce_attribute_adjustment);
}
