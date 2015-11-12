<?php

class ImmoclientCustomerInlineEntityFormController extends EntityInlineEntityFormController{
    
   /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Seller'),
      'plural' => t('Sellers'),
    );
    return $labels;
  }
  

/**
 * Overrides EntityInlineEntityFormController::entityForm().
 */
public function entityForm($entity_form, &$form_state) {
  $info = entity_get_info($this->entityType);
  $entity = $entity_form['#entity'];


  $entity_form ['customer'] = array(
    '#title' => t('Buyer-Seller?'),
    '#type' => 'select',
    '#options' => array(
      1 => t('Seller')
    ),
    '#default_value' => isset($entity->customer) ? $entity->customer : '1',
    '#required' => TRUE,
    '#prefix' => '<div style="float:left; padding: 0 15px;min-height:100px;">',
    '#suffix' =>'</div>',
    '#access' =>FALSE,
  );
  $entity_form ['objekt_ref'] = array(
    '#title' => t('Reference number'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->objekt_ref) ? $entity->objekt_ref : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px;min-height:100px;">',
    '#suffix' =>'</div>',
    '#access' =>FALSE,
  );
  $entity_form ['salutation'] = array(
    '#title' => t('Salutation'),
    '#type' => 'select',
    '#options' => array(
      0 => t('Mr'),
      1 => t('Mrs')
    ),
    '#default_value' => isset($entity->salutation) ? $entity->salutation : '',
    '#required' => TRUE,
    '#prefix' => '<div style="padding: 0 15px;min-height:100px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['prename'] = array(
    '#title' => t('First name'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->prename) ? $entity->prename : '',
    '#size' => 30,
    '#required' => TRUE,
    '#prefix' => '<div style="clear:left; float:left;padding: 0 15px;min-height:100px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['name'] = array(
     '#title' => t('Name'),
     '#type' => 'textfield',
     '#default_value' => isset($entity->name) ? $entity->name : '',
     '#size' => 30,
     '#required' => TRUE,
     '#prefix' => '<div style="padding: 0 15px;min-height:100px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['customer_adress'] = array(
    '#type' => 'fieldset',
    '#title' => t('Address'),
    '#prefix' => '<div style="clear:left;">',
    '#suffix' =>'</div>'
    
  );
  /**
   * to avoid warnings you have to set parents, I honestly don't know why!
   * https://api.drupal.org/comment/48938#comment-48938
   */
  if (!isset($form['#parents'])) {
      $form['#parents'] = array();
    }
  
  //copy
  $field_name = 'field_customer_copy';
  $default_array = immoclient_customer_default_values_customer($entity,$field_name);
  if($default_array){
      $default = $default_array[0];
  }
  else{
      $default = 'NO';
  }
  $field = field_info_field('field_customer_copy');
  $instance = field_info_instance('immoclient_customer', 'field_customer_copy', 'immoclient_customer');
  $my_field = field_default_form('immoclient_customer', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_customer_copy']['#fieldset'] = 'customer_adress';
  $my_field['field_customer_copy']['und']['#default_value']= $default;
  $entity_form += (array) $my_field;

  //street, nbr
  $field_name = 'field_customer_strasse_hnr';
  $default_array = immoclient_customer_default_values_customer_text($entity,$field_name);
  if($default_array){
      $default = $default_array;
  }
  else{
      $default = '';
  }
  $field = field_info_field('field_customer_strasse_hnr');
  $instance = field_info_instance('immoclient_customer', 'field_customer_strasse_hnr', 'immoclient_customer');
  $my_field = field_default_form('immoclient_customer', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_customer_strasse_hnr']['#fieldset'] = 'customer_adress';
  $my_field['field_customer_strasse_hnr']['und'][0]['value']['#default_value']= $default;
  $entity_form += (array) $my_field;

  // PLC
  $field_name = 'field_customer_plz';
  $default = immoclient_customer_default_values_customer_text($entity,$field_name);
  $field = field_info_field($field_name);
  $instance = field_info_instance('immoclient_customer', $field_name, 'immoclient_customer');
  $my_field = field_default_form('immoclient_customer', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_customer_plz']['#fieldset'] = 'customer_adress';
  $my_field['field_customer_plz']['#prefix'] = '<div style="float:left; margin-right:10px;">';
  $my_field['field_customer_plz']['#suffix'] = '</div>';
  if ($default != ''){
    $my_field['field_customer_plz']['und'][0]['value']['#default_value']= $default;
  }
  $entity_form += (array) $my_field;

  // City
  $field_name = 'field_customer_stadt';
  $default = immoclient_customer_default_values_customer_text($entity,$field_name);
  $field = field_info_field($field_name);
  $instance = field_info_instance('immoclient_customer', $field_name, 'immoclient_customer'); 
  $my_field = field_default_form('immoclient_customer', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_customer_stadt']['#fieldset'] = 'customer_adress';
  if ($default != ''){
    $my_field['field_customer_stadt']['und'][0]['value']['#default_value']= $default;
  }
  $entity_form += (array) $my_field;
  
  // Country
  $field_name = 'field_customer_land';
  $default = immoclient_customer_default_values_customer_text($entity,$field_name);
  $field = field_info_field($field_name);
  $instance = field_info_instance('immoclient_customer', $field_name, 'immoclient_customer');
  $my_field = field_default_form('immoclient_customer', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_customer_land']['#fieldset'] = 'customer_adress';
  if ($default != ''){
    $my_field['field_customer_land']['und'][0]['value']['#default_value']= $default;
  }
  $entity_form += (array) $my_field;
  
  // Tel privat
  $field_name = 'field_customer_tel_privat';
  $default = immoclient_customer_default_values_customer_text($entity,$field_name);
  $field = field_info_field($field_name);
  $instance = field_info_instance('immoclient_customer', $field_name, 'immoclient_customer');
  $my_field = field_default_form('immoclient_customer', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_customer_tel_privat']['#fieldset'] = 'customer_adress';
  if ($default != ''){
    $my_field['field_customer_tel_privat']['und'][0]['value']['#default_value']= $default;
  }
  $entity_form += (array) $my_field;
  
  // Tel mobil
  $field_name = 'field_customer_tel_mobil';
  $default = immoclient_customer_default_values_customer_text($entity,$field_name);
  $field = field_info_field($field_name);
  $instance = field_info_instance('immoclient_customer', $field_name, 'immoclient_customer');
  $my_field = field_default_form('immoclient_customer', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_customer_tel_mobil']['#fieldset'] = 'customer_adress';
  if ($default != ''){
    $my_field['field_customer_tel_mobil']['und'][0]['value']['#default_value']= $default;
  }
  $entity_form += (array) $my_field;

  // Fax
  $field_name = 'field_customer_fax';
  $default = immoclient_customer_default_values_customer_text($entity,$field_name);
  $field = field_info_field($field_name);
  $instance = field_info_instance('immoclient_customer', $field_name, 'immoclient_customer');
  $my_field = field_default_form('immoclient_customer', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_customer_fax']['#fieldset'] = 'customer_adress';
  if ($default != ''){
    $my_field['field_customer_fax']['und'][0]['value']['#default_value']= $default;  
  }
  $entity_form += (array) $my_field;

  // Email
  $field_name = 'field_customer_email';
  $default = immoclient_customer_default_values_customer_text($entity,$field_name);
  $field = field_info_field($field_name);
  $instance = field_info_instance('immoclient_customer', $field_name, 'immoclient_customer');
  $my_field = field_default_form('immoclient_customer', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_customer_email']['#fieldset'] = 'customer_adress';
  if ($default != ''){
    $my_field['field_customer_email']['und'][0]['value']['#default_value']= $default;
  }
  $entity_form += (array) $my_field;
  return $entity_form;
  }
}




  
 
  