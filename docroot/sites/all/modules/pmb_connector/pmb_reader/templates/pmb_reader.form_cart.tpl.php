<?php

/**
 * @file
 * PMB reader cart form template.
 */

// Display reader cart.
$header = array();
$rows = array();
foreach ($form['notices_ids']['#value'] as $anotice_id => $anotice) {
  $rows[] = array(
    array(
      'data' => drupal_render($form['checked_notices'][$anotice_id]),
      'class' => array('checkbox'),
    ),
    theme('pmb_view_notice_display', array(
      'notice' => $anotice,
      'display_type' => 'medium_line',
      'parameters' => array(),
  )));
}
if ($rows) {
  $template .= theme('table', array('header' => $header, 'rows' => $rows));

  $template .= drupal_render_children($form);
}
else {
  $template .= t('Your cart is empty.') . '<br /><br />';
}
