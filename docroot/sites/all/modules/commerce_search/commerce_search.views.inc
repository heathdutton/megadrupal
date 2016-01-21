<?php

/**
 * Implements hook_views_data_alter().
 */
function commerce_search_views_data_alter(&$data) {
  $data['search_index']['table']['join']['commerce_product'] = array(
    'left_field' => 'product_id',
    'field' => 'sid',
  );

  $data['search_total']['table']['join']['commerce_product'] = $data['search_total']['table']['join']['node'];
  $data['search_dataset']['table']['join']['commerce_product'] = $data['search_dataset']['table']['join']['node'];
}
