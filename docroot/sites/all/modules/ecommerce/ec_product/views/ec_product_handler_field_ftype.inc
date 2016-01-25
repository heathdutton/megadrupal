<?php

/**
 * @file
 * Handles the display of product features.
 */

class ec_product_handler_field_ftype extends views_handler_field_prerender_list {
  function construct() {
    parent::construct();
    $this->additional_fields['ptype'] = array(
      'table' => 'ec_product',
      'field' => 'ptype',
    );
  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['ptype'];
  }

  function pre_render($values) {
    $ptypes = array();
    $this->items = array();

    foreach ($values as $result) {
      $ptypes[$result->{$this->aliases['ptype']}] = $result->{$this->aliases['ptype']};
    }

    if (!empty($ptypes)) {
      $reault = db_select('ec_product_features')
        ->fields(
          array(
            'ptype',
            'ftype',
          )
        )
        ->condition('ptype', $ptypes, 'IN')
        ->fetchAll();
      foreach ($result as $feature) {
        $this->items[$feature->ptype][$feature->ftype] = ec_product_feature_get('name', $feature->ftype);
      }
    }
  }
}
