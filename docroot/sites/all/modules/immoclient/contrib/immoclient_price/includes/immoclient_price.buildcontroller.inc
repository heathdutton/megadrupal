<?php

/**
 * Extending the EntityAPIController for the pricedefault entity.
 */
class ImmoclientPricedefaultEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    
    $build['pricedef_kauf'] = array(
      '#type' => 'fieldset',
      '#title' => t('Price'),
      '#collapsible' => FALSE,
    );
    $build['pricedef_courtage'] = array(
      '#type' => 'fieldset',
      '#title' => t('Courtage'),
      '#collapsible' => FALSE,
    );
    $build['pricedef_miete'] = array(
      '#type' => 'fieldset',
      '#title' => t('Rent'),
      '#collapsible' => FALSE,
    );
    $build['pricedef_nk'] = array(
      '#type' => 'fieldset',
      '#title' => t('Additionel costs'),
      '#collapsible' => FALSE,
    );  
    $build['pricedef_rendite'] = array(
      '#type' => 'fieldset',
      '#title' => t('Yield'),
      '#collapsible' => FALSE,
    );
    $build['pricedef_at'] = array(
      '#type' => 'fieldset',
      '#title' => t('Austria'),
      '#collapsible' => FALSE,
    );
    $build['pricedef_pacht'] = array(
      '#type' => 'fieldset',
      '#title' => t('Lease'),
      '#collapsible' => FALSE,
    );
    
    $build['pricedef_gr'] = array(
      '#type' => 'fieldset',
      '#title' => t('Estate'),
      '#collapsible' => FALSE,
    );
    if($entity->kaufpreis != 0){
      $build['pricedef_kauf']['kaufpreis'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Price').'</b>'.': '.immoclient_price_make_money($entity->kaufpreis),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
        '#suffix' => '</div>',
      );
    }
    if($entity->richtpreis != 0){
      $build['pricedef_kauf']['richtpreis'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Suggested price').'</b>'.': '.immoclient_price_make_money($entity->richtpreis),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
        '#suffix' => '</div>',
      );
    }
    if($entity->auf_anfrage != 0){
      $build['pricedef_kauf']['auf_anfrage'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('On demand').'</b>'.': '.immoclient_price_bool($entity->auf_anfrage),
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->vb != 0){
      $build['pricedef_kauf']['vb'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Negotiable').'</b>'.': '.immoclient_price_bool($entity->vb),
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->kaufpreisnetto != 0){
      $build['pricedef_kauf']['kaufpreisnetto'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Price net').'</b>'.': '.immoclient_price_make_money($entity->kaufpreisnetto),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreisnetto">',
        '#suffix' => '</div>',
      );
    }
    if($entity->kaufpreisust != 0){
      $build['pricedef_kauf']['kaufpreisust'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('VAT').'</b>'.': '.immoclient_price_make_money($entity->kaufpreisust),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreisnetto">',
        '#suffix' => '</div>',
      );
    }
    if($entity->kaufpreisbrutto != 0){
      $build['pricedef_kauf']['kaufpreisbrutto'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Gross Price').'</b>'.': '.immoclient_price_make_money($entity->kaufpreisbrutto),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreisbrutto">',
        '#suffix' => '</div>',
      );
    }
    if($entity->nettokaltmiete != 0){
      $build['pricedef_miete']['nettokaltmiete'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Rent excluded utilities').'</b>'.': '.immoclient_price_make_money($entity->nettokaltmiete),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_miete">',
        '#suffix' => '</div>',
      );
    }
    if($entity->kaltmiete != 0){
      $build['pricedef_miete']['kaltmiete'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Rent including utlilities excluding heating').'</b>'.': '.immoclient_price_make_money($entity->kaltmiete),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_miete">',
        '#suffix' => '</div>',
      );
    }
    if($entity->warmmiete != 0){
      $build['pricedef_miete']['warmmiete'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Rent including heating').'</b>'.': '.immoclient_price_make_money($entity->warmmiete),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_miete">',
        '#suffix' => '</div>',
      );
    }
    if($entity->nebenkosten != 0){
      $build['pricedef_nk']['nebenkosten'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Monthly utlilities').'</b>'.': '.immoclient_price_make_money($entity->nebenkosten),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->heizkosten_enthalten != 1){
      $build['pricedef_nk']['heizkosten_enthalten'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Heating costs included').'</b>'.': '.immoclient_price_bool($entity->heizkosten_enthalten),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->heizkosten != 0){
      $build['pricedef_nk']['heizkosten'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Monthly Heating Costs - advance payment').'</b>'.': '.immoclient_price_make_money($entity->heizkosten),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->zzg_mehrwertsteuer != 0){
      $build['pricedef_nk']['zzg_mehrwertsteuer'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Plus VAT - one time per building').'</b>'.': '.immoclient_price_bool($entity->zzg_mehrwertsteuer),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->mietzuschlaege != 0){
      $build['pricedef_nk']['mietzuschlaege'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Rent additions').'</b>'.': '.immoclient_price_make_money($entity->mietzuschlaege),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->hauptmietzinsnetto != 0){
      $build['pricedef_at']['hauptmietzinsnetto'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Main rent interest net').'</b>'.': '.immoclient_price_make_money($entity->hauptmietzinsnetto),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->hauptmietzinsust != 0){
      $build['pricedef_at']['hauptmietzinsust'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Main rent interest tax').'</b>'.': '.immoclient_price_make_money($entity->hauptmietzinsust),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
   }
    if($entity->pauschalmiete != 0){
      $build['pricedef_miete']['pauschalmiete'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('All in rent').'</b>'.': '.immoclient_price_make_money($entity->pauschalmiete),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->betriebskostennetto != 0){
      $build['pricedef_at']['betriebskostennetto'] = array(
      '#type' => 'markup',
      '#markup' =>'<b>'. t('Cost of ownership').'</b>'.': '.immoclient_price_make_money($entity->betriebskostennetto),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
      );
    }
    if($entity->betriebskostenust != 0){
      $build['pricedef_at']['betriebskostenust'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Cost of ownership VAT').'</b>'.': '.immoclient_price_make_money($entity->betriebskostenust),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->pacht != 0){
      $build['pricedef_pacht']['pacht'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Lease').'</b>'.': '.immoclient_price_make_money($entity->pacht),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
   }
   if($entity->erbpacht != 0){
      $build['pricedef_pacht']['erbpacht'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Leasehold').'</b>'.': '.immoclient_price_make_money($entity->erbpacht),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->hausgeld != 0){
       $build['pricedef_rendite']['hausgeld'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Rent payment').'</b>'.': '.immoclient_price_make_money($entity->hausgeld),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->abstand != 0){
      $build['pricedef_rendite']['abstand'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Agio').'</b>'.': '.immoclient_price_make_money($entity->abstand),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->preis_zeitraum_von != 0){
      $build['pricedef_kauf']['preis_zeitraum_von'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Price from date').'</b>'.': '.date("d.m.Y",($entity->preis_zeitraum_von)),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->preis_zeitraum_bis != 0){
      $build['pricedef_kauf']['preis_zeitraum_bis'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Price until date').'</b>'.': '.date("d.m.Y",($entity->preis_zeitraum_bis)),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->preis_zeiteinheit != '0'){      
      $build['pricedef_kauf']['preis_zeiteinheit'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Price time unit').'</b>'.': '.check_plain($entity->preis_zeiteinheit),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
   
    if($entity->aussen_courtage != '0' && $entity->aussen_courtage != ''){
      $build['pricedef_courtage']['aussen_courtage'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Commission value from customer').'</b>'.': '.  immoclient_price_make_komma($entity->aussen_courtage),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->mit_mwst_a != '0' && $entity->aussen_courtage != '0' && $entity->aussen_courtage != ''){
      $build['pricedef_courtage']['mit_mwst_a'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Commission value from customer with VAT').'</b>'.': '.immoclient_price_bool($entity->mit_mwst_a),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->innen_courtage != '0'){
      $build['pricedef_courtage']['innen_courtage'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Commission value from seller').'</b>'.': '.immoclient_price_make_komma($entity->innen_courtage),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->mit_mwst != '0'&& $entity->innen_courtage != '0' && $entity->innen_courtage != ''){
      $build['pricedef_courtage']['mit_mwst'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Commission value from seller with VAT').'</b>'.': '.immoclient_price_bool($entity->mit_mwst),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->mwst_satz != '0.19'){
      $build['pricedef_courtage']['mwst_satz'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('VAT').'</b>'.': '.immoclient_price_make_komma($entity->mwst_satz),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->mwst_gesamt != '0'){
      $build['pricedef_courtage']['mwst_gesamt'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('VAT as amount').'</b>'.': '.immoclient_price_make_komma($entity->mwst_gesamt),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->provisionspflichtig != '0'){
      $build['pricedef_courtage']['provisionspflichtig'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Subject to commission').'</b>'.': '.immoclient_price_bool($entity->provisionspflichtig),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->provision_teilen != '0'){
      $build['pricedef_courtage']['provision_teilen'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Share commission').'</b>'.': '.immoclient_price_bool($entity->provision_teilen),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->provision_teilen_wert != ''){
      $build['pricedef_courtage']['provision_teilen_wert'] = array(
        '#type' => 'markup',
        '#markup' =>'<b>'. t('Share commission value').'</b>'.': '.check_plain($entity->provision_teilen_wert),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->courtage_hinweis != ''){
      $build['pricedef_courtage']['courtage_hinweis'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Useful infos about commission').'</b>'.':<br/>'.check_plain($entity->courtage_hinweis),
        '#prefix' => '<div style="clear:left;float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->waehrung != 'EUR'){
      $build['pricedef_courtage']['waehrung'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Currency').'</b>'.':<br/>'.check_plain($entity->waehrung),
        '#prefix' => '<div style="clear:left;float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->x_fache != ''){
      $build['pricedef_rendite']['x_fache'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('n-fold').'</b>'.':'.check_plain($entity->x_fache),
        '#prefix' => '<div style="clear:both; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->mietpreis_pro_qm != 0){
      $build['pricedef_rendite']['mietpreis_pro_qm'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Rent per square meter').'</b>'.':'.immoclient_price_make_money($entity->mietpreis_pro_qm),
        '#prefix' => '<div style="clear:both; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->kaufpreis_pro_qm != 0){
      $build['pricedef_rendite']['kaufpreis_pro_qm'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Price per square meter').'</b>'.':'.immoclient_price_make_money($entity->kaufpreis_pro_qm),
        '#prefix' => '<div style="clear:both; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->nettorendite != '0'){
      $build['pricedef_rendite']['nettorendite'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Net yield').'</b>'.':'.immoclient_price_make_komma($entity->nettorendite).'%',
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->nettorendite_soll != '0'){
      $build['pricedef_rendite']['nettorendite_soll'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Net yield target').'</b>'.':'.immoclient_price_make_komma($entity->nettorendite_soll).'%',
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->nettorendite_ist != '0'){
      $build['pricedef_rendite']['nettorendite_ist'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Actual net yield').'</b>'.':'.immoclient_price_make_komma($entity->nettorendite_ist).'%',
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->mieteinnahmen_soll != '0'){
      $build['pricedef_rendite']['mieteinnahmen_soll'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Net yield target').'</b>'.':'.immoclient_price_make_money($entity->mieteinnahmen_soll),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->mieteinnahmen_soll != '0'){
      $build['pricedef_rendite']['periode_soll'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Time intervall').'</b>'.':'.  check_plain($entity->periode_soll),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->mieteinnahmen_ist != '0'){
      $build['pricedef_rendite']['mieteinnahmen_ist'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Actual net yield').'</b>'.':'.  immoclient_price_make_money($entity->mieteinnahmen_ist),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
    if($entity->mieteinnahmen_ist != '0'){
      $build['pricedef_rendite']['periode'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Time intervall').'</b>'.':'. check_plain($entity->periode),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      );
    }
      if($entity->erschliessungskosten != '0'){
      $build['pricedef_gr']['erschliessungskosten'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Development cost').'</b>'.':'.  immoclient_price_make_money($entity->erschliessungskosten),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      ); 
    }
    if($entity->kaution != '0'){
      $build['pricedef_miete']['kaution'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Deposit').'</b>'.':'.  immoclient_price_make_money($entity->kaution),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      ); 
    }
    if($entity->kaution_text != ''){
        $build['pricedef_miete']['kaution_text'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Deposit description').'</b>'.':'. check_plain($entity->kaution_text),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      ); 
    }
    if($entity->geschaeftsguthaben != '0'){
      $build['pricedef_rendite']['geschaeftsguthaben'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Business assets').'</b>'.':'.  immoclient_price_make_money($entity->geschaeftsguthaben),
        '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
        '#suffix' => '</div>',
      ); 
    }
    return $build;
  }
  
}