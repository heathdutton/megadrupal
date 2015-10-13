<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class ImmoclientParkingInlineEntityFormController extends EntityInlineEntityFormController{
    
   /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Parking Place'),
      'plural' => t('Parking Places'),
    );
    return $labels;
  }
  

/**
 * Overrides EntityInlineEntityFormController::entityForm().
 */
 public function entityForm($entity_form, &$form_state) {
    $info = entity_get_info($this->entityType);
    $entity = $entity_form['#entity'];
   
    $entity_form['platzart'] = array(
      '#type' => 'select',
      '#title' => t('Parking space'),
      '#options' => array(
        'SONSTIGE'=>t('Other'),
        'CARPORT' => t('Carport'),
        'DUPLEX' =>t('Duplex'),
        'FREIPLATZ'=>t('Free space'),
        'GARAGE'=>t('Garage'),
        'PARKHAUS'=>t('Parking deck'),
        'TIEFGARAGE'=>t('Basement garage'),
        ),
      '#default_value' => isset($entity->platzart) ? $entity->platzart : 'SONSTIGES',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px">',
      '#suffix' =>'</div>',
    );
    
    $entity_form['anzahl'] = array(
      '#title' => t('Number of spaces'),
      '#type' => 'textfield',
      '#default_value' => isset($entity->anzahl) ? $entity->anzahl : '1',
      '#size' => 3,
      '#maxlength' => 3, 
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px">',
      '#suffix' =>'</div>',   
    );
    
    return $entity_form;
  }
}
