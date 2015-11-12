<?php

/**
* Plugin definition
*/
$plugin = array(
  'title' => t('Opentable awards'),
  'single' => TRUE,
  'category' => array(t('Opentable')),
);

/**
 * Render Callback
 */
function restaurant_opentable_opentable_awards_content_type_render($subtype, $conf, $args) {
  $block = new stdClass();
  $url = _restaurant_opentable_get_url();
  $info = _restaurant_opentable_get_info($url);

  $block->title = isset($conf['title']) ? $conf['title'] : t('Opentable Awards');
  $block->content = theme('restaurant_opentable_awards', array('awards' => $info['awards']));

  return $block;
}
