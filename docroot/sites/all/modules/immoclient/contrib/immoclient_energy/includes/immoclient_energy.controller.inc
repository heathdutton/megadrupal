<?php

class ImmoclientEnergyInlineEntityFormController extends EntityInlineEntityFormController{
    
   /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Immo energy'),
      'plural' => t('Immo energies'),
    );
    return $labels;
  }
  

/**
 * Overrides EntityInlineEntityFormController::entityForm().
 */
 public function entityForm($entity_form, &$form_state) {
    $info = entity_get_info($this->entityType);
    $entity = $entity_form['#entity'];
    
    $entity_form ['ausweis'] = array(
      '#title' => t('Passport?'),
      '#type' => 'radios',
      '#weight' => 1,
      '#options' => array(0 => t('Available'),1=> t('Not yet available')),
      '#default_value' => isset($entity->ausweis) ? $entity->ausweis : '0',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>',
        );
    $entity_form ['ener_base'] = array(
        '#type' => 'fieldset',
        '#title' => t('Available'),
        '#weight' => 2,
        '#prefix' => '<div style="clear:left;">',
        '#suffix' =>'</div>',
        '#states' => array(
        'visible' => array(
        ':input[name="field_energieausweis_reference[und][form][ausweis]"]' => 
          array('value' => 0), 
        ),
      ),
    );
    $entity_form ['epart'] = array(
      '#title' => t('Calculation basis?'),
      '#type' => 'select',
      '#weight' => -1,
      '#options' => array(
        '0' => t('none'), 
        'VERBRAUCH'=> t('Consumption'),
        'BEDARF'=> t('Demand')
        ),
      '#default_value' => isset($entity->epart) ? $entity->epart : '0',
      '#fieldset' => 'ener_base',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>',
        );
    
    if(isset($entity->gueltig_bis)){
      $field_date_g = $entity->gueltig_bis;
      // make sure it is integer / timestamp
      $field_date = intval($field_date_g);
    }
    else{$field_date = '';}
    
    $entity_form ['gueltig_bis'] = array(
      '#title' => t('Valid until'),
   //   '#description' =>t('01-2024'),
      '#type' => 'date_popup',
      '#element_validate' => array('_immoclient_callback_validate_uidate2timestamp'),
      '#fieldset' => 'ener_base',
      '#default_value' => isset($entity->gueltig_bis) ? $entity->gueltig_bis : '',
      '#date_timezone' => date_default_timezone(),
      '#date_format' => 'd.m.Y',
      '#date_year_range' => '-1:+20',
      '#weight' => 5, 
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>',   
    );
    if ($field_date !='') {
        $date = new DateObject($field_date);
    //    dpm($date);
        $entity_form['gueltig_bis']['#default_value'] = $date->format(DATE_FORMAT_DATETIME);
        $entity_form['gueltig_bis']['#date_format'] = 'd.m.Y';
        $entity_form['gueltig_bis']['#date_timezone'] = $date->getTimeZone()->getName();
    }
    
    if(isset($entity->ausstelldatum)){
      $field_date_a = $entity->ausstelldatum;
      // make sure it is integer / timestamp
      $field_date = intval($field_date_a);
    }
    else{
        $field_date = '';
    }
     $entity_form ['ausstelldatum'] = array(
      '#title' => t('Date of issue'),
      '#type' => 'date_popup',
      '#element_validate' => array('_immoclient_callback_validate_uidate2timestamp'),
      '#date_timezone' => date_default_timezone(),
      '#date_format' => 'd.m.Y',
      '#date_year_range' => '-15:+1',
      '#fieldset' => 'ener_base',
      '#weight' => 4,
      '#default_value' => isset($entity->ausstelldatum) ? date($entity->ausstelldatum) : '',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>', 
    );
    if ($field_date != '') {
        $date = new DateObject($field_date);
        $entity_form['ausstelldatum']['#default_value'] = $date->format(DATE_FORMAT_DATETIME);
        $entity_form['ausstelldatum']['#date_format'] = 'd.m.Y';
        $entity_form['ausstelldatum']['#date_timezone'] = $date->getTimeZone()->getName();
    }
    
    $entity_form ['energieverbrauchkennwert'] = array(
      '#title' => t('Parameter of energy consumption'),
      '#description' =>t('50,50 kWh/(a m²)'),
      '#type' => 'numericfield',
      '#precision' => 7,
      '#decimals' => 2,
      '#minimum' => 0,
      '#weight' => 1,
      '#maximum' => 50000000.99,
      '#fieldset' => 'ener_base',
      '#element_validate' => array('immoclient_price_area_check_empty_string'),
      '#default_value' => isset($entity->energieverbrauchkennwert) ? $entity->energieverbrauchkennwert : '',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px; min-height:110px;">',
      '#suffix' =>'</div>',
      '#states' => array(
        'invisible' => array(
        ':input[name="field_energieausweis_reference[und][form][epart]"]' => 
          array('value' => 'BEDARF'), 
        ),
      ),
    );
    $entity_form ['mitwarmwasser'] = array(
      '#title' => t('With warm water?'),
      '#type' => 'radios',
      '#weight' => 3,
      '#options' => array(0 => t('No'),1=> t('Yes')),
      '#default_value' => isset($entity->mitwarmwasser) ? $entity->mitwarmwasser : '0',
      '#fieldset' => 'ener_base',
      '#required' => TRUE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>',
    );
    $entity_form ['endenergiebedarf'] = array(
      '#title' => t('Final energy demand'),
      '#type' => 'textfield',
      '#element_validate' => array('immoclient_price_area_check_empty_string'),
      '#fieldset' => 'ener_base',
      '#default_value' => isset($entity->endenergiebedarf) ? $entity->endenergiebedarf : '',
      '#size' => 10,
      '#weight' => 2, 
      '#maxlength' => 10, 
      '#required' => FALSE,
      '#prefix' => '<div style="float:left;padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>',
      '#states' => array(
        'invisible' => array(
        ':input[name="field_energieausweis_reference[und][form][epart]"]' => 
          array('value' => 'VERBRAUCH'), 
        ),
      ),
    );
    /***
     * Primärenergieträger
     * $V127 Aufzählung wie in: befeuerung. Optional freier Text
     */
    /*** Anfang checkbox mit fieldable ***/
    // Finde den schon gesetzten Wert für das Objekt heraus um es als default anzugeben
    $field_name = 'field_primaerenergietraeger';
    $default = default_values_energy($entity,$field_name);
    if(!$default){
        $default = array();
    }
    // Ziehe die Form aus der Entity um die fieldable jede für sich einzusetzen
    $field = field_info_field('field_primaerenergietraeger');
    $instance = field_info_instance('immoclient_energy', 'field_primaerenergietraeger', 'immoclient_energy');
    
    $my_field = field_default_form('immoclient_energy', $entity, $field, $instance, LANGUAGE_NONE, array(), $form, $form_state);
    //Hau die default Werte dazu:
    $my_field['field_primaerenergietraeger']['und']['#default_value']= $default;
    $my_field['field_primaerenergietraeger']['#weight'] = 6;
    $my_field['field_primaerenergietraeger']['#fieldset'] = 'ener_base';
    $my_field['field_primaerenergietraeger']['#prefix'] = '<div style = "clear:left; margin-bottom:50px;">';
    $my_field['field_primaerenergietraeger']['#suffix'] = '</div>';
   // dpm($my_field,'Bad');
    $entity_form += (array) $my_field;

    /*** Ende Checkbox mit fieldable ***/
    $entity_form ['stromwert'] = array(
    '#title' => t('Current value'),
    '#description' =>t('Number between 0 - 10000'),
    '#type' => 'textfield',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
   '#fieldset' => 'ener_base',
    '#default_value' => isset($entity->stromwert) ? $entity->stromwert : '',
    '#size' => 10,
    '#weight' => 7, 
    '#maxlength' => 10, 
    '#required' => FALSE,
     '#prefix' => '<div style="clear:left;float:left; padding: 0 15px;min-height:110px;">',
     '#suffix' =>'</div>',   
    );
     $entity_form ['waermewert'] = array(
    '#title' => t('Heating value'),
    '#description' =>t('Number between 0 - 500'),
    '#type' => 'textfield',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
   '#fieldset' => 'ener_base',
    '#default_value' => isset($entity->waermewert) ? $entity->waermewert : '',
    '#size' => 10,
    '#weight' => 8, 
    '#maxlength' => 10, 
    '#required' => FALSE,
     '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
     '#suffix' =>'</div>',   
    );
     $entity_form ['wertklasse'] = array(
    '#title' => t('Class'),
    '#description' =>t('A - G'),
    '#type' => 'select',
   // '#element_validate' => array('immoclient_price_area_check_empty_string'),
   '#fieldset' => 'ener_base',
    '#default_value' => isset($entity->wertklasse) ? $entity->wertklasse : '0',
   '#options' => array(
     '0' => t('none'),
     'A'=> 'A',
     'B'=> 'B',
     'C'=> 'C',
     'D'=> 'D',
     'E'=> 'E',
     'F'=> 'F',
     'G'=> 'G'
    ),
    '#weight' => 9, 
    '#required' => FALSE,
     '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
     '#suffix' =>'</div>',   
    );

    $entity_form ['jahrgang'] = array(
      '#title' => t('Year'),
      '#description'=>t('Values: "2008" = before 1.5.2014, "2014"= since 1.5.2014 ! ,<br> "none"= not available  ,<br> "not necessary" e.g. monument conservation'),
      '#type' => 'radios',
      '#weight' => 13,
      '#options' => array(
        'ohne' => t('none'),
        '2008'=> '2008',
        '2014'=> '2014',
        'nicht_noetig'=>t('not necessary')
        ),
      '#default_value' => isset($entity->jahrgang) ? $entity->jahrgang : 'ohne',
      '#fieldset' => 'ener_base',
      '#required' => TRUE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>',
        );
    $entity_form ['gebaeudeart'] = array(
      '#title' => t('Building type'),
      '#type' => 'radios',
      '#weight' => 12,
      '#options' => array(
        '0' => t('none'),
        'wohn'=>t('Habitation'),
        'nichtwohn'=> t('Non habitation')
        ),
      '#default_value' => isset($entity->gebaeudeart) ? $entity->gebaeudeart : '0',
      '#fieldset' => 'ener_base',
      '#required' => TRUE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>',
        );
    $entity_form ['furn_at'] = array(
        '#type' => 'fieldset',
        '#title' => t('Austria'),
        '#weight' => 14,
        '#prefix' => '<div style="clear:left;">',
        '#suffix' =>'</div>',
        '#states' => array(
        'visible' => array(
        ':input[name="field_energieausweis_reference[und][form][ausweis]"]' => array('value' => 0), 
      ),
    ),
    );
    $entity_form ['hwbwert'] = array(
    '#title' => t('Heating demand'),
  //  '#description' =>t('A - G'),
    '#type' => 'textfield',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
   '#fieldset' => 'furn_at',
    '#default_value' => isset($entity->hwbwert) ? $entity->hwbwert : '',
    '#size' => 5,
    '#weight' => 1, 
    '#maxlength' => 10, 
    '#required' => FALSE,
     '#prefix' => '<div style="float:left; padding: 0px 15px">',
     '#suffix' =>'</div>',   
    );
    $entity_form ['hwbklasse'] = array(
    '#title' => t('Class of heating demand'),
  //  '#description' =>t('A - G'),
    '#type' => 'textfield',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
   '#fieldset' => 'furn_at',
    '#default_value' => isset($entity->hwbklasse) ? $entity->hwbklasse : '',
    '#size' => 5,
    '#weight' => 2, 
    '#maxlength' => 10, 
    '#required' => FALSE,
     '#prefix' => '<div style="float:left; padding: 0px 15px">',
     '#suffix' =>'</div>',   
    );
    $entity_form ['fgeewert'] = array(
    '#title' => t('Energy efficience factor'),
  //  '#description' =>t('A - G'),
    '#type' => 'textfield',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
   '#fieldset' => 'furn_at',
    '#default_value' => isset($entity->fgeewert) ? $entity->fgeewert : '',
    '#size' => 5,
    '#weight' => 3, 
    '#maxlength' => 10, 
    '#required' => FALSE,
     '#prefix' => '<div style="float:left; padding: 0px 15px">',
     '#suffix' =>'</div>',   
    );
    $entity_form ['fgeeklasse'] = array(
    '#title' => t('Class of energy efficience'),
  //  '#description' =>t('A - G'),
    '#type' => 'textfield',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
   '#fieldset' => 'furn_at',
    '#default_value' => isset($entity->fgeeklasse) ? $entity->fgeeklasse : '',
    '#size' => 5,
    '#weight' => 4, 
    '#maxlength' => 10, 
    '#required' => FALSE,
     '#prefix' => '<div style="float:left; padding: 0px 15px">',
     '#suffix' =>'</div>',   
    );
    return $entity_form;
 }
}

