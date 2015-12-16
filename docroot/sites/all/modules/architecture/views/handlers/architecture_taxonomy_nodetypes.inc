<?php

/**
 * @file
 * Views handler override for field that exposes taxonomy nodetypes.
 */


/**
 * @inheritdoc
 */
class architecture_taxonomy_nodetype_views_handler extends views_handler_field {

  /**
  * Render the nodetypes for this term's vocabulary.
  */
  function render($values) {
    $vid = $this->get_value($values);
    $vocabularies = taxonomy_get_vocabularies();
    $vocabulary = $vocabularies[$vid];
    $nodetypes = array();
    $return = '';

    // Find all the fields that use term references.
    $fields = db_select('field_config', 'fc')
      ->fields('fc', array('field_name', 'data'))
      ->condition('type', 'taxonomy_term_reference','=')
      ->execute()
      ->fetchAllAssoc('field_name');

    foreach($fields as $field) {
      $data = unserialize($field->data);
      foreach ($data['settings']['allowed_values'] as $allowed_values) {
        // Find the bundles that the fields appear on, that use current vocab.
        if($allowed_values['vocabulary'] == $vocabulary->machine_name) {
          $bundles = db_select('field_config_instance', 'fci')
            ->fields('fci', array('bundle'))
            ->condition('field_name', $field->field_name,'=')
            ->execute()
            ->fetchAllAssoc('bundle');
          foreach($bundles as $bundle) {
            $nodetypes[] = $bundle->bundle;
          }
        }
      }
    }
    // Make a nice list of nodetypes.
    sort($nodetypes);
    $nodetype_names = node_type_get_types();
    foreach(array_unique(($nodetypes)) as $nodetype) {
      $return .= $nodetype_names[$nodetype]->name . ', ';
    }
    return rtrim($return, " ,");
  }
}
