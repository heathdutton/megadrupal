<?php

class ImmoclientareaInlineEntityFormController extends EntityInlineEntityFormController{
    
   /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Immo area'),
      'plural' => t('Immo areas'),
    );
    return $labels;
  }
  

/**
 * Overrides EntityInlineEntityFormController::entityForm().
 */
public function entityForm($entity_form, &$form_state) {
  $info = entity_get_info($this->entityType);
  $entity = $entity_form['#entity'];

  $entity_form['area'] = array(
    '#type' => 'vertical_tabs',       
  );
  $entity_form ['area_basis'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basics'),
    '#weight' => 1,
    '#description' =>t('LIVING SPACE, TOTAL AREA ...'),
    '#group' => 'field_flaechenangaben][und][form][area',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes'=> array('class' => array('vertical-tabs-pane')),
    );
  $entity_form ['wohnflaeche'] = array(
    '#title' => t('Living space'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'area_basis',
    '#element_validate' => array('immoclient_price_check_null'),
    '#default_value' => isset($entity->wohnflaeche) ? $entity->wohnflaeche : '0.00',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px;min-height:100px;">',
    '#suffix' =>'</div>'
    );
  $entity_form ['gesamtflaeche'] = array(
    '#title' => t('Total area'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'area_basis',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->gesamtflaeche) ? $entity->gesamtflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px;min-height:100px;">',
    '#suffix' =>'</div>'
    );
  $entity_form ['bgf'] = array(
    '#title' => t('Total floor area'),
    '#description' => t('Sum of all areas of every floor'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'area_basis',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->bgf) ? $entity->bgf : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px;min-height:100px;">',
    '#suffix' =>'</div>'
    );
  $entity_form ['sonstflaeche'] = array(
    '#title' => t('Other space'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 1000000,
    '#fieldset' => 'area_basis',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->sonstflaeche) ? $entity->sonstflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="padding: 0 15px;min-height:100px;">',
    '#suffix' =>'</div>'
    );
  $entity_form ['gewerbe'] = array(
    '#type' => 'fieldset',
    '#title' => t('Commercial Properties'),
    '#weight' => 2, 
    '#description' =>t('USEFUL AREA, STORESPACE, SURFACE OF THE WINDOWS ...'),
    '#group' => 'field_flaechenangaben][und][form][area',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes'=> array('class' => array('vertical-tabs-pane')),
    );
  $entity_form ['nutzflaeche'] = array(
    '#title' => t('Useful area'),
   // '#description' => t('Price with VAT'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->nutzflaeche) ? $entity->nutzflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
  $entity_form ['ladenflaeche'] = array(
    '#title' => t('Store space'),
   // '#description' => t('Price with VAT'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->ladenflaeche) ? $entity->ladenflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['lagerflaeche'] = array(
    '#title' => t('Storage space'),
   // '#description' => t('Price with VAT'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->lagerflaeche) ? $entity->lagerflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['verkaufsflaeche'] = array(
    '#title' => t('Sales area'),
   // '#description' => t('Price with VAT'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->verkaufsflaeche) ? $entity->verkaufsflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['freiflaeche'] = array(
    '#title' => t('Open space'),
   // '#description' => t('Price with VAT'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->freiflaeche) ? $entity->freiflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
     $entity_form ['bueroflaeche'] = array(
    '#title' => t('Office space'),
   // '#description' => t('Price with VAT'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->bueroflaeche) ? $entity->bueroflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['bueroteilflaeche'] = array(
    '#title' => t('Office space apart'),
   // '#description' => t('Price with VAT'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->bueroteilflaeche) ? $entity->bueroteilflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['fensterfront'] = array(
    '#title' => t('Surface of the windows'),
   // '#description' => t('Price with VAT'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->fensterfront) ? $entity->fensterfront : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['verwaltungsflaeche'] = array(
    '#title' => t('Space for administration'),
   // '#description' => t('Price with VAT'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->verwaltungsflaeche) ? $entity->verwaltungsflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['gastro'] = array(
        '#type' => 'fieldset',
        '#title' => t('Gastro'),
        '#weight' => 3, 
        '#description' =>t('SPACE FOR GASTRONOMY, AMOUNT OF MEETINGROOMS ...'),
        '#group' => 'field_flaechenangaben][und][form][area',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#tree' => TRUE,
        '#attributes'=> array('class' => array('vertical-tabs-pane')),
        );
    $entity_form ['gastroflaeche'] = array(
    '#title' => t('Space for gastronomy'),
   // '#description' => t('Price with VAT'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50000000.99,
    '#fieldset' => 'gastro',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->gastroflaeche) ? $entity->gastroflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['fl_gr'] = array(
        '#type' => 'fieldset',
        '#title' => t('ESTATE'),
        '#weight' => 4, 
        '#description' =>t('AREA OF ESTATE, GREEN AREA ...'),
        '#group' => 'field_flaechenangaben][und][form][area',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#tree' => TRUE,
        '#attributes'=> array('class' => array('vertical-tabs-pane')),
        );
    $entity_form ['grundstuecksflaeche'] = array(
    '#title' => t('Property area'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 50,
    '#weigth' => 1,
    '#fieldset' => 'fl_gr',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->grundstuecksflaeche) ? $entity->grundstuecksflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['gartenflaeche'] = array(
    '#title' => t('Area of garden in m²'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 10,
   '#fieldset' => 'fl_gr',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->gartenflaeche) ? $entity->gartenflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['grz'] = array(
    '#title' => t('Site occupancy index'),
    '#description' => t('Proportion of a building plot that can be overbuilt (e.g. in Ger:up to 0,3)'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 100,
    '#weigth' => 2,
    '#fieldset' => 'fl_gr',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->grz) ? $entity->grz : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
     $entity_form ['gfz'] = array(
    '#title' => t('Floor area ratio'),
    '#description' => t('Ratio of all floor areas to area of property of site'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#weigth' => 3,
    '#minimum' => 0,
    '#maximum' => 100,
    '#fieldset' => 'fl_gr',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->gfz) ? $entity->gfz : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
     $entity_form ['bmz'] = array(
    '#title' => t('Cubic index'),
    '#description' => t('Ratio of cubic capacity to area of property of site'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 100,
   '#fieldset' => 'fl_gr',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->bmz) ? $entity->bmz : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
     
    $entity_form ['fl_ausstattung'] = array(
        '#type' => 'fieldset',
        '#title' => t('Interior'),
        '#weight' => 5, 
        '#description' =>t('NUMBERS OF DIFFERENT CHAMBERS, AREAS OF ATTIC AND CELLAR ...'),
        '#group' => 'field_flaechenangaben][und][form][area',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#tree' => TRUE,
        '#attributes'=> array('class' => array('vertical-tabs-pane')),        
    );
    $entity_form ['anzahl_zimmer'] = array(
    '#title' => t('Number of chambers'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 1,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_zimmer) ? $entity->anzahl_zimmer : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_kuechen'] = array(
    '#title' => t('Number of kitchens'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_kuechen) ? $entity->anzahl_kuechen : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_schlafzimmer'] = array(
    '#title' => t('Number of bedrooms'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_schlafzimmer) ? $entity->anzahl_schlafzimmer : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_badezimmer'] = array(
    '#title' => t('Number of bathrooms'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_badezimmer) ? $entity->anzahl_badezimmer : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_sep_wc'] = array(
    '#title' => t('Number of separated toilets'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_sep_wc) ? $entity->anzahl_sep_wc : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_balkone'] = array(
    '#title' => t('Number of balconies'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_balkone) ? $entity->anzahl_balkone : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_terrassen'] = array(
    '#title' => t('Number of patios'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_terrassen) ? $entity->anzahl_terrassen : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_logia'] = array(
    '#title' => t('Number of loggias'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_logia) ? $entity->anzahl_logia : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['balkon_terrasse_flaeche'] = array(
    '#title' => t('Amount of all balconies and patios in m²'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->balkon_terrasse_flaeche) ? $entity->balkon_terrasse_flaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_wohn_schlafzimmer'] = array(
    '#title' => t('Amount of all living-bedrooms'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_wohn_schlafzimmer) ? $entity->anzahl_wohn_schlafzimmer : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    
    $entity_form ['kellerflaeche'] = array(
    '#title' => t('Basement space in qm'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->kellerflaeche) ? $entity->kellerflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['fensterfront_qm'] = array(
    '#title' => t('Window frontiere in qm'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->fensterfront_qm) ? $entity->fensterfront_qm : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['grundstuecksfront'] = array(
    '#title' => t('Distance to the street in m'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->grundstuecksfront) ? $entity->grundstuecksfront : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['dachbodenflaeche'] = array(
    '#title' => t('Attic space'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->dachbodenflaeche) ? $entity->dachbodenflaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['teilbar_ab'] = array(
    '#title' => t('Dividable since'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'sonst_fl',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->teilbar_ab) ? $entity->teilbar_ab : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['beheizbare_flaeche'] = array(
    '#title' => t('Heating area'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->beheizbare_flaeche) ? $entity->beheizbare_flaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_stellplaetze'] = array(
    '#title' => t('Number of parking places'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 10,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_stellplaetze) ? $entity->anzahl_stellplaetze : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['plaetze_gastraum'] = array(
    '#title' => t('Places in the official rooms'),
    '#description' => t('Max:10000'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 5,
    '#fieldset' => 'gastro',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->plaetze_gastraum) ? $entity->plaetze_gastraum : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_betten'] = array(
    '#title' => t('Amount of beds'),
    '#description' => t('Max:10000'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 5,
    '#fieldset' => 'gastro',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_betten) ? $entity->anzahl_betten : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_tagungsraeume'] = array(
    '#title' => t('Amount of meeting rooms'),
    '#description' => t('Max:10000'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 5,
    '#fieldset' => 'gastro',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_tagungsraeume) ? $entity->anzahl_tagungsraeume : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['vermietbare_flaeche'] = array(
    '#title' => t('Space to rent'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 5,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->vermietbare_flaeche) ? $entity->vermietbare_flaeche : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_wohneinheiten'] = array(
    '#title' => t('Numbers of living units'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 5,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_wohneinheiten) ? $entity->anzahl_wohneinheiten : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['anzahl_gewerbeeinheiten'] = array(
    '#title' => t('Numbers of business units'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 0,
    '#minimum' => 0,
    '#maximum' => 5,
    '#fieldset' => 'fl_ausstattung',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->anzahl_gewerbeeinheiten) ? $entity->anzahl_gewerbeeinheiten : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    $entity_form ['einliegerwohnung'] = array(
      '#title' => t('Granny flat available?'),
      '#type' => 'radios',
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => isset($entity->einliegerwohnung) ? $entity->einliegerwohnung : '0',
      '#fieldset' => 'fl_ausstattung',
      '#required' => FALSE,
      '#prefix' => '<div style="float:left; padding: 0 15px">',
      '#suffix' =>'</div>',
    );
    $entity_form ['sonst_fl'] = array(
        '#type' => 'fieldset',
        '#title' => t('OTHER'),
        '#weight' => 6, 
        '#description' =>t('KUBATUR'),
        '#group' => 'field_flaechenangaben][und][form][area',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#tree' => TRUE,
        '#attributes'=> array('class' => array('vertical-tabs-pane')),        
    );
    
     $entity_form ['kubatur'] = array(
    '#title' => t('Kubatur'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 5,
    '#fieldset' => 'sonst_fl',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->kubatur) ? $entity->kubatur : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
     $entity_form ['ausnuetzungsziffer'] = array(
    '#title' => t('Governing plot ratios'),
    '#description' => t('Ratio of land property and total floor area, mainly in Swiss'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 5,
    '#fieldset' => 'sonst_fl',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->ausnuetzungsziffer) ? $entity->ausnuetzungsziffer : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
     $entity_form ['flaechevon'] = array(
    '#title' => t('Area from'),
    '#description' => t('For commercial objects'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 5,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->flaechevon) ? $entity->flaechevon : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
     $entity_form ['flaechebis'] = array(
    '#title' => t('Area to'),
    '#description' => t('For commercial objects'),
    '#type' => 'numericfield',
    '#precision' => 10,
    '#decimals' => 2,
    '#minimum' => 0,
    '#maximum' => 5,
    '#fieldset' => 'gewerbe',
    '#element_validate' => array('immoclient_price_area_check_empty_string'),
    '#default_value' => isset($entity->flaechebis) ? $entity->flaechebis : '',
    '#required' => FALSE,
    '#prefix' => '<div style="float:left; padding: 0 15px">',
    '#suffix' =>'</div>'
    );
    return $entity_form;
  }
}




  
 
  