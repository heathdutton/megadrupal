<?php

/**
 * @file
 * PMB browse category template.
 */

$template .= '<h2>' . t('Subcategories') . '</h2>';

// Display items.
if (count($category->node->node_children)) {
  $header = array();
  $rows = array();
  $chosen_language = 'fr_FR';
  foreach ($category->node->node_children as $child) {
    if (!$child->node_id)
      continue;
    $caption = '';
    foreach ($child->categories as $acategory) {
      if ($acategory->category_lang == $chosen_language) {
        $caption =$acategory->category_caption;
        break;
      }
    }
    if (!$caption) {
      $caption = count($child->categories) ? $child->categories[0]->category_caption : t('Unknown caption');
      if (!$caption) {
        $caption = t('Unknown caption');
      }
    }
    if ($child->is_link)
      $rows[] = array('<i>' . l($caption, _pmb_p('catalog/category/') . $child->node_id) . '</i>');
    else
      $rows[] = array(l($caption, _pmb_p('catalog/category/') . $child->node_id));
  }
  $template .= theme('table', array('header' => $header, 'rows' => $rows));
}
else {
  $template .= t('This category has no subcategories.');
}
$template .= '<br style="clear: both"/>';

// Display linked terms.
if (count($category->node->node_seealso)) {
  $template .= '<h2>' . t('See also') . '</h2>';
  $header = array(t('Caption'));
  $rows = array();
  $chosen_language = 'fr_FR';
  foreach ($category->node->node_seealso as $child) {
    if (!$child->node_id)
      continue;
    $caption = '';
    foreach ($child->categories as $acategory) {
      if ($acategory->category_lang == $chosen_language) {
        $caption = $acategory->category_caption;
        break;
      }
    }
    if (!$caption) {
      $caption = count($child->categories) ? $child->node_seealso[0]->category_caption : t('Unknown caption');
      if (!$caption) {
        $caption = t('Unknown caption');
      }
    }
    $rows[] = array(l($caption, _pmb_p('catalog/category/') . $child->node_id));
  }
  $template .= theme('table', array('header' => $header, 'rows' => $rows));
  $template .= '<br style="clear: both"/>';
}

// Display notices.
$template .= '<h2>' . t('Records (!item)', array('!item' => count($category->notices_ids))) . '</h2>';
if (count($notices)) {
  $template .= '<div style="float: left;" id="category_' . $category->node->node_id . '_notices">';

  $header = array();
  $rows = array();
  foreach ($notices as $anotice) {
    $rows[] = array(
      theme('pmb_view_notice_display', array(
        'notice' => $anotice,
        'display_type' => 'medium_line',
        'parameters' => array(),
    )));
  }
  $template .= theme('table', array('header' => $header, 'rows' => $rows));

  // Display pager.
  $link_maker_function = create_function('$page_number', 'return "' . _pmb_p('catalog/category/') . $category->node->node_id . '/" . $page_number;');

  $template .= theme('pmb_pager', array(
    'current_page' => $parameters['page_number'],
    'page_count' => ceil($parameters['notice_count'] / $parameters['notices_per_pages']),
    'tags' => array(),
    'quantity' => 7,
    'link_generator_callback' => $link_maker_function,
  ));
  $template .= '</div>';
}
else {
  $template .= t('This category has no records.');
}
