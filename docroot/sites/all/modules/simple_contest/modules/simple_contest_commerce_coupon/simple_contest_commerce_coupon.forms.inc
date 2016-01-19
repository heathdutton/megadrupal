<?php

function _simple_contest_commerce_coupon_form_simple_contest_content_type_settings_form_alter( &$form, &$form_state ) {
  
  if ( !$form['is_sc']['#default_value'] ) {
    return;
  }
  
  $type = $form['type']['#value'];

  $settings = $form['#contest_settings'];
  
  $commerce_coupon = isset($form_state['values']['commerce_coupon']) 
    ?  $form_state['values']['commerce_coupon'] 
    :  isset($settings['commerce_coupon'])?$settings['commerce_coupon']:array();
  
  $coupon_values = isset($commerce_coupon['coupon_values'])?$commerce_coupon['coupon_values']:array();
  
  $form['sc']['email']['token_help']['help']['#token_types'][] = 'commerce_coupon';
  
  $form['sc']['commerce_coupon'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#title'       => t('Prize coupons for contest winners'),
    '#group'       => 'additional_settings',
    '#tree'   => TRUE,
  );
  
  
  
  $coupon_types = array();
  foreach (commerce_coupon_get_types() as $type => $info) {
    $coupon_types[$type] = $info->label;
  }
  
  $form['sc']['commerce_coupon']['create_coupon'] = array(
    '#type'   => 'checkbox',
    '#title'  => t('Enable prize coupon for winners'),
    '#return_value' => 1,
    '#default_value' => isset($commerce_coupon['create_coupon'])?$commerce_coupon['create_coupon']:0,
    '#description' => t('Select coupon type.'),
  );
  
  /*
  $form['sc']['commerce_coupon']['coupon_entity'] = array(
    '#type'   => 'select',
    '#title'  => t('Coupon type'),
    '#options' => $coupon_types,
    '#default_value' => isset($commerce_coupon['coupon_entity'])?$commerce_coupon['coupon_entity']:'',
    '#description' => t('Select coupon type.'),
  );
  */
  
  $form['sc']['commerce_coupon']['coupon_values'] = array(
    '#type'   => 'markup',
    '#prefix' => '<div id="simple_contest_commerce_coupon_wrapper">',
    '#suffix' => '</div>',
    '#tree'   => TRUE,
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );

  $max_winners = isset($form_state['values']['max_winners'])
  ?  $form_state['values']['max_winners']
  :  $settings['max_winners'];
  
  $old_title = drupal_get_title();

  // get the entity form
  module_load_include('inc', 'commerce_coupon_ui', 'includes/commerce_coupon_ui.forms');
  
  $entity_type = 'commerce_coupon';
  $bundle_name = 'sc_commerce_coupon';
  
  $instances = field_info_instances($entity_type, $bundle_name);
  
  //print_r($coupon_values);
  //exit();
  $exclude_fields = array('commerce_coupon_code', 'sc_commerce_coupon_user', 'sc_commerce_coupon_contest');
  
  for ( $i=1; $i<= $max_winners; $i++ ) {
    $form['sc']['commerce_coupon']['coupon_values']['coupon_' . $i] = array(
      '#type'          => 'fieldset',
      //'#title'         => t('Coupon value for Rank #!rank', array('!rank' => $i)), // always hidden?
      '#description' => '<label>' . t('Coupon value for Rank #!rank', array('!rank' => $i)) . '</label>' .
        t('Leaving this field as empty or zero will ignore the coupon creation for rank #!rank.', array('!rank' => $i)),
      '#collapsible' => FALSE,
      '#collapsed'   => FALSE,
      '#tree'   => TRUE,
      '#parents' => array(
        'commerce_coupon',
        'coupon_values',
        'coupon_' . $i,
      ),
    );
    
    $coupon = isset($coupon_values['coupon_' . $i])?$coupon_values['coupon_' . $i]:array();
    //$coupon['type'] = $bundle_name;
    
    $entity = entity_create( $entity_type, $coupon );
    
    foreach( $instances as $field_name => $field ) {
      
      if ( in_array($field_name, $exclude_fields) ) {
        continue;
      }
      
      $instance = field_info_instance ( $entity_type, $field_name, $bundle_name );
      
      if (isset($coupon[$field_name]['und'])) {
        $instance['default_value'] = $coupon[$field_name]['und'];
      }
      
      $field_obj = field_info_field($field_name);
      $items = $instance['default_value'];    
      
      $field_form = field_default_form( $entity_type, NULL, $field_obj, $instance, LANGUAGE_NONE, $items, $form, $form_state, 0 );
      $form['sc']['commerce_coupon']['coupon_values']['coupon_' . $i] += $field_form;
      
    }
  }
  
  drupal_set_title($old_title);
}
