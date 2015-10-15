<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class ImmoclientDistancesportInlineEntityFormController extends EntityInlineEntityFormController{
    
   /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Distance to sport places'),
      'plural' => t('Distances to sport places'),
    );
    return $labels;
  }
  

/**
 * Overrides EntityInlineEntityFormController::entityForm().
 */
 public function entityForm($entity_form, &$form_state) {
    $info = entity_get_info($this->entityType);
    $entity = $entity_form['#entity'];
   
    $entity_form['distanz_zu_sport'] = array(
      '#title' => t('Distance to?'),
      '#type' => 'select',
      '#options' => array(
        'STRAND'=>t('Beach'),
        'SEE' => t('Lake'),
        'MEER' =>t('Sea'),
        'SKIGEBIET'=>t('Ski resort'),
        'SPORTANLAGEN'=>t('Sports facilities'),
        'WANDERGEBIETE'=>t('Hiking area'),
        'NAHERHOLUNG'=>t('Local recreation'),
        ),
      '#default_value' => isset($entity->distanz_zu_sport) ? $entity->distanz_zu_sport : '',
      '#required' => TRUE,
     '#prefix' => '<div style="float:left; padding: 0 15px">',
     '#suffix' =>'</div>',
    );
    $entity_form['distancesport_measure'] = array(
      '#title' => t('Distance'),
      '#description' => t('Only integer for km'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 0,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#default_value' => isset($entity->distancesport_measure) ? $entity->distancesport_measure : '',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px">',
      '#suffix' =>'</div>',    
    );
    return $entity_form;
  }
}
