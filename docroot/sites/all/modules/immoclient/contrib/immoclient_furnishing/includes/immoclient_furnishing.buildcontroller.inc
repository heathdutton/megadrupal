<?php


/**
 * Extending the EntityAPIController for the immofurnish entity.
 */
class ImmoclientfurnishEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
//      dpm($entity);
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    
    if(isset($entity->ausstatt_kategorie)&&($entity->ausstatt_kategorie != '0')){
    $build['ausstatt_kategorie'] = array(
      '#type' => 'markup',
    
      '#markup' =>'<b>'. t('Category').'</b>'.': '.immoclient_furnishing_category($entity->ausstatt_kategorie),
    
     
    //  '#prefix' => '<div style="float:left; padding: 0 5px">',
    //  '#suffix' => '</div>',
    );
    }
    $build ['furn_build_basis'] = array(
        '#type' => 'fieldset',
        '#title' => t('House-Appartment-Basics'),
        '#weight' => 2,
    );
    $build ['furn_build_zus_wohn_haus'] = array(
        '#type' => 'fieldset',
        '#title' => t('House-Appartment-Addition'),
        '#weight' => 3,
    );
    $build ['furn_build_waerme'] = array(
        '#type' => 'fieldset',
        '#title' => t('Heating - Conditioned'),
        '#weight' => 4,
    );
    $build ['furn_build_media'] = array(
        '#type' => 'fieldset',
        '#title' => t('Media'),
        '#weight' => 5,
    );
    $build ['furn_build_raeume'] = array(
        '#type' => 'fieldset',
        '#title' => t('Additional rooms'),
        '#weight' => 6,
    );
    $build ['furn_build_alter'] = array(
        '#type' => 'fieldset',
        '#title' => t('Senior-Handicapped'),
        '#weight' => 7,
    );
    $build ['furn_build_gew'] = array(
        '#type' => 'fieldset',
        '#title' => t('Business'),
        '#weight' => 8,
    );
    $build ['furn_build_build_gastro'] = array(
        '#type' => 'fieldset',
        '#title' => t('Gastro'),
        '#weight' => 8,
    );
    if($entity->raeume_veraenderbar != 0){
      $build['furn_build_zus_wohn_haus']['raeume_veraenderbar'] = array(
        '#type' => 'markup',    
        '#markup' => '<b>'. t('Rooms changeable?').'</b>'.': '.immoclient_price_bool($entity->raeume_veraenderbar),     
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->wg_geeignet != 0){
      $build ['furn_build_zus_wohn_haus']['wg_geeignet'] = array(
        '#type' => 'markup',    
        '#markup' => '<b>'. t('Flatshare possible?').'</b>'.': '.immoclient_price_bool($entity->wg_geeignet),     
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->klimatisiert != 0){
      $build['furn_build_waerme'] ['klimatisiert'] = array(
        '#type' => 'markup',    
        '#markup' => '<b>'. t('Air conditioned?').'</b>'.': '.immoclient_price_bool($entity->klimatisiert),     
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->gartennutzung != 0){
      $build['furn_build_zus_wohn_haus']['gartennutzung'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Garden available?').'</b>'.': '.immoclient_price_bool($entity->gartennutzung), 
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->moeb != 'none'){
      $build['furn_build_zus_wohn_haus']['moeb'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Furnitured?').'</b>'.': '.immoclient_furnishing_moeb($entity->moeb),    
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->rollstuhlgerecht != 0){
      $build['furn_build_alter']['rollstuhlgerecht'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Wheelchair suitable?').'</b>'.': '.immoclient_price_bool($entity->rollstuhlgerecht),
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->barrierefrei != 0){
      $build['furn_build_alter']['barrierefrei'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Barrier-free?').'</b>'.': '.immoclient_price_bool($entity->barrierefrei),
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->kabel_sat_tv != 0){
      $build['furn_build_media']['kabel_sat_tv'] = array(
        '#type' => 'markup',   
        '#markup' =>'<b>'.  t('Cable-SAT-TV available?').'</b>'.': '.immoclient_price_bool($entity->kabel_sat_tv), 
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->dvbt != 0){
      $build['furn_build_media']['dvbt'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('DVBT available?').'</b>'.': '.immoclient_price_bool($entity->dvbt),
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->sauna != 0){
      $build['furn_build_zus_wohn_haus']['sauna'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Sauna?').'</b>'.': '.immoclient_price_bool($entity->sauna),     
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->swimmingpool != 0){
      $build['furn_build_zus_wohn_haus']['swimmingpool'] = array(
        '#type' => 'markup',   
        '#markup' =>'<b>'.  t('Swimmingpool?').'</b>'.': '.immoclient_price_bool($entity->swimmingpool),     
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->wasch_trockenraum != 0){
      $build['furn_build_raeume']['wasch_trockenraum'] = array(
        '#type' => 'markup',   
        '#markup' =>'<b>'.  t('Laundryroom?').'</b>'.': '.immoclient_price_bool($entity->wasch_trockenraum),       
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->wintergarten != 0){
      $build['furn_build_raeume']['wintergarten'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Winter garden?').'</b>'.': '.immoclient_price_bool($entity->wintergarten),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->dv_verkabelung != 0){
      $build['furn_build_media']['dv_verkabelung'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('DV wiring?').'</b>'.': '.immoclient_price_bool($entity->dv_verkabelung),        
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->rampe != 0){
      $build['furn_build_alter']['rampe'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Ramp?').'</b>'.': '.immoclient_price_bool($entity->rampe),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->hebebuehne != 0){
      $build['furn_build_alter']['hebebuehne'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Lift?').'</b>'.': '.immoclient_price_bool($entity->hebebuehne),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->kran != 0){
      $build['furn_build_gew']['kran'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Crane?').'</b>'.': '.immoclient_price_bool($entity->kran),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->gastterrasse != 0){
      $build['furn_build_build_gastro']['gastterrasse'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Patios for guests?').'</b>'.': '.immoclient_price_bool($entity->gastterrasse),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->stromanschlusswert != 0){
      $build['furn_build_gew']['stromanschlusswert'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Power connection value').'</b>'.': '.immoclient_price_make_komma($entity->stromanschlusswert).' KW',    
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->kantine_cafeteria != 0){
      $build['furn_build_gew']['kantine_cafeteria'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Canteen - Cafeteria?').'</b>'.': '.immoclient_price_bool($entity->kantine_cafeteria),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->teekueche != 0){
      $build['furn_build_raeume']['teekueche'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Tea kitchen?').'</b>'.': '.immoclient_price_bool($entity->teekueche),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if(isset($entity->hallenhoehe)){
      $build['furn_build_gew']['hallenhoehe'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Hall height').'</b>'.': '.immoclient_price_make_komma($entity->hallenhoehe).' m',         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if(isset($entity->brauereibindung) && $entity->brauereibindung != 0){
      $build['furn_build_build_gastro']['brauereibindung'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Commitment to brewery?').'</b>'.': '.immoclient_price_bool($entity->brauereibindung),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->sporteinrichtungen != 0){
      $build['furn_build_zus_wohn_haus']['sporteinrichtungen'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Sport facilities?').'</b>'.': '.immoclient_price_bool($entity->sporteinrichtungen),       
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->wellnessbereich != 0){
      $build['furn_build_zus_wohn_haus']['wellnessbereich'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Wellness area?').'</b>'.': '.immoclient_price_bool($entity->wellnessbereich),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->telefon_ferienimmobilie != 0){
      $build['furn_build_media']['telefon_ferienimmobilie'] = array(
        '#type' => 'markup',   
        '#markup' =>'<b>'.  t('Telephone at holiday object?').'</b>'.': '.immoclient_price_bool($entity->telefon_ferienimmobilie),    
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->breitband_zugang != 0){
      $build['furn_build_media']['breitband_zugang'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Broadband access?').'</b>'.': '.immoclient_price_bool($entity->breitband_zugang),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->art != 0){
      $build['art'] = array(
        '#type' => 'markup',   
        '#markup' =>'<b>'.  t('Kind of broadband access').'</b>'.': '.check_plain($entity->art), 
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->speed != 0){
      $build['furn_build_media']['speed'] = array(
        '#type' => 'markup',   
        '#markup' =>'<b>'.  t('Speed of broadband access').'</b>'.': '.  immoclient_price_make_komma($entity->speed).' Mbit/Sec', 
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->umts_empfang != 0){
      $build['furn_build_media']['umts_empfang'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('UMTS access?').'</b>'.': '.immoclient_price_bool($entity->umts_empfang),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->unterkellert != 0){
      $build['furn_build_raeume']['unterkellert'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Build with a cellar?').'</b>'.': '.immoclient_furnishing_bool($entity->unterkellert),       
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->abstellraum != 0){
      $build['furn_build_raeume']['abstellraum'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Store room?').'</b>'.': '.immoclient_price_bool($entity->abstellraum),       
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->fahrradraum != 0){
      $build['furn_build_raeume']['fahrradraum'] = array(
        '#type' => 'markup',   
        '#markup' =>'<b>'.  t('Bike room?').'</b>'.': '.immoclient_price_bool($entity->fahrradraum),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->rolladen != 0){
      $build['furn_build_zus_wohn_haus']['rolladen'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Shutter?').'</b>'.': '.immoclient_price_bool($entity->rolladen),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->bibliothek != 0){
      $build['furn_build_zus_wohn_haus']['bibliothek'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Library?').'</b>'.': '.immoclient_price_bool($entity->bibliothek),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->kamin != 0){
      $build['furn_build_waerme']['kamin'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Chimney available?').'</b>'.': '.immoclient_price_bool($entity->kamin),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->dachboden != 0){
      $build['furn_build_raeume']['dachboden'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Attic?').'</b>'.': '.immoclient_price_bool($entity->dachboden),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
     if($entity->gaestewc != 0){
    $build['furn_build_raeume']['gaestewc'] = array(
      '#type' => 'markup',    
      '#markup' =>'<b>'.  t('Guest toilet?').'</b>'.': '.immoclient_price_bool($entity->gaestewc),         
      '#prefix' => '<div style="float:left; padding: 0 5px">',
      '#suffix' => '</div>',
    );
    }
    if($entity->kabelkanaele != 0){
      $build['furn_build_media']['kabelkanaele'] = array(
        '#type' => 'markup',    
        '#markup' =>'<b>'.  t('Cable duct?').'</b>'.': '.immoclient_price_bool($entity->kabelkanaele),         
        '#prefix' => '<div style="float:left; padding: 0 5px">',
        '#suffix' => '</div>',
      );
    }
    if($entity->seniorengerecht != 0){
    $build['furn_build_alter']['seniorengerecht'] = array(
      '#type' => 'markup',    
      '#markup' =>'<b>'.  t('For elderly?').'</b>'.': '.immoclient_price_bool($entity->seniorengerecht),         
      '#prefix' => '<div style="float:left; padding: 0 5px">',
      '#suffix' => '</div>',
    );
    }
    $checkbox1 = '';
    if(isset($entity->field_bad['und'][0]['value'])){
      $entity_bad = $entity->field_bad['und']; 
      if($entity_bad){
      foreach($entity_bad as $key=>$value){
        foreach($value as $key =>$value){
          $trans = immoclient_furnishing_get_translation($value);
          $checkbox1.= $trans.', ';
          }
      }
      $build ['furn_build_basis']['field_bad'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Bath').'</b>'.': '.check_plain($checkbox1),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
      }
    }
    $checkbox2 = '';
    if(isset($entity->field_heizungsart['und'][0]['value'])){
      $entity_bad = $entity->field_heizungsart['und'];
      foreach($entity_bad as $key=>$value){
        foreach($value as $key =>$value){
          $trans = immoclient_furnishing_get_translation($value);
          $checkbox2.=$trans.', ';
        }
      }
      $build ['furn_build_waerme']['field_heizungsart'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Heating').'</b>'.': '.check_plain($checkbox2),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    $checkbox11 = '';
    if(isset($entity->field_fahrstuhl['und'][0]['value'])){
      $entity_bad = $entity->field_fahrstuhl['und'];
      foreach($entity_bad as $key=>$value){
        foreach($value as $key =>$value){
          $trans = immoclient_furnishing_get_translation($value);
          $checkbox11.=$trans.', ';
        }
      }
      $build ['furn_build_zus_wohn_haus']['field_fahrstuhl'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Elevator').'</b>'.': '.check_plain($checkbox11),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
      '#weight' => 100,
    );
    }
    $checkbox3 = ' ';
    if(isset($entity->field_boden['und'][0]['value'])){
      $entity_bad = $entity->field_boden['und'];
      foreach($entity_bad as $key=>$value){
        foreach($value as $key =>$value){
          $trans = immoclient_furnishing_get_translation($value);
          $checkbox3.=$trans.', ';
        }
      }
      $build ['furn_build_basis']['field_boden'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Ground').'</b>'.': '.check_plain($checkbox3),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    $checkbox4 = '';
     if(isset($entity->field_befeuerung['und'][0]['value'])){
      $entity_bad = $entity->field_befeuerung['und'];
      foreach($entity_bad as $key=>$value){
        foreach($value as $key =>$value){
          $trans = immoclient_furnishing_get_translation($value);
          $checkbox4.=$trans.', ';
        }
      }
      $build ['furn_build_waerme']['field_befeuerung'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Firing').'</b>'.': '.check_plain($checkbox4),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    $checkbox5 = '';
    if(isset($entity->field_ausricht_balkon_terrasse['und'][0]['value'])){
      $entity_bad = $entity->field_ausricht_balkon_terrasse['und'];
      foreach($entity_bad as $key=>$value){
        foreach($value as $key =>$value){
          $trans = immoclient_furnishing_get_translation($value);
          $checkbox5.=$trans.', ';
        }
      }
      $build ['furn_build_basis']['field_ausricht_balkon_terrasse'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Direction balcony patio').'</b>'.': '.check_plain($checkbox5),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    $checkbox7 = '';
    if(isset($entity->field_angeschl_gastronomie['und'][0]['value'])){
      $entity_bad = $entity->field_angeschl_gastronomie['und'];
      foreach($entity_bad as $key=>$value){
        foreach($value as $key =>$value){
          $trans = immoclient_furnishing_get_translation($value);
          $checkbox7.=$trans.', ';
          }
      }
      $build ['furn_build_build_gastro']['field_angeschl_gastronomie'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Affiliated gastronomy').'</b>'.': '.check_plain($checkbox7),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    $checkbox8 = '';
    if(isset($entity->field_serviceleistungen['und'][0]['value'])){
      $entity_bad = $entity->field_serviceleistungen['und'];
      foreach($entity_bad as $key=>$value){
        foreach($value as $key =>$value){
          $trans = immoclient_furnishing_get_translation($value);
          $checkbox8.=$trans.', ';
        }
      }
      $build ['furn_build_alter']['field_serviceleistungen'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Services').'</b>'.': '.check_plain($checkbox8),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    $checkbox9 = '';
    if(isset($entity->field_sicherheitstechnik['und'][0]['value'])){
      $entity_bad = $entity->field_sicherheitstechnik['und'];
      foreach($entity_bad as $key=>$value){
        foreach($value as $key =>$value){
          $trans = immoclient_furnishing_get_translation($value);
          $checkbox9.=$trans.', ';
          }
      }
      $build ['furn_build_gew']['field_sicherheitstechnik'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Security technics').'</b>'.': '.check_plain($checkbox9),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    $checkbox10 = '';
    if(isset($entity->field_kueche['und'][0]['value'])){
      $entity_kitchen = $entity->field_kueche['und'];
      foreach($entity_kitchen as $key=>$value){
        foreach($value as $key =>$value){
          $trans = immoclient_furnishing_get_translation($value);
          $checkbox10.= $trans.', ';
        }
      }
      $build ['furn_build_basis']['field_kueche'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Kitchen').'</b>'.': '.check_plain($checkbox10),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    return $build;
  }
  
}

