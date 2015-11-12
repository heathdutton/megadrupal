<?php

/**
 * @file
 * API integration for the cyoutube module.
 */

/**
 * Act on youtube channel names being loaded.
 *
 * @param array $cids
 *   The active fields that hold youtube channel infomation.
 */
function hook_cyoutube_get_channels_info($cids) {
  $return = array();
  foreach ($cids as $cid => $ids) {
    $col = $cid . '_value';
    foreach ($ids as $id) {
      if ($id->entity_type == 'node' && $id->bundle == 'artist') {
        $return[$id->$col] = array('field_artist' => array($id->entity_id));
      }
      if ($id->entity_type == 'node' && $id->bundle == 'show') {
        $return[$id->$col] = array('field_show' => array($id->entity_id));
      }
    }
  }
  return $return;
}
