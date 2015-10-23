<?php

function menu_link_views_data_alter(&$data) {
  unset($data['field_data_menu_link']['menu_link_weight']['sort']['handler']);
  $data['field_data_menu_link']['menu_link_weight']['sort']['handler'] = 'menu_link_handler_sort_weight';
}
