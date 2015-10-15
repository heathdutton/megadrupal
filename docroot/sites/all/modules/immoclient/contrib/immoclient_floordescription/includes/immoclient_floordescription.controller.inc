<?php

class ImmoclientFloordescriptionInlineEntityFormController extends EntityInlineEntityFormController{
    
   /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Description / floor'),
      'plural' => t('Descriptions / floor'),
    );
    return $labels;
  }
  

/**
 * Overrides EntityInlineEntityFormController::entityForm().
 */
 public function entityForm($entity_form, &$form_state) {
    $info = entity_get_info($this->entityType);
    $entity = $entity_form['#entity'];
    
    $entity_form ['geschoss'] = array(
      '#title' => t('Floor?'),
      '#type' => 'select',
      '#weight' => 1,
      '#options' => array(
      'BASEMENT'=>t('BASEMENT'),
      'FIRST_FLOOR' => t('First Floor'), 
      'SECOND_FLOOR' =>t('Second Floor'),
      'THIRD_FLOOR' => t('Third Floor'),
      'FOURTH_FLOOR' => t('Fourth Floor'),
      'FIFTH_FLOOR' => t('Fifth Floor'),
      'SIXTH_FLOOR' => t('Sixth Floor'),
      'SEVENTH_FLOOR' => t('Seventh Floor'),
      'EIGHTTH_FLOOR' => t('Eighth Floor'),
      'NINTH_FLOOR' => t('Ninth Floor'),
      'ATTIC' => t('Attic')
      ),
      '#default_value' => isset($entity->geschoss) ? $entity->geschoss : '0',
    //  '#fieldset' => 'preis',
      '#required' => TRUE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>',
        );
    $entity_form ['floordescription'] = array(
    '#title' => t('Furnishing'),
    '#description' =>t('Description of chambers etc.'),
    '#type' => 'textfield',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
  // '#fieldset' => 'furn_media',
    '#default_value' => isset($entity->floordescription) ? $entity->floordescription : '',
    '#size' => 32,
    '#weight' => 2, 
    '#maxlength' => 128, 
    '#required' => TRUE,
     '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
     '#suffix' =>'</div>',   
    );
    
    return $entity_form;
 }
}

