<?php

class ImmoclientfurnishInlineEntityFormController extends EntityInlineEntityFormController{
    
   /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Immo furnishing'),
      'plural' => t('Immo furnishing'),
    );
    return $labels;
  }
  

/**
 * Overrides EntityInlineEntityFormController::entityForm().
 */
public function entityForm($entity_form, &$form_state) {
  $info = entity_get_info($this->entityType);
  $entity = $entity_form['#entity'];

  $entity_form['furnished'] = array(
     '#type' => 'vertical_tabs',       
  );
  $entity_form ['furn_basis'] = array(
    '#type' => 'fieldset',
    '#title' => t('House-Appartment-Basics'),
    '#weight' => 2,
    '#description' =>t('BATH, KITCHEN, FLOOR'),
    '#group' => 'field_ausstattung][und][form][furnished',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes'=> array('class' => array('vertical-tabs-pane')),        
  );
  $entity_form ['furn_zus_wohn_haus'] = array(
    '#type' => 'fieldset',
    '#title' => t('House-Appartment-Addition'),
    '#weight' => 3,
    '#description' =>t('SAUNA, SWIMMINGPOOL, GARDEN USAGE '),
    '#group' => 'field_ausstattung][und][form][furnished',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes'=> array('class' => array('vertical-tabs-pane')),
  );
  $entity_form ['furn_waerme'] = array(
    '#type' => 'fieldset',
    '#title' => t('Heating - Conditioned'),
    '#weight' => 4,
    '#description' =>t('CHIMNEY, HEATING SORT, LIGHTING, AIR CONDITIONED'),
    '#group' => 'field_ausstattung][und][form][furnished',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes'=> array('class' => array('vertical-tabs-pane')),
  );
   $entity_form ['furn_media'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media'),
    '#weight' => 5,
    '#description' =>t('INTERNET,TV'),
    '#group' => 'field_ausstattung][und][form][furnished',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes'=> array('class' => array('vertical-tabs-pane')),
  );
  $entity_form ['furn_raeume'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional rooms'),
    '#weight' => 6,
    '#description' =>t('ATTIC, COMPANY WC, CELLARED, STOREROOM'),
    '#group' => 'field_ausstattung][und][form][furnished',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes'=> array('class' => array('vertical-tabs-pane')),
  );
  $entity_form ['furn_alter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Senior-Handicapped'),
    '#weight' => 7,
    '#description' =>t('BARRIER-FREE, RAMP, LIFT,WHEELCHAIR SUITABLE'),
    '#group' => 'field_ausstattung][und][form][furnished',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes'=> array('class' => array('vertical-tabs-pane')),
  );
  $entity_form ['furn_gew'] = array(
    '#type' => 'fieldset',
    '#title' => t('Business'),
    '#weight' => 8,
    '#description' =>t('CRANE, CAFETERIA, SAFETY TECHNOLOGY, POWER CONNECTION VALUE'),
    '#group' => 'field_ausstattung][und][form][furnished',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes'=> array('class' => array('vertical-tabs-pane')),
  );
  $entity_form ['furn_gastro'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gastro'),
    '#weight' => 9,
    '#description' =>t('PATIOS FOR GUESTS, AFFILIATED GASTRONOMY'),
    '#group' => 'field_ausstattung][und][form][furnished',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes'=> array('class' => array('vertical-tabs-pane')),
  );
  $entity_form ['ausstatt_kategorie'] = array(
    '#title' => t('Category?'),
    '#type' => 'radios',
    '#weight' => 1,
    '#options' => array('none' => t('none'),'STANDARD'=> t('Standard'), 'GEHOBEN' => t('Upmarket'), 'LUXUS' =>t('Luxury') ),
    '#default_value' => isset($entity->ausstatt_kategorie) ? $entity->ausstatt_kategorie : 'none',
    '#fieldset' => 'furn_basis',
    '#required' => FALSE,
    '#prefix' => '<div class="category_furn">',
    '#suffix' =>'</div>'
  );
  $entity_form ['raeume_veraenderbar'] = array(
    '#title' => t('Rooms changeable?'),
    '#type' => 'radios',
    '#weight' => 2,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->raeume_veraenderbar) ? $entity->raeume_veraenderbar : '0',
    '#fieldset' => 'furn_zus_wohn_haus',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['wg_geeignet'] = array(
    '#title' => t('Flatshare possible?'),
    '#type' => 'radios',
    '#weight' => 2,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->wg_geeignet) ? $entity->wg_geeignet : '0',
    '#fieldset' => 'furn_zus_wohn_haus',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  /***  Start checkbox  ***/
  $field_name = 'field_bad';
  $default = immoclient_furnishing_default_values($entity,$field_name);
  if(!$default){
    $default = array();
  }    
  $field = field_info_field('field_bad');
  $instance = field_info_instance('immofurnish', 'field_bad', 'immofurnish');    
  $my_field = field_default_form('immofurnish', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);        
  $my_field['field_bad']['und']['#default_value']= $default;
  $my_field['field_bad']['#weight'] = 3;
  $my_field['field_bad']['#fieldset'] = 'furn_basis';
  $entity_form += (array) $my_field;

  /***  End of checkbox  ***/
    
  /***  Start checkbox  ***/

  $field_name = 'field_kueche';
  $default = immoclient_furnishing_default_values($entity,$field_name);
  if(!$default){
    $default = array();
  }
  $field = field_info_field('field_kueche');
  $instance = field_info_instance('immofurnish', 'field_kueche', 'immofurnish');
  $my_field = field_default_form('immofurnish', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_kueche']['und']['#default_value']= $default;
  $my_field['field_kueche']['#weight'] = 4;
  $my_field['field_kueche']['#fieldset'] = 'furn_basis';
  $entity_form += (array) $my_field;

  /***  End of checkbox  ***/
  
  /***  Start checkbox  ***/
  $field_name = 'field_boden';
  $default = immoclient_furnishing_default_values($entity,$field_name);
  if(!$default){
    $default = array();
  }
  $field = field_info_field('field_boden');
  $instance = field_info_instance('immofurnish', 'field_boden', 'immofurnish');
  $my_field = field_default_form('immofurnish', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_boden']['und']['#default_value']= $default;
  $my_field['field_boden']['#weight'] = 5;
  $my_field['field_boden']['#fieldset'] = 'furn_basis';
  $entity_form += (array) $my_field;

  /***  End of checkbox  ***/
  $entity_form ['kamin'] = array(
    '#title' => t('Chimney available?'),
    '#type' => 'radios',
    '#weight' => 6,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->kamin) ? $entity->kamin : '0',
    '#fieldset' => 'furn_waerme',
    '#required' => FALSE,
  );
  
/***  Start checkbox  ***/

  $field_name = 'field_heizungsart';
  $default = immoclient_furnishing_default_values($entity,$field_name);
  if(!$default){
    $default = array();
  }
  $field = field_info_field('field_heizungsart');
  $instance = field_info_instance('immofurnish', 'field_heizungsart', 'immofurnish');
  $my_field = field_default_form('immofurnish', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_heizungsart']['und']['#default_value']= $default;
  $my_field['field_heizungsart']['#weight'] = 7;
  $my_field['field_heizungsart']['#fieldset'] = 'furn_waerme';
  $entity_form += (array) $my_field;

  /***  End of checkbox  ***/
   /***  Start checkbox  ***/

  $field_name = 'field_befeuerung';
  $default = immoclient_furnishing_default_values($entity,$field_name);
  if(!$default){
    $default = array();
  }
  $field = field_info_field('field_befeuerung');
  $instance = field_info_instance('immofurnish', 'field_befeuerung', 'immofurnish');
  $my_field = field_default_form('immofurnish', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_befeuerung']['und']['#default_value']= $default;
  $my_field['field_befeuerung']['#weight'] = 8;
  $my_field['field_befeuerung']['#fieldset'] = 'furn_waerme';
  $entity_form += (array) $my_field;

  /***  End of checkbox  ***/
  $entity_form ['klimatisiert'] = array(
    '#title' => t('Air conditioned?'),
    '#type' => 'radios',
    '#weight' => 9,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->klimatisiert) ? $entity->klimatisiert : '0',
    '#fieldset' => 'furn_waerme',
    '#required' => FALSE,
  );

  /***  Start checkbox  ***/

  $field_name = 'field_fahrstuhl';
  $default = immoclient_furnishing_default_values($entity,$field_name);
  if(!$default){
    $default = array();
  }
  $field = field_info_field('field_fahrstuhl');
  $instance = field_info_instance('immofurnish', 'field_fahrstuhl', 'immofurnish');
  $my_field = field_default_form('immofurnish', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
  $my_field['field_fahrstuhl']['und']['#default_value']= $default;
  $my_field['field_fahrstuhl']['#weight'] = 10;
  $my_field['field_fahrstuhl']['#fieldset'] = 'furn_zus_wohn_haus';
  $entity_form += (array) $my_field;
  /***  End of checkbox  ***/
  
  $entity_form ['gartennutzung'] = array(
    '#title' => t('Garden sharing?'),
    '#type' => 'radios',
    '#weight' => 11,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->gartennutzung) ? $entity->gartennutzung : '0',
    '#fieldset' => 'furn_zus_wohn_haus',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  
  /***  Start checkbox  ***/

  $field_name = 'field_ausricht_balkon_terrasse';
  $default = immoclient_furnishing_default_values($entity,$field_name);
  if(!$default){
    $default = array();
  }

  $field = field_info_field('field_ausricht_balkon_terrasse');
  $instance = field_info_instance('immofurnish', 'field_ausricht_balkon_terrasse', 'immofurnish');    
  $my_field = field_default_form('immofurnish', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);    
  $my_field['field_ausricht_balkon_terrasse']['und']['#default_value']= $default;
  $my_field['field_ausricht_balkon_terrasse']['#weight'] = 12;
  $my_field['field_ausricht_balkon_terrasse']['#fieldset'] = 'furn_basis';
  $entity_form += (array) $my_field;
  /***  End of checkbox  ***/

  $entity_form ['moeb'] = array(
    '#title' => t('Furnitured?'),
    '#type' => 'select',
    '#weight' => 13,
    '#options' => array('none'=>t('none'),'TEIL' => t('Partial'), 'VOLL' => t('Complete')),
    '#default_value' => isset($entity->moeb) ? $entity->moeb : 'none',
    '#fieldset' => 'furn_zus_wohn_haus',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['rollstuhlgerecht'] = array(
    '#title' => t('Handicapped accessible?'),
    '#type' => 'radios',
    '#weight' => 14,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->rollstuhlgerecht) ? $entity->rollstuhlgerecht : '0',
    '#fieldset' => 'furn_alter',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['barrierefrei'] = array(
    '#title' => t('Barrier-free?'),
    '#type' => 'radios',
    '#weight' => 15,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->barrierefrei) ? $entity->barrierefrei : '0',
    '#fieldset' => 'furn_alter',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['kabel_sat_tv'] = array(
    '#title' => t('Cable-SAT-TV available?'),
    '#type' => 'radios',
    '#weight' => 16,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->kabel_sat_tv) ? $entity->kabel_sat_tv : '0',
    '#fieldset' => 'furn_media',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['dvbt'] = array(
    '#title' => t('DVBT available?'),
    '#type' => 'radios',
    '#weight' => 17,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->dvbt) ? $entity->dvbt : '0',
    '#fieldset' => 'furn_media',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['sauna'] = array(
    '#title' => t('Sauna?'),
    '#type' => 'radios',
    '#weight' => 18,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->sauna) ? $entity->sauna : '0',
    '#fieldset' => 'furn_zus_wohn_haus',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['swimmingpool'] = array(
    '#title' => t('Swimmingpool?'),
    '#type' => 'radios',
    '#weight' => 19,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->swimmingpool) ? $entity->swimmingpool : '0',
    '#fieldset' => 'furn_zus_wohn_haus',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['wasch_trockenraum'] = array(
    '#title' => t('Laundry room?'),
    '#type' => 'radios',
    '#weight' => 20,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->wasch_trockenraum) ? $entity->wasch_trockenraum : '0',
    '#fieldset' => 'furn_raeume',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['wintergarten'] = array(
    '#title' => t('Winter garden?'),
    '#type' => 'radios',
    '#weight' => 21,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->wintergarten) ? $entity->wintergarten : '0',
    '#fieldset' => 'furn_raeume',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['dv_verkabelung'] = array(
    '#title' => t('DV wiring?'),
    '#type' => 'radios',
    '#weight' => 22,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->dv_verkabelung) ? $entity->dv_verkabelung : '0',
    '#fieldset' => 'furn_media',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['rampe'] = array(
    '#title' => t('Ramp?'),
    '#type' => 'radios',
    '#weight' => 23,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->rampe) ? $entity->rampe : '0',
    '#fieldset' => 'furn_alter',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['hebebuehne'] = array(
    '#title' => t('Hydraulic lift?'),
    '#type' => 'radios',
    '#weight' => 24,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->hebebuehne) ? $entity->hebebuehne : '0',
    '#fieldset' => 'furn_alter',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['kran'] = array(
    '#title' => t('Crane?'),
    '#type' => 'radios',
    '#weight' => 25,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->kran) ? $entity->kran : '0',
    '#fieldset' => 'furn_gew',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['gastterrasse'] = array(
    '#title' => t('Patio for guests?'),
    '#type' => 'radios',
    '#weight' => 26,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->gastterrasse) ? $entity->gastterrasse : '0',
    '#fieldset' => 'furn_gastro',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['stromanschlusswert'] = array(
    '#title' => t('Power connection value'),
    '#type' => 'numericfield',
    '#precision' => 4,
    '#decimals' => 0,
    '#minimum' => 0,
    '#weight' => 26,
    '#maximum' => 50000000.99,
    '#fieldset' => 'furn_gew',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->stromanschlusswert) ? $entity->stromanschlusswert : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['kantine_cafeteria'] = array(
    '#title' => t('Canteen - Cafeteria?'),
    '#type' => 'radios',
    '#weight' => 27,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->kantine_cafeteria) ? $entity->kantine_cafeteria : '0',
    '#fieldset' => 'furn_gew',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['teekueche'] = array(
    '#title' => t('Tea kitchen?'),
    '#type' => 'radios',
    '#weight' => 28,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->teekueche) ? $entity->teekueche : '0',
    '#fieldset' => 'furn_raeume',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['hallenhoehe'] = array(
    '#title' => t('Hall height'),
    '#type' => 'numericfield',
    '#precision' => 4,
    '#decimals' => 0,
    '#minimum' => 0,
    '#weight' => 29,
    '#maximum' => 50000000.99,
    '#fieldset' => 'furn_gew',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->hallenhoehe) ? $entity->hallenhoehe : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  /***  Start checkbox  ***/

  $field_name = 'field_angeschl_gastronomie';
  $default = immoclient_furnishing_default_values($entity,$field_name);
  if(!$default){
      $default = array();
  }    
  $field = field_info_field('field_angeschl_gastronomie');
  $instance = field_info_instance('immofurnish', 'field_angeschl_gastronomie', 'immofurnish');    
  $my_field = field_default_form('immofurnish', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);    
  $my_field['field_angeschl_gastronomie']['und']['#default_value']= $default;
  $my_field['field_angeschl_gastronomie']['#weight'] = 30; 
  $my_field['field_angeschl_gastronomie']['#fieldset'] = 'furn_gastro';
  $entity_form += (array) $my_field;
/***  End of checkbox  ***/
  
  $entity_form ['brauereibindung'] = array(
    '#title' => t('Commitment to brewery?'),
    '#type' => 'radios',
    '#weight' => 31,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->brauereibindung) ? $entity->brauereibindung : '0',
    '#fieldset' => 'furn_gastro',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['sporteinrichtungen'] = array(
    '#title' => t('Sport facilities?'),
    '#type' => 'radios',
    '#weight' => 32,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->sporteinrichtungen) ? $entity->sporteinrichtungen : '0',
    '#fieldset' => 'furn_zus_wohn_haus',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['wellnessbereich'] = array(
    '#title' => t('Wellness area?'),
    '#type' => 'radios',
    '#weight' => 33,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->wellnessbereich) ? $entity->wellnessbereich : '0',
    '#fieldset' => 'furn_zus_wohn_haus',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  
  /***  Start checkbox  ***/

  $field_name = 'field_serviceleistungen';
  $default = immoclient_furnishing_default_values($entity,$field_name);
  if(!$default){
    $default = array();
  }    
  $field = field_info_field('field_serviceleistungen');
  $instance = field_info_instance('immofurnish', 'field_serviceleistungen', 'immofurnish');    
  $my_field = field_default_form('immofurnish', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);    
  $my_field['field_serviceleistungen']['und']['#default_value']= $default;
  $my_field['field_serviceleistungen']['#weight'] = 50;
  $my_field['field_serviceleistungen']['#fieldset'] = 'furn_alter';
  $entity_form += (array) $my_field;

  /***  End of checkbox  ***/
    
  $entity_form ['telefon_ferienimmobilie'] = array(
    '#title' => t('Telephone at holiday object?'),
    '#type' => 'radios',
    '#weight' => 35,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->telefon_ferienimmobilie) ? $entity->telefon_ferienimmobilie : '0',
    '#fieldset' => 'furn_media',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['breitband_zugang'] = array(
    '#title' => t('Broadband access?'),
    '#type' => 'radios',
    '#weight' => 35,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->breitband_zugang) ? $entity->breitband_zugang : '0',
    '#fieldset' => 'furn_media',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['art'] = array(
    '#title' => t('Kind of broadband access'),
    '#description' =>t('DSL,SDSL, VDSL, ADSL, SKYDSL, IPTV?'),
    '#type' => 'textfield',
    '#fieldset' => 'furn_media',
    '#default_value' => isset($entity->art) ? $entity->art : '',
    '#size' => 10,
    '#weight' => 36, 
    '#maxlength' => 128, 
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 10px 15px">',
    '#suffix' =>'</div>',   
  );
  $entity_form ['speed'] = array(
    '#title' => t('Speed of broadband access'),
    '#description' =>t('Speed in Mbit/Sec'),
    '#type' => 'textfield',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#fieldset' => 'furn_media',
    '#default_value' => isset($entity->speed) ? $entity->speed : '',
    '#size' => 10,
    '#weight' => 37,
    '#maxlength' => 128, 
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 10px 15px">',
    '#suffix' =>'</div>',   
    );
  $entity_form ['umts_empfang'] = array(
    '#title' => t('UMTS access?'),
    '#type' => 'radios',
    '#weight' => 38,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->umts_empfang) ? $entity->umts_empfang : '0',
    '#fieldset' => 'furn_media',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  
   /***  Start checkbox  ***/

  $field_name = 'field_sicherheitstechnik';
  $default = immoclient_furnishing_default_values($entity,$field_name);
  if(!$default){
    $default = array();
  }
  $field = field_info_field('field_sicherheitstechnik');
  $instance = field_info_instance('immofurnish', 'field_sicherheitstechnik', 'immofurnish');    
  $my_field = field_default_form('immofurnish', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);    
  $my_field['field_sicherheitstechnik']['und']['#default_value']= $default;
  $my_field['field_sicherheitstechnik']['#weight'] = 39;
  $my_field['field_sicherheitstechnik']['#fieldset'] = 'furn_gew';
  $entity_form += (array) $my_field;

  /***  End of checkbox  ***/
  
  $entity_form ['unterkellert'] = array(
    '#title' => t('Build with a cellar?'),
    '#type' => 'radios',
    '#weight' => 48,
    '#options' => array(0 => t('No'), 1 => t('Yes'),2 => t('Partial')),
    '#default_value' => isset($entity->unterkellert) ? $entity->unterkellert : '0',
    '#fieldset' => 'furn_raeume',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['abstellraum'] = array(
    '#title' => t('Store room?'),
    '#type' => 'radios',
    '#weight' => 41,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->abstellraum) ? $entity->abstellraum : '0',
    '#fieldset' => 'furn_raeume',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['fahrradraum'] = array(
    '#title' => t('Bike room?'),
    '#type' => 'radios',
    '#weight' => 42,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->fahrradraum) ? $entity->fahrradraum : '0',
    '#fieldset' => 'furn_raeume',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['rolladen'] = array(
    '#title' => t('Shutter?'),
    '#type' => 'radios',
    '#weight' => 43,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->rolladen) ? $entity->rolladen : '0',
    '#fieldset' => 'furn_zus_wohn_haus',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['bibliothek'] = array(
    '#title' => t('Library?'),
    '#type' => 'radios',
    '#weight' => 44,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->bibliothek) ? $entity->bibliothek : '0',
    '#fieldset' => 'furn_zus_wohn_haus',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['dachboden'] = array(
    '#title' => t('Attic?'),
    '#type' => 'radios',
    '#weight' => 45,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->dachboden) ? $entity->dachboden : '0',
    '#fieldset' => 'furn_raeume',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
    $entity_form ['gaestewc'] = array(
    '#title' => t('Guest toilet?'),
    '#type' => 'radios',
    '#weight' => 46,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->gaestewc) ? $entity->gaestewc : '0',
    '#fieldset' => 'furn_raeume',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['kabelkanaele'] = array(
    '#title' => t('Cable duct?'),
    '#type' => 'radios',
    '#weight' => 47,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->kabelkanaele) ? $entity->kabelkanaele : '0',
    '#fieldset' => 'furn_media',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
  $entity_form ['seniorengerecht'] = array(
    '#title' => t('For elderly?'),
    '#type' => 'radios',
    '#weight' => 48,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => isset($entity->seniorengerecht) ? $entity->seniorengerecht : '0',
    '#fieldset' => 'furn_alter',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
    '#suffix' =>'</div>'
  );
    
  return $entity_form;
  } 
}