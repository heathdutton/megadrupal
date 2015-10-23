<?php

/**
 * Extending the EntityAPIController for the pricedefault entity.
 */
class ImmoclientImpressumEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $build = parent::buildContent($entity, $view_mode, $langcode, $content);

      $build['firmenname'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->firmenname),
      '#prefix' => '<div class="imp_name">',
      '#suffix' => '</div>',
    ); 
      $build['firmenanschrift'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->firmenanschrift),
      '#prefix' => '<div class="imp_addr">',
      '#suffix' => '</div>',
    );
      $build['telefon'] = array(
      '#type' => 'markup',
      '#markup' =>  check_plain($entity->telefon),
      '#prefix' => '<div class="imp_tel">',
      '#suffix' => '</div>',
    );
      $build['vertretungsberechtigter'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->vertretungsberechtigter),
      '#prefix' => '<div class="imp_vertr">',
      '#suffix' => '</div>',
    );
      $build['berufsaufsichtsbehoerde'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->berufsaufsichtsbehoerde),
      '#prefix' => '<div class="imp_beruf">',
      '#suffix' => '</div>',
    );
      $build['handelsregister'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->handelsregister),
      '#prefix' => '<div class="imp_hr">',
      '#suffix' => '</div>',
    );
      $build['handelsregister_nr'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->handelsregister_nr),
      '#prefix' => '<div class="imp_hrnr">',
      '#suffix' => '</div>',
    );
      $build['umsst_id'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->umsst_id),
      '#prefix' => '<div class="imp_umsid">',
      '#suffix' => '</div>',
    );
      $build['steuernummer'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->steuernummer),
      '#prefix' => '<div class="imp_stnr">',
      '#suffix' => '</div>',
    );
      $build['weiteres'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->weiteres),
      '#prefix' => '<div class="imp_weit">',
      '#suffix' => '</div>',
    );
    
    return $build;
  }
  
}

