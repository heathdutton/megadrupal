<?php

function uc_cartlinksbuilder_product_autocomplete($string = '') {
  if (empty($string)) {
    return NULL;
  }
 
  $sql = "SELECT n.nid, n.title, n.type, p.model FROM {node} n JOIN {uc_products} p on p.nid = n.nid WHERE n.title LIKE :string";
  $result = db_query($sql, array(':string' => '%' . $string . '%'));


  $matches = array();
  while ($row = db_fetch_object($result)) {
    $nid = $row->nid;
    // Add a class wrapper for a few required CSS overrides.
    $matches[$row->title . " [nid:$nid]"] = sprintf('(%s) %s', $row->model, $row->title);
  }
  drupal_json_encode($matches);
}
