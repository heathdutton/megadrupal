<?php

/**
 * Extending the EntityAPIController for the pricedefault entity.
 */
class ImmoclientParkingEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    
    // Our additions to the $build render array.
    $build['stellplatz'] = array(
      '#type' => 'fieldset',
      '#title' => t('Parking space'),
      '#collapsible' => FALSE,
    );
    if($entity->platzart != '0'){
      $build['stellplatz']['platzart'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Parking space').'</b>'.':'.  immoclient_parking_platzart_select($entity->platzart),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    ); 
    }
    if($entity->anzahl != '0'){
      $build['stellplatz']['anzahl'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Number of spaces').'</b>'.':'. $entity->anzahl,
      '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_nk">',
      '#suffix' => '</div>',
    ); 
    }
    return $build;
  }
  
}

