<?php
/**
 * Extending the EntityAPIController for the immofloor entity.
 */
class ImmoclientFloordescriptionEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
  
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
   $build['geau'] = array(
      '#type' => 'fieldset',
      '#title' => t('Floor - Furnishing'),
      '#collapsible' => FALSE,
    );
    if(isset($entity->geschoss)){
    $build['geau']['geschoss'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Floor').'</b>'.': '.  immoclient_floordescription_geschoss_select($entity->geschoss),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="floordescription">',
      '#suffix' => '</div>',
    );
    }
    if(isset($entity->floordescription)){
    $build['geau']['ausstattung'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Furnishing').'</b>'.': '.check_plain($entity->floordescription),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="floordescription">',
      '#suffix' => '</div>',
    );
    }
    
return $build;
  }
  
}