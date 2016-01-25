<?php

/**
 * @file AUL grants field.
 */

/**
 * Class for AUL grants field.
 */
class views_handler_aul_roles_grants_field extends views_handler_field {
  
  /**
   * Overrides views_handler_filter::options_definition.
   */
  function option_definition() {

    $options = parent::option_definition();

    $options['label'] = array('default' => t('Roles grants'));
    $options['render_label'] = array('default' => TRUE);
    $options['render_label_collon'] = array('default' => TRUE);

    return $options;
  }

  /**
   * Overrides views_handler_field::render.
   */
  public function render($values) {
    
    if(empty($values->nid)) {
      return;
    }
    
    $nid = $values->nid;
        
    // @todo: refactore it. See aul views
    $auls = array(); //aul_collect_node_grants($nid, AUL_ROLES_SOURCE, 'rid');
    
    if(!$auls) {
      return;
    }
    
    $grants = array();
    
    foreach ($auls as $aul) {
      if($access_string = aul_roles_node_access_explain($aul)) {
        $grants[] = $access_string;
      }
    }
    
    return implode('<br/>', $grants);
  }

  /**
   * Overrides views_handler_field::query.
   */
  public function query() {
    // No query overrides.
  }

}
