<?php

function menuimage_views_views_data() {
  $data = array();
  $data['menu_links']['menuimage'] = array(
    'title' => t('Image'),
    'help' => t('from menuimage module, via menuimage_views'),
    'field' => array(
      'handler' => 'menuimage_views_menuimage_handler',
    ),
  );
  return $data;
}
?>