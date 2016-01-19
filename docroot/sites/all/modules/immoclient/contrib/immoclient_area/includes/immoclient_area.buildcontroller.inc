<?php


/**
 * Extending the EntityAPIController for the immoclient_area entity.
 */
class ImmoclientareaEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    
    // Our additions to the $build render array.
    $build['general'] = array(
      '#type' => 'fieldset',
      '#title' => t('General'),
      '#collapsible' => FALSE,
    );
    if($entity->wohnflaeche != '0'){
      $build ['general']['wohnflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Living space').'</b>'.': '. immoclient_price_make_komma($entity->wohnflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    ); 
      if(isset($entity->gesamtflaeche)){
      $build ['general']['gesamtflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Total area').'</b>'.': '.immoclient_price_make_komma($entity->gesamtflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    }
      if(isset($entity->bgf)){
      $build ['general']['bgf'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Total floor area').'</b>'.': '.immoclient_price_make_komma($entity->bgf).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    ); 
    }
    
      if(isset($entity->sonstflaeche)){
      $build ['general']['sonstflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Other space').'</b>'.': '.immoclient_price_make_komma($entity->sonstflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    ); 
    }
    $build['businessobject'] = array(
      '#type' => 'fieldset',
      '#title' => t('Businessobject'),
      '#collapsible' => FALSE,
    );
    if(isset($entity->nutzflaeche)){
      $build['businessobject']['nutzflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Useful area').'</b>'.': '.immoclient_price_make_komma($entity->nutzflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->ladenflaeche)){
      $build['businessobject']['ladenflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Store space').'</b>'.': '.immoclient_price_make_komma($entity->ladenflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->lagerflaeche)){
      $build['businessobject']['lagerflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Storage space').'</b>'.': '.immoclient_price_make_komma($entity->lagerflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->verkaufsflaeche)){
      $build['businessobject'] ['verkaufsflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Sales area').'</b>'.': '.immoclient_price_make_komma($entity->verkaufsflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->freiflaeche)){
      $build['businessobject']['freiflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Open space').'</b>'.': '.immoclient_price_make_komma($entity->freiflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->bueroflaeche)){
      $build['businessobject']['bueroflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Office space').'</b>'.': '.immoclient_price_make_komma($entity->bueroflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->bueroteilflaeche)){
      $build['businessobject']['bueroteilflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Office space apart').'</b>'.': '.immoclient_price_make_komma($entity->bueroteilflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->verwaltungsflaeche)){
      $build['businessobject']['verwaltungsflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Space for administration').'</b>'.': '.immoclient_price_make_komma($entity->verwaltungsflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->vermietbare_flaeche)){
      $build['businessobject']['vermietbare_flaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Space to rent').'</b>'.': '.immoclient_price_make_komma($entity->vermietbare_flaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->flaechevon)){
      $build['businessobject']['flaechevon'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Area from').'</b>'.': '.immoclient_price_make_komma($entity->flaechevon).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->flaechebis)){
      $build['businessobject']['flaechebis'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Area to').'</b>'.': '.immoclient_price_make_komma($entity->flaechebis).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    $build['gastro'] = array(
      '#type' => 'fieldset',
      '#title' => t('Gastro'),
      '#collapsible' => FALSE,
    );
    if(isset($entity->gastroflaeche)){
      $build ['gastro']['gastroflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Space for gastronomy').'</b>'.': '.immoclient_price_make_komma($entity->gastroflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->plaetze_gastraum)){
      $build ['gastro']['plaetze_gastraum'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Places in the official rooms').'</b>'.': '.immoclient_price_make_komma($entity->plaetze_gastraum),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->anzahl_betten)){
      $build ['gastro']['anzahl_betten'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Amount of beds').'</b>'.': '.$entity->anzahl_betten,
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->anzahl_tagungsraeume)){
      $build['gastro']['anzahl_tagungsraeume'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Amount of meeting rooms').'</b>'.': '.$entity->anzahl_tagungsraeume,
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    $build['grst'] = array(
      '#type' => 'fieldset',
      '#title' => t('Estate'),
      '#collapsible' => FALSE,
    );
    if(isset($entity->grundstuecksflaeche)){
      $build ['grst']['grundstuecksflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Property area').'</b>'.': '.immoclient_price_make_komma($entity->grundstuecksflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->gartenflaeche)){
      $build ['grst']['gartenflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Green area').'</b>'.': '.immoclient_price_make_komma($entity->gartenflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->grz)){
      $build ['grst']['grz'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Site occupancy index').'</b>'.': '.immoclient_price_make_komma($entity->grz),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->gfz)){
      $build ['grst']['gfz'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Floor area ratio').'</b>'.': '.immoclient_price_make_komma($entity->gfz),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->bmz)){
      $build ['grst']['bmz'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Cubic index').'</b>'.': '.immoclient_price_make_komma($entity->bmz),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    $build['ausstattung_fl'] = array(
      '#type' => 'fieldset',
      '#title' => t('Furnishings'),
      '#collapsible' => FALSE,
    );
    if(isset($entity->anzahl_zimmer)){
      $build ['ausstattung_fl']['anzahl_zimmer'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Number of chambers').'</b>'.': '.immoclient_price_make_komma($entity->anzahl_zimmer),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->anzahl_kuechen)){
      $build ['ausstattung_fl']['anzahl_kuechen'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Number of kitchens').'</b>'.': '.immoclient_price_make_komma($entity->anzahl_kuechen),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->anzahl_schlafzimmer)){
      $build ['ausstattung_fl']['anzahl_schlafzimmer'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Number of bedrooms').'</b>'.': '.immoclient_price_make_komma($entity->anzahl_schlafzimmer),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->anzahl_badezimmer)){
      $build ['ausstattung_fl']['anzahl_badezimmer'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Number of bathrooms').'</b>'.': '.immoclient_price_make_komma($entity->anzahl_badezimmer),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->anzahl_sep_wc)){
      $build ['ausstattung_fl']['anzahl_sep_wc'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Number of separated toilets').'</b>'.': '.immoclient_price_make_komma($entity->anzahl_sep_wc),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
     if(isset($entity->anzahl_balkone)){
      $build ['ausstattung_fl']['anzahl_balkone'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Number of balconies').'</b>'.': '.immoclient_price_make_komma($entity->anzahl_balkone),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->anzahl_terrassen)){
      $build ['ausstattung_fl']['anzahl_terrassen'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Number of patios').'</b>'.': '.immoclient_price_make_komma($entity->anzahl_terrassen),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->anzahl_logia)){
      $build ['ausstattung_fl']['anzahl_logia'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Number of loggias').'</b>'.': '.immoclient_price_make_komma($entity->anzahl_logia),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->balkon_terrasse_flaeche)){
      $build ['ausstattung_fl']['balkon_terrasse_flaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Amount of area of all balconies and patios').'</b>'.': '
          .immoclient_price_make_komma($entity->balkon_terrasse_flaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->fensterfront)){
      $build['ausstattung_fl']['fensterfront'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Surface of the windows').'</b>'.': '.immoclient_price_make_komma($entity->fensterfront).' m',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->fensterfront_qm)){
      $build['ausstattung_fl']['fensterfront_qm'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Window frontiere in qm').'</b>'.': '.immoclient_price_make_komma($entity->fensterfront_qm).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
      if(isset($entity->anzahl_wohn_schlafzimmer)){
      $build ['ausstattung_fl']['anzahl_wohn_schlafzimmer'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Amount of all living-bedrooms').'</b>'.': '.immoclient_price_make_komma($entity->anzahl_wohn_schlafzimmer),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->kellerflaeche)){
      $build ['ausstattung_fl']['kellerflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Basement space in qm').'</b>'.': '.immoclient_price_make_komma($entity->kellerflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->grundstuecksfront)){
      $build ['ausstattung_fl']['grundstuecksfront'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Distance to the street in m').'</b>'.': '.immoclient_price_make_komma($entity->grundstuecksfront),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->dachbodenflaeche)){
      $build ['ausstattung_fl']['dachbodenflaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Attic space').'</b>'.': '.immoclient_price_make_komma($entity->dachbodenflaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->beheizbare_flaeche)){
      $build ['ausstattung_fl']['beheizbare_flaeche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Heating area').'</b>'.': '.immoclient_price_make_komma($entity->beheizbare_flaeche).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->anzahl_stellplaetze)){
      $build ['ausstattung_fl']['anzahl_stellplaetze'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Number of parking places').'</b>'.': '.$entity->anzahl_stellplaetze,
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->anzahl_wohneinheiten)){
      $build ['ausstattung_fl']['anzahl_wohneinheiten'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Numbers of living units').'</b>'.': '.immoclient_price_make_komma($entity->anzahl_wohneinheiten),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->anzahl_gewerbeeinheiten)){
      $build ['ausstattung_fl']['anzahl_gewerbeeinheiten'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Numbers of business units').'</b>'.': '.immoclient_price_make_komma($entity->anzahl_gewerbeeinheiten),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if($entity->einliegerwohnung != 0){
    $build ['ausstattung_fl'] ['einliegerwohnung'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Granny flat available?').'</b>'.': '.immoclient_price_bool($entity->einliegerwohnung),
      '#prefix' => '<div style="float:left; padding: 0 5px">',
      '#suffix' => '</div>',
    );
    }
    $build['sonst_fl'] = array(
      '#type' => 'fieldset',
      '#title' => t('Miscalleneous'),
      '#collapsible' => FALSE,
    );
    if(isset($entity->teilbar_ab)){
      $build ['sonst_fl']['teilbar_ab'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Dividable since').'</b>'.': '.immoclient_price_make_komma($entity->teilbar_ab).' m²',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->kubatur)){
      $build ['sonst_fl']['kubatur'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Kubatur').'</b>'.': '.immoclient_price_make_komma($entity->kubatur),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->ausnuetzungsziffer)){
      $build ['sonst_fl']['ausnuetzungsziffer'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Governing plot ratios').'</b>'.': '.immoclient_price_make_komma($entity->ausnuetzungsziffer),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    return $build;
  }
  
}