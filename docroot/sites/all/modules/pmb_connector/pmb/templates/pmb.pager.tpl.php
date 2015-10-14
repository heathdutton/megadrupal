<?php

/**
 * @file
 * PMB pager template.
 */

$values = array();
for ($i = max(1, $current_page - ceil($quantity / 2)); $i <= min($current_page + ceil($quantity / 2), $page_count); $i++) {
  $values[] = $i;
}

$items = array();

if ($current_page > 2) {
  $items[] = array(
    'class' => array('pager-first'),
    'data' => l(t('<< first'), $link_generator_callback(1)),
  );
}

if ($current_page > 1) {
  $items[] = array(
    'class' => array('pager-previous'),
    'data' => l(t('< previous'), $link_generator_callback($current_page - 1)),
  );
}

foreach ($values as $avalue) {
  $items[] = array(
    'class' => array('pager-item'),
    'data' => l($avalue, $link_generator_callback($avalue)),
  );
}

if ($current_page < $page_count) {
  $items[] = array(
    'class' => array('pager-next'),
    'data' => l(t('next >'), $link_generator_callback($current_page + 1)),
  );
}

if ($current_page < $page_count - 1) {
  $items[] = array(
    'class' => array('pager-last'),
    'data' => l(t('last (!count) >>', array('!count' => $page_count)), $link_generator_callback($page_count)),
  );
}

$template .= theme('item_list', array(
  'items' => $items,
  'title' => NULL,
  'type' => 'ul',
  'attributes' => array(
    'class' => array('pager'),
)));
