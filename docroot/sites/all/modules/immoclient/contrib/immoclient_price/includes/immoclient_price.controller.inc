<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class ImmoclientPricedefaultInlineEntityFormController extends EntityInlineEntityFormController{
    
   /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Price default'),
      'plural' => t('Prices default'),
    );
    return $labels;
  }
  

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    $info = entity_get_info($this->entityType);
    $entity = $entity_form['#entity'];
    
    $entity_form['price'] = array(
       '#type' => 'vertical_tabs',       
    );
    
    $entity_form['preis']= array(
      '#type' => 'fieldset',
      '#title' => t('Price'),
      '#description' =>t('PRICES NET OR GROSS'),
      '#weight' => 1,
      '#group' => 'field_immo_price][und][form][price',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#attributes'=> array('class' => array('vertical-tabs-pane')),
      '#attached' => array(
        'js' => array(
          'vertical-tabs' => drupal_get_path('module', 'immoclient') . '/js/immoclient_vertical_tabs_description.js',
        ),
        'css' => array(
          'seven' => drupal_get_path('module', 'immoclient') . '/css/immoclient_seven.css',
        ),  
      ),
    );
    $entity_form['auf_anfrage'] = array(
      '#title' => t('On demand?'),
      '#type' => 'radios',
      '#options' => array(
        0 => t('No'), 
        1 => t('Yes')
        ),
      '#default_value' => isset($entity->auf_anfrage) ? $entity->auf_anfrage : '0',
      '#fieldset' => 'preis',
      '#states' => array(
        'visible' => array(
        ':input[name="field_immo_price[und][form][kaufpreis]"]' => array(
          array('value' => ''),
          array('value'=> 0),
          array('value' => "0,00"),
            ),
          ':input[name="field_immo_price[und][form][kaufpreisnetto]"]' => array(//or condition
            array('value' => 0),
            array('value' => ""),
            array('value' => "0,00"), 
            ),
          ),
        ),
       '#prefix' => '<div class="price_left">',
       '#suffix' =>'</div>'
    );
    
    $active_vb = array(0 => t('No'), 1 => t('Yes'));
    $entity_form ['vb'] = array(
      '#title' => t('Price negotiable?'),
      '#type' => 'radios',
      '#options' => $active_vb,
      '#default_value' => isset($entity->vb) ? $entity->vb : '0',
      '#fieldset' => 'preis',
      '#states' => array(
        'visible' => array(//and condition
        ':input[name="field_immo_price[und][form][auf_anfrage]"]'=> array('value' => "0"),
        ':input[name="field_immo_price[und][form][kaufpreis]"]' => array(//or condition
            array('value' => 0),
            array('value' => ""),
            array('value' => "0,00"),
          ),
          ':input[name="field_immo_price[und][form][kaufpreisnetto]"]' => array(//or condition
            array('value' => 0),
            array('value' => ""),
            array('value' => "0,00"), 
          ),
        ),
      ),
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>'
    );
 
    $entity_form ['kaufpreis'] = array(
      '#title' => t('Price incl. VAT'),
      '#description' => t('Price with VAT'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'preis',
      '#element_validate' => array('immoclient_price_check_null'),
      '#default_value' => isset($entity->kaufpreis) ? $entity->kaufpreis : '0.00',
      '#required' => FALSE,
        '#states' => array(
        'invisible' => array(//or condition
        array(':input[name="field_immo_price[und][form][auf_anfrage]"]'=> array('value' => "1")),
        array(':input[name="field_immo_price[und][form][vb]"]' => array('value' => '1')),
        array(':input[name="field_immo_price[und][form][kaufpreisnetto]"]' => array(//or condition
            array('!value' => "0,00"), 
          ))
          ),
        ),
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>' 
    );

    $entity_form['richtpreis']=array(
      '#title' => t('Suggested price'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'preis',
      '#element_validate' => array('immoclient_price_check_null'),
      '#default_value' => isset($entity->richtpreis) ? $entity->richtpreis : '0.00',
      '#required' => FALSE,
      '#states' => array( 
        'invisible' => array(
          ':input[name="field_immo_price[und][form][vb]"]' => array(
            array('value' => '0'),
            array('value' => '')),
            ),
        ),
      '#prefix' => '<div class="price_left" class="richtpreis">',//formerly hidden with class="richtpreis_hidden"
      '#suffix' =>'</div>'           
    );
    $entity_form ['kaufpreisnetto'] = array(
      '#title' => t('Price excl. VAT'),
      '#description' =>t('Commercial: Price net, optional with VAT'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'preis',
      '#states' => array( 
          'visible' => array( //and-condition
            ':input[name="field_immo_price[und][form][vb]"]' => array('value' => '0'),
            ':input[name="field_immo_price[und][form][kaufpreis]"]'=> array(
                array('value' => "0"),
                array('value' => "0,00"),
                array('value' => ""),
                ),
            ':input[name="field_immo_price[und][form][auf_anfrage]"]'=> array('value' => "0"),
          ),
        ),
      '#default_value' => isset($entity->kaufpreisnetto) ? $entity->kaufpreisnetto : '0.00',
      '#required' => FALSE,

      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    $entity_form['kaufpreisust'] = array(
      '#title' => t('VAT'),
      '#description' =>t('Commercial: VAT'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#default_value' => isset($entity->kaufpreisust) ? $entity->kaufpreisust : '0.00',
      '#fieldset' => 'preis',
      '#required' => FALSE, 
       '#states' => array( 
          'visible' => array( //and-condition
            ':input[name="field_immo_price[und][form][vb]"]' => array('value' => '0'),
            ':input[name="field_immo_price[und][form][kaufpreis]"]'=> array(
                array('value' => "0"),
                array('value' => "0,00"),
                array('value' => ""),
                ),
            ':input[name="field_immo_price[und][form][auf_anfrage]"]'=> array('value' => "0"),
          ),
        ),
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>'
    );
    $entity_form ['kaufpreisbrutto'] = array(
      '#title' => t('Price with VAT'),
      '#description' =>t('Commercial use: Price incl. VAT'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'preis',
      '#default_value' => isset($entity->kaufpreisbrutto) ? $entity->kaufpreisbrutto : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="kaufpreisbrutto_hidden">',
      '#suffix' =>'</div>',   
    );
     
    $entity_form['prov']= array(
      '#type' => 'fieldset',
      '#title' => t('Commission'),
      '#weight' => 2,
      '#description' =>t('Commission value from customer or seller'),
      '#group' => 'field_immo_price][und][form][price',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#attributes'=> array('class' => array('vertical-tabs-pane')),
  );
  
    $entity_form ['aussen_courtage'] = array(
      '#title' => t('Commission value from customer'),
      '#description' =>t('E.g. 3,57 = 3% plus VAT,<br/> or 2500,00 = 2500€'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#fieldset' => 'prov',
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_value_percent'),
      '#default_value' => isset($entity->aussen_courtage) ? $entity->aussen_courtage : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px; min-height:115px">',
      '#suffix' =>'</div>',
    );
    
    $entity_form ['mit_mwst_a'] = array(
      '#title' => t('Commission value from customer with VAT'),
      '#type' => 'radios',
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => isset($entity->mit_mwst_a) ? $entity->mit_mwst_a : '1',
      '#fieldset' => 'prov',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:115px">',
      '#suffix' =>'</div>',
    );
    
     $entity_form ['innen_courtage'] = array(
    '#title' => t('Commission value from seller'),
    '#description' =>t('E.g. 3,57 for 3% with VAT or 2500,00 for 2500€'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#fieldset' => 'prov',
    '#maximum' => 50000000.99,
    '#element_validate' => array('immoclient_price_check_value_percent'),
    '#default_value' => isset($entity->innen_courtage) ? $entity->innen_courtage : '0.00',
    '#required' => FALSE,
     '#prefix' => '<div style="float:left; padding: 0 15px;min-height:115px">',
     '#suffix' =>'</div>',
     '#states' => array(
     'enabled' => array(
        ':input[name="field_immo_price[und][form][aussen_courtage]"]' => array(
            array('value'=>''), array('value'=>'0'), array('value'=>'0,00'),)
            ),
        ),
    );
     
    $entity_form ['mit_mwst'] = array(
      '#title' => t('Commission value from seller with VAT'),
      '#type' => 'radios',
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => isset($entity->mit_mwst) ? $entity->mit_mwst : '1',
      '#fieldset' => 'prov',
      '#required' => FALSE,
      '#prefix' => '<div style="padding: 0 15px;min-height:115px">',
      '#suffix' =>'</div>',
      '#states' => array(     
        'enabled' => array(
          ':input[name="field_immo_price[und][form][aussen_courtage]"]' => array(
              array('value'=>''), array('value'=>'0'), array('value'=>'0,00'),)
        ),
      ),
    );
    $entity_form ['mwst_satz'] = array(
      '#title' => t('VAT'),
      '#description' =>t('Rate of VAT for both Commissions'),
      '#type' => 'numericfield',
      '#precision' => 4,
      '#decimals' => 2,
      '#minimum' => 0,
      '#fieldset' => 'prov',
      '#maximum' => 50,
      '#element_validate' => array('immoclient_price_check_null'),
      '#default_value' => isset($entity->mwst_satz) ? $entity->mwst_satz : '0.19', 
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:115px">',
      '#suffix' =>'</div>',    
        '#states' => array(     
          'invisible' => array(
            ':input[name="field_immo_price[und][form][mit_mwst]"]' => array('value'=>'1'),
            ':input[name="field_immo_price[und][form][mit_mwsta]"]' => array('value'=>'1'),
          ),
        ),
      );
    
    $entity_form ['mwst_gesamt'] = array(
      '#title' => t('VAT as amount'),
      '#description' =>t('Amount written as number when VAT rates are different'),
      '#type' => 'numericfield',
      '#precision' => 8,
      '#decimals' => 2,
      '#minimum' => 0,
      '#fieldset' => 'prov',
      '#maximum' => 50,
      '#element_validate' => array('immoclient_price_check_null'),
      '#default_value' => isset($entity->mwst_gesamt) ? $entity->mwst_gesamt : '0.00', 
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:115px">',
      '#suffix' =>'</div>',    
      '#states' => array(     
        'invisible' => array(
          ':input[name="field_immo_price[und][form][mit_mwst]"]' => array('value'=>'1'),
          ':input[name="field_immo_price[und][form][mit_mwsta]"]' => array('value'=>'1'),
        ),
      ),
    );
    
    $active_prov = array(0 => t('No'), 1 => t('Yes'));
    $entity_form ['provisionspflichtig'] = array(
      '#title' => t('Subject to commission'),
      '#type' => 'radios',
      '#options' => $active_prov,
      '#default_value' => isset($entity->provisionspflichtig) ? $entity->provisionspflichtig : '0',
      '#fieldset' => 'prov',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:115px">',
      '#suffix' =>'</div>'
    );
    
    $entity_form ['provision_teilen'] = array(
      '#title' => t('Share commission'),
      '#type' => 'radios',
      '#options' => $active_prov,
      '#default_value' => isset($entity->provision_teilen) ? $entity->provision_teilen : '0',
      '#fieldset' => 'prov',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:115px">',
      '#suffix' =>'</div>'
    );
    
    $entity_form ['provision_teilen_wert'] = array(
      '#title' => t('Share commission value'),
      '#description' =>t('"A Meta" business: fixed value, percent, or string.'),
      '#type' => 'textfield',
      '#fieldset' => 'prov',
      '#default_value' => isset($entity->provision_teilen_wert) ? $entity->provision_teilen_wert : '',
      '#size' => 30,
      '#maxlength' => 128, 
      '#required' => FALSE,
       '#prefix' => '<div style="float:left; padding: 10px 15px">',
       '#suffix' =>'</div>',   
    );
    
    $entity_form ['courtage_hinweis'] = array(
      '#title' => t('Useful infos about commission'),
      '#description' =>t('Add whatever needed as your commission e.g. how many monthtly rents'),
      '#type' => 'textarea',
      '#fieldset' => 'prov',
      '#default_value' => isset($entity->courtage_hinweis) ? $entity->courtage_hinweis : '',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
    $entity_form ['waehrung'] = array(
      '#title' => t('Currency'),
      '#type' => 'select',
      '#options' => array(
        'EUR' => 'EUR',
        'BIH' => 'BIH',
        'BGR' => 'BGR',
        'DNK' => 'DNK',
        'GBP' => 'GBP',
        'HRK' => 'HRK',
        'MKD' => 'MKD',
        'NOK' => 'NOK',
        'PLZ' => 'PLZ',
        'ROL' => 'ROL',
        'SRB' => 'SRB',
        'SWE' => 'SWE',
        'CHE' => 'CHE',
        'TUR' => 'TUR',
        'CZK' => 'CZK',
        'HUF' => 'HUF',
        'USD' => 'USD',
      ),
      '#fieldset' => 'prov',
      '#default_value' => isset($entity->waehrung) ? $entity->waehrung : 'EUR', 
      '#required' => FALSE,
      '#prefix' => '<div style="float:left;padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>',
    );
    
    $entity_form['miete']= array(
      '#type' => 'fieldset',
      '#title' => t('Rent'),
      '#weight' => 3,
      '#description' =>t('DIFFERENT KINDS OF RENT AND DEPOSIT'),
      '#group' => 'field_immo_price][und][form][price',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE, 
      '#attributes'=> array('class' => array('vertical-tabs-pane')),
    );

    $entity_form ['kaltmiete'] = array(
      '#title' => t('Cold rent'),
      '#description' =>t('Rent net with utilities without heating'),
      '#type' => 'numericfield',
      '#element_validate' => array('immoclient_price_check_null'),
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'miete',
      '#default_value' => isset($entity->kaltmiete) ? $entity->kaltmiete : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
    $entity_form ['nettokaltmiete'] = array(
      '#title' => t('Net Cold Rent'),
      '#description' =>t('Cold rent without utilities'),
      '#type' => 'numericfield',
      '#element_validate' => array('immoclient_price_check_null'),
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'miete',
      '#default_value' => isset($entity->nettokaltmiete) ? $entity->nettokaltmiete : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );

    $entity_form ['warmmiete'] = array(
      '#title' => t('Warm Rent'),
      '#description' =>t('Rent incl. utilities and heating'),
      '#type' => 'numericfield',
      '#element_validate' => array('immoclient_price_check_null'),
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'miete',
      '#default_value' => isset($entity->warmmiete) ? $entity->warmmiete : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    $entity_form ['pauschalmiete'] = array(
      '#title' => t('All in rent'),
      '#description' =>t('Flat rate'),
      '#type' => 'numericfield',
      '#element_validate' => array('immoclient_price_check_null'),
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'miete',
      '#default_value' => isset($entity->pauschalmiete) ? $entity->pauschalmiete : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    $entity_form ['kaution'] = array(
      '#title' => t('Deposit'),
      '#description' =>t('Deposit (€)'),
      '#type' => 'numericfield',
      '#element_validate' => array('immoclient_price_check_null'),
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'miete',
      '#default_value' => isset($entity->kaution) ? $entity->kaution : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
    $entity_form ['kaution_text'] = array(
      '#title' => t('Deposit description'),
      '#description' =>t('optional'),
      '#type' => 'textarea',
      '#fieldset' => 'miete',
      '#default_value' => isset($entity->kaution_text) ? $entity->kaution_text : '',
      '#required' => FALSE,
      '#prefix' => '<div style="clear:both;">',
      '#suffix' =>'</div>',   
    );
    if(isset($entity->preis_zeitraum_von)){
      $field_date_g = $entity->preis_zeitraum_von;
      // make sure it is integer / timestamp
      $field_date = intval($field_date_g);
    }
    else{$field_date = '';}
    
    $entity_form ['preis_zeitraum_von'] = array(
      '#title' => t('Price from date'),
      '#description' =>t('For holiday properties'),
      '#type' => 'date_popup',
      '#element_validate' => array('_immoclient_callback_validate_uidate2timestamp'),
      '#fieldset' => 'preis',
      '#default_value' => isset($entity->preis_zeitraum_von) ? $entity->preis_zeitraum_von : '',
      '#date_timezone' => date_default_timezone(),
      '#date_format' => 'd.m.Y',
      '#date_year_range' => '-1:+20',
      '#weight' => 12, 
      '#required' => FALSE,
      '#states' => array( 
        'visible' => array( 
          ':input[name="field_als_ferien[und]"]' => array('value' => 'Yes'),
        ),
      ),
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>',   
    );
    if ($field_date !='') {
        $date = new DateObject($field_date);
        $entity_form['preis_zeitraum_von']['#default_value'] = $date->format(DATE_FORMAT_DATETIME);
        $entity_form['preis_zeitraum_von']['#date_format'] = 'd.m.Y';
        $entity_form['preis_zeitraum_von']['#date_timezone'] = $date->getTimeZone()->getName();
    }
    
    if(isset($entity->preis_zeitraum_bis)){
      $field_date_g = $entity->preis_zeitraum_bis;
      // make sure it is integer / timestamp
      $field_date = intval($field_date_g);
    }
    else{$field_date = '';}
    
    $entity_form ['preis_zeitraum_bis'] = array(
      '#title' => t('Price until'),
      '#description' =>t('For holiday properties'),
      '#type' => 'date_popup',
      '#element_validate' => array('_immoclient_callback_validate_uidate2timestamp'),
      '#fieldset' => 'preis',
      '#default_value' => isset($entity->preis_zeitraum_bis) ? $entity->preis_zeitraum_bis : '',
      '#date_timezone' => date_default_timezone(),
      '#date_format' => 'd.m.Y',
      '#date_year_range' => '-1:+20',
      '#weight' => 13, 
      '#required' => FALSE,
      '#states' => array( 
        'visible' => array( 
          ':input[name="field_als_ferien[und]"]' => array('value' => 'Yes'),
        ),
      ),
      '#prefix' => '<div style="float:left; padding: 0 15px;min-height:110px;">',
      '#suffix' =>'</div>',   
    );
    if ($field_date !='') {
        $date = new DateObject($field_date);
    //    dpm($date);
        $entity_form['preis_zeitraum_bis']['#default_value'] = $date->format(DATE_FORMAT_DATETIME);
        $entity_form['preis_zeitraum_bis']['#date_format'] = 'd.m.Y';
        $entity_form['preis_zeitraum_bis']['#date_timezone'] = $date->getTimeZone()->getName();
    }
    $entity_form ['preis_zeiteinheit'] = array(
      '#title' => t('Time unit'),
      '#description' =>t('For holiday properties'),
      '#type' => 'select',
      '#options' => array(
          '0' => t('none'),
          'TAG' => t('Day'),
          'WOCHE' =>t('Week'),
          'MONAT' => t('Month'),
          'JAHR' => t('Year'),
      ),
      '#fieldset' => 'preis',
      '#default_value' => isset($entity->preis_zeiteinheit) ? $entity->preis_zeiteinheit : '0', 
      '#maxlength' => 10, 
      '#required' => FALSE,
      '#states' => array( 
        'visible' => array( 
          ':input[name="field_als_ferien[und]"]' => array('value' => 'Yes'),
        ),
      ),
      '#weight' => 14, 
      '#prefix' => '<div style="float:left; padding: 0 15px;">',
      '#suffix' =>'</div>',
    );
    $entity_form['nkhz']= array(
      '#type' => 'fieldset',
      '#title' => t('Utlilities/Heating'),
      '#weight' => 4,
      '#description' =>t('INCL. RENT ADDITIONS ...'),
      '#group' => 'field_immo_price][und][form][price',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#attributes'=> array('class' => array('vertical-tabs-pane')),
  );
    $entity_form ['nebenkosten'] = array(
      '#title' => t('Utilities'),
      '#description' =>t('Monthly utilities without heating'),
      '#type' => 'numericfield',
      '#element_validate' => array('immoclient_price_check_null'),
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'nkhz',
      '#default_value' => isset($entity->nebenkosten) ? $entity->nebenkosten : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    $entity_form ['heizkosten_enthalten'] = array(
      '#title' => t('Heating costs included?'),
      '#type' => 'radios',
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => isset($entity->heizkosten_enthalten) ? $entity->heizkosten_enthalten : 1,
      '#fieldset' => 'nkhz',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>'
    );
    $entity_form ['heizkosten'] = array(
      '#title' => t('Heating'),
      '#description' =>t('Monthly Heating Costs - advance payment'),
      '#type' => 'numericfield',
      '#element_validate' => array('immoclient_price_check_null'),
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'nkhz',
      '#default_value' => isset($entity->heizkosten) ? $entity->heizkosten : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    $entity_form ['zzg_mehrwertsteuer'] = array(
      '#title' => t('Plus VAT - one time per building?'),
      '#type' => 'radios',
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => isset($entity->zzg_mehrwertsteuer) ? $entity->zzg_mehrwertsteuer : 0,
      '#fieldset' => 'nkhz',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>'
    );
    $entity_form ['mietzuschlaege'] = array(
      '#title' => t('Rent additions'),
      '#type' => 'numericfield',
      '#element_validate' => array('immoclient_price_check_null'),
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'nkhz',
      '#default_value' => isset($entity->mietzuschlaege) ? $entity->mietzuschlaege : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
     $entity_form['hg_ab']= array(
      '#type' => 'fieldset',
      '#title' => t('Yield'),
      '#weight' => 5,
      '#description' =>t('YIELD, X-NODE, AGIO'),
      '#group' => 'field_immo_price][und][form][price',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#attributes'=> array('class' => array('vertical-tabs-pane')),
  );
     $entity_form ['nettorendite'] = array(
    '#title' => t('Net yield'),
      '#description' =>t('Net yield'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->nettorendite) ? $entity->nettorendite : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );

    $entity_form ['nettorendite_soll'] = array(
      '#title' => t('Net yield target'),
      '#description' =>t('yield-additional cost target'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->nettorendite_soll) ? $entity->nettorendite_soll : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    $entity_form ['nettorendite_ist'] = array(
      '#title' => t('Actual net yield'),
      '#description' =>t('Actual yield-additional cost'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->nettorendite_ist) ? $entity->nettorendite_ist : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    $entity_form ['mieteinnahmen_ist'] = array(
      '#title' => t('Actual rental'),
      '#description' =>t('Actual rental target'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->mieteinnahmen_ist) ? $entity->mieteinnahmen_ist : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div style="padding: 0 15px; min-height:110px;">',
      '#suffix' =>'</div>',   
    );
    $entity_form ['periode'] = array(
      '#title' => t('Time intervall for actual rental'),
      '#type' => 'select',
      '#options' => array(
          '0' => t('none'),
          'TAG' => t('Day'),
          'WOCHE' =>t('Week'),
          'MONAT' => t('Month'),
          'JAHR' => t('Year'),
      ),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->periode) ? $entity->periode : '0', 
      '#required' => FALSE, 
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',
    );
    $entity_form ['mieteinnahmen_soll'] = array(
      '#title' => t('Rental target'),
      '#description' =>t('Planned rental target'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->mieteinnahmen_soll) ? $entity->mieteinnahmen_soll : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    $entity_form ['periode_soll'] = array(
      '#title' => t('Time intervall for rental target'),
      '#type' => 'select',
      '#options' => array(
          '0' => t('none'),
          'TAG' => t('Day'),
          'WOCHE' =>t('Week'),
          'MONAT' => t('Month'),
          'JAHR' => t('Year'),
      ),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->periode_soll) ? $entity->periode_soll : '0', 
      '#required' => FALSE, 
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',
    );
    
    $entity_form ['hausgeld'] = array(
      '#title' => t('Rent payment'),
      '#description' =>t('Common charges based on "WEG" (without VAT)'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#weight'=>10,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->hausgeld) ? $entity->hausgeld : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
    $entity_form ['abstand'] = array(
      '#title' => t('Agio'),
      '#description' =>t('Advanced payments'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#weight'=> 11,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->abstand) ? $entity->abstand : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
    $entity_form ['geschaeftsguthaben'] = array(
    '#title' => t('Business assets'),
      '#description' =>t('E.g. housing corporation'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#weight'=> 9,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->geschaeftsguthaben) ? $entity->geschaeftsguthaben : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
     $entity_form ['mietpreis_pro_qm'] = array(
      '#title' => t('Rent per square meter'),
      '#description' =>t('net'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->mietpreis_pro_qm) ? $entity->mietpreis_pro_qm : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
     
    $entity_form ['kaufpreis_pro_qm'] = array(
      '#title' => t('Price per square meter'),
      '#description' =>t('net'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->kaufpreis_pro_qm) ? $entity->kaufpreis_pro_qm : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
    $entity_form ['x_fache'] = array(
      '#title' => t('n-fold (Price / Rent per year)'),
      '#description' =>t('Relation of price to income by annual rent'),
      '#type' => 'textfield',
      '#fieldset' => 'hg_ab',
      '#default_value' => isset($entity->x_fache) ? $entity->x_fache : '',
      '#size' => 30,
      '#maxlength' => 128, 
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
    $entity_form['pacht_fs']= array(
      '#type' => 'fieldset',
      '#title' => t('Lease'),
      '#weight' => 6,
      '#description' =>t('LEASE, LEASEHOLD'),
      '#group' => 'field_immo_price][und][form][price',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#attributes'=> array('class' => array('vertical-tabs-pane')),
    );
    
    $entity_form ['pacht'] = array(
      '#title' => t('Lease'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'pacht_fs',
      '#default_value' => isset($entity->pacht) ? $entity->pacht : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
    $entity_form ['erbpacht'] = array(
      '#title' => t('Leasehold'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'pacht_fs',
      '#default_value' => isset($entity->erbpacht) ? $entity->erbpacht : '0.00',
      '#required' => FALSE,
       '#prefix' => '<div class="price_left">',
       '#suffix' =>'</div>',   
    );
    
    $entity_form['nkhz_at']= array(
      '#type' => 'fieldset',
      '#title' => t('Specialcase Austria VAT'),
      '#weight' => 7,
      '#description' =>t('RENT NET AND GROSS'),
      '#group' => 'field_immo_price][und][form][price',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#attributes'=> array('class' => array('vertical-tabs-pane')),
    );
    
    $entity_form ['hauptmietzinsnetto'] = array(
      '#title' => t('Main rent interest net'),
      '#description' =>t('Because of different VATS'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'nkhz_at',
      '#default_value' => isset($entity->hauptmietzinsnetto) ? $entity->hauptmietzinsnetto : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
    $entity_form ['hauptmietzinsust'] = array(
      '#title' => t('Main rent interest tax'),
      '#description' =>t('Because of different VATS'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'nkhz_at',
      '#default_value' => isset($entity->hauptmietzinsust) ? $entity->hauptmietzinsust : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
    $entity_form ['betriebskostennetto'] = array(
      '#title' => t('Cost of ownership'),
      '#description' =>t('Because of different VATS'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#element_validate' => array('immoclient_price_check_null'),
      '#fieldset' => 'nkhz_at',
      '#default_value' => isset($entity->betriebskostennetto) ? $entity->betriebskostennetto : '0.00',
      '#required' => FALSE,
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
     $entity_form ['betriebskostenust'] = array(
      '#title' => t('Cost of ownership VAT'),
      '#description' =>t('Because of different VATS'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'nkhz_at',
      '#default_value' => isset($entity->betriebskostenust) ? $entity->betriebskostenust : '0.00',
      '#required' => FALSE,
      '#element_validate' => array('immoclient_price_check_null'),
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
     
    $entity_form['estate']= array(
      '#type' => 'fieldset',
      '#title' => t('Estate'),
      '#weight' => 8,
      '#description' =>t('DEVELOPMENT COST'),
      '#group' => 'field_immo_price][und][form][price',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#attributes'=> array('class' => array('vertical-tabs-pane')),
    );
    
    $entity_form ['erschliessungskosten'] = array(
      '#title' => t('Development cost'),
      '#description' =>t('Development cost (€)'),
      '#type' => 'numericfield',
      '#precision' => 10,
      '#decimals' => 2,
      '#minimum' => 0,
      '#maximum' => 50000000.99,
      '#fieldset' => 'estate',
      '#default_value' => isset($entity->erschliessungskosten) ? $entity->erschliessungskosten : '0.00',
      '#required' => FALSE,
      '#element_validate' => array('immoclient_price_check_null'),
      '#prefix' => '<div class="price_left">',
      '#suffix' =>'</div>',   
    );
    
    return $entity_form;
  }
}
