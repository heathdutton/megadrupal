<?php

/**
 * Extending the EntityAPIController for the pricedefault entity.
 */
class ImmoclientDistancesportEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    
    $build['distancesport_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Distance to sport places'),
      '#collapsible' => FALSE,
    );
      $build['distancesport_fieldset']['distanz_zu_sport'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Distance to sport places').'</b>'.': '.  immoclient_distancesport_option_callback($entity->distanz_zu_sport),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="distance">',
      '#suffix' => '</div>',
    ); 
      $build['distancesport_fieldset']['distancesport_measure'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Distance').'</b>'.': '. $entity->distancesport_measure.' km',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="distance">',
      '#suffix' => '</div>',
    );
    
    return $build;
  }
  
}

