<?php
/**
 * Extending the EntityAPIController for the immoclient_energy entity.
 */
class ImmoclientenergyEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
   
    if(isset($entity->ausweis)){
    $build['ausweis'] = array(
      '#type' => 'markup',
      '#weight' => -1,
      '#markup' => '<b>'.t('Passport available?').'</b>'.': '. immoclient_energy_calc_ausweis($entity->ausweis),
      '#prefix' => '<div style="padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    $build ['energy_build_basis'] = array(
        '#type' => 'fieldset',
        '#title' => t('Energy pass values'),
        '#weight' => 1,
    );
    $build ['energy_build_at'] = array(
        '#type' => 'fieldset',
        '#title' => t('Austria'),
        '#weight' => 2,
    );
    if(isset($entity->epart)){
    $build ['energy_build_basis']['epart'] = array(
      '#type' => 'markup',
      '#weight' => 1,
      '#markup' => '<b>'.t('Calculation basis?').'</b>'.': '.  immoclient_energy_calc_basis($entity->epart),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->gueltig_bis) && ($entity->gueltig_bis != '0')){
    $build['energy_build_basis']['gueltig_bis'] = array(
      '#type' => 'markup',
      '#weight' => 6,
      '#markup' => '<b>'.t('Valid until').'</b>'.': '.date("d.m.Y",($entity->gueltig_bis)),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->energieverbrauchkennwert)){
    $build['energy_build_basis']['energieverbrauchkennwert'] = array(
      '#type' => 'markup',
      '#weight' => 2,
      '#markup' => '<b>'.t('Parameter of energy consumption').'</b>'.': '.  immoclient_price_make_komma($entity->energieverbrauchkennwert).' kWh/(a*m²)',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->mitwarmwasser)){
    $build['energy_build_basis']['mitwarmwasser'] = array(
      '#type' => 'markup',
      '#weight' => 4,
      '#markup' =>'<b>'. t('With warm water?').'</b>'.': '.  immoclient_price_bool($entity->mitwarmwasser),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->endenergiebedarf)){
    $build['energy_build_basis']['endenergiebedarf'] = array(
      '#type' => 'markup',
      '#weight' => 3,
      '#markup' =>'<b>'. t('Final energy demand').'</b>'.': '.check_plain($entity->endenergiebedarf),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    $checkbox1 = '';
    if(isset($entity->field_primaerenergietraeger['und'][0]['value'])){
      $entity_befeuerung = $entity->field_primaerenergietraeger['und'];
      foreach($entity_befeuerung as $key=>$value){
          foreach($value as $key =>$value){
            $trans = immoclient_energy_get_translation($value);
              $checkbox1.=$trans.', ';
          }
      }
      $build ['energy_build_basis']['field_primaerenergietraeger1'] = array(
      '#type' => 'markup',
      '#weight' => 7,
      '#markup' => '<b>'.t('Primary energy carrier').'</b>'.': '.check_plain($checkbox1),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->stromwert)){
    $build['energy_build_basis']['stromwert'] = array(
      '#type' => 'markup',
      '#weight' => 8,
      '#markup' => '<b>'.t('Current value').'</b>'.': '.check_plain($entity->stromwert),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->waermewert)){
    $build['energy_build_basis']['waermewert'] = array(
      '#type' => 'markup',
      '#weight' => 9,
      '#markup' => '<b>'.t('Heating value').'</b>'.': '.check_plain($entity->waermewert),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->wertklasse)){
    $build['energy_build_basis']['wertklasse'] = array(
      '#type' => 'markup',
      '#weight' => 10,
      '#markup' => '<b>'.t('Class').'</b>'.': '.$entity->wertklasse,
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(($entity->ausstelldatum != '0')&&(isset($entity->ausstelldatum))){
      $build['energy_build_basis']['ausstelldatum'] = array(
      '#type' => 'markup',
      '#weight' => 5,
      '#markup' => '<b>'.t('Date of issue').'</b>'.': '.date("d.m.Y",($entity->ausstelldatum)),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->jahrgang)){
    $build['energy_build_basis']['jahrgang'] = array(
      '#type' => 'markup',
      '#weight' => 12,
      '#markup' => '<b>'.t('Year').'</b>'.': '.  immoclient_energy_calc_jg($entity->jahrgang),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->gebaeudeart)){
    $build['energy_build_basis']['gebaeudeart'] = array(
      '#type' => 'markup',
      '#weight' => 11,
      '#markup' => '<b>'.t('Building type').'</b>'.': '.  immoclient_energy_calc_gb($entity->gebaeudeart),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->hwbwert)){
    $build['energy_build_at']['hwbwert'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Heating demand').'</b>'.': '.check_plain($entity->hwbwert),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->hwbklasse)){
    $build['energy_build_at']['hwbklasse'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Class of heating demand').'</b>'.': '.check_plain($entity->hwbklasse),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->fgeewert)){
    $build['energy_build_at']['fgeewert'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Energy efficience factor').'</b>'.': '.check_plain($entity->fgeewert),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->fgeeklasse)){
    $build['energy_build_at']['fgeeklasse'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Class of energy efficience').'</b>'.': '.check_plain($entity->fgeeklasse),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
      '#suffix' => '</div>',
    );
    }
return $build;
  }
  
}