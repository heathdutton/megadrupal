<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class ImmoclientDistanceInlineEntityFormController extends EntityInlineEntityFormController{
    
   /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Distance to important places'),
      'plural' => t('Distances to important places'),
    );
    return $labels;
  }
  

/**
 * Overrides EntityInlineEntityFormController::entityForm().
 */
 public function entityForm($entity_form, &$form_state) {
    $info = entity_get_info($this->entityType);
    $entity = $entity_form['#entity'];
   
    $entity_form['distanz_zu'] = array(
      '#title' => t('Distance to?'),
      '#type' => 'select',
      '#options' => array(
        'FLUGHAFEN'=>t('Airport'),
        'FERNBAHNHOF' => t('Long distance station'),
        'AUTOBAHN' =>t('Autobahn'),
        'US_BAHN'=>t('Tube'),
        'BUS'=>t('Bus'),
        'KINDERGAERTEN'=>t('Kindergarten'),
        'GRUNDSCHULE'=>t('Primary School'),
        'HAUPTSCHULE'=>t('Secondary General School'),
        'REALSCHULE'=>t('Intermediate Secondary School'),
        'GESAMTSCHULE'=>t('Comprehensive Schools'),
        'GYMNASIUM'=>t('Grammar Schools'),
        'ZENTRUM'=>t('Centre'),
        'EINKAUFSMOEGLICHKEITEN'=>t('Shopping facilities'),
        'GASTSTAETTEN'=>t('Pubs')
          ),
      '#default_value' => isset($entity->distanz_zu) ? $entity->distanz_zu : '',
      '#required' => TRUE,
     '#prefix' => '<div style="float:left; padding: 0 15px">',
     '#suffix' =>'</div>',
    );
    $entity_form['distance_measure'] = array(
      '#title' => t('Distance'),
      '#description' => t('Only integer for km'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 0,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#default_value' => isset($entity->distance_measure) ? $entity->distance_measure : '',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px">',
      '#suffix' =>'</div>',    
    );
    return $entity_form;
  }
}
