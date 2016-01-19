<?php

/**
 * @file
 * PMB block pager template.
 */

$items = array();

if ($current_page > 2) {
  $items[] = array(
    'class' => array('pager-first'),
    'data' => l(t('<<'), $link_generator_callback(1)),
    'onclick' => "pmb_block_get_page('" . $id . "', '" . $link_generator_callback(1) . "'); return false;",
  );
}

if ($current_page > 1) {
  $items[] = array(
    'class' => array('pager-previous'),
    'data' => l(t('<'), $link_generator_callback($current_page - 1)),
    'onclick' => "pmb_block_get_page('" . $id . "', '" . $link_generator_callback($current_page - 1) . "'); return false;",
  );
}

$items[] = array(
  'class' => array('pager-item'),
  'data' => l($current_page, $link_generator_callback($current_page)),
  'onclick' => "pmb_block_get_page('" . $id . "', '" . $link_generator_callback($current_page) . "'); return false;",
);

if ($current_page < $page_count) {
  $items[] = array(
    'class' => array('pager-next'),
    'data' => l(t('>'), $link_generator_callback($current_page + 1)),
    'onclick' => "pmb_block_get_page('" . $id . "', '" . $link_generator_callback($current_page + 1) . "'); return false;",
  );
}

if ($current_page < $page_count - 1) {
  $items[] = array(
    'class' => array('pager-last'),
    'data' => l(t('>>'), $link_generator_callback($page_count)),
    'onclick' => "pmb_block_get_page('" . $id . "', '" . $link_generator_callback($page_count) . "'); return false;",
  );
}

$template .= theme('item_list', array(
  'items' => $items,
  'title' => NULL,
  'type' => 'ul',
  'attributes' => array(
    'class' => array('pager'),
)));
