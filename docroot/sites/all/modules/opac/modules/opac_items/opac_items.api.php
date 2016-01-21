<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * This hook is invoked from theme_opac_items() function.
 *
 * Use it to add row or column in the table.
 *
 * @param array $data
 *   Items data.
 *
 * @ingroup hooks
 */
function hook_opac_items_view_alter(&$data) {
  $data['mapping'][] = array(
    'item_field' => 'hold_link',
    'display_header' => 'Operations',
    'weight' => '50',
  );
  $nid = $data['nid'];

  foreach ($data['items'] as $itemnumber => $item) {
    if (!empty($item['canhold'])) {
      $data['items'][$itemnumber]['hold_link'] = l(t('hold', array(), array('context' => 'libraries')), "node/$nid/holditem/$itemnumber");
    }
    else {
      $data['items'][$itemnumber]['hold_link'] = t('Not available for hold', array(), array('context' => 'libraries'));
    }
  }
}
