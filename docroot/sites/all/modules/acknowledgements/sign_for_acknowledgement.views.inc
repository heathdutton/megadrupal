<?php
/**
 * hook for the Views module (if it is used by your site)
 */
/**
 * Implements hook_views_pre_render().
 */ 
function sign_for_acknowledgement_views_pre_render(&$view) {
  $uid = $GLOBALS['user']->uid;
  if (!$uid) {
    return;
  }
  if (drupal_is_front_page()) {
    return;
  }
  $msg = FALSE;
  foreach ($view->result as $i => $node) {
    //dsm($node);
    if (!isset($node->nid)) {
      continue;
    }
    $nodeobject = node_load($node->nid);
    if (!_sign_for_acknowledgement_applies_to($nodeobject)) {
      continue;
    }
    $timestamp = 0;
    $alternate = '';
    $note = '';
    $is_signed = FALSE;
    if (_sign_for_acknowledgement_already_signed($uid, $node->nid, $timestamp, $alternate, $note)) {
      $is_signed = TRUE;
    }
    $is_alt = FALSE;
    if (isset($nodeobject->alternate_form['und'][0]['value']) && $nodeobject->alternate_form['und'][0]['value'] == 1)
      $is_alt = TRUE;
    $timestamp = _sign_for_acknowledgement_get_expiration_date(TRUE, $node->nid, $nodeobject);
    $is_expired = ($timestamp && variable_get('sign_for_acknowledgement_block_expired', FALSE) && time() > $timestamp);
    $continue = FALSE;
    if (!empty($node->node_title) && !$is_expired && !$is_signed) {
      $node->node_title .= '***';
    } else {
      $continue = TRUE;
    }
    $html = $final_html = '';
    if ($is_signed) {
      $html = SIGNED_OK;
    } else if ($is_expired) {
      $html = TERMS_EXPIRED;
    } else if ($is_alt) {
      $html = t('alternate form to be compiled');
    }
    if (isset($node->field_data_field_sfa_status_node_entity_type)) {
      if (empty($html)) {
        if (variable_get('sign_for_acknowledgement_checkbox_in_views', FALSE)) {
          $ass_array = array();
          $ass_array['my_checkbox'] = drupal_get_form('sign_for_acknowledgement_form_'.$i, $nodeobject, false, false);
          $final_html = theme('sign_for_acknowledgement_views', array ('content' => '', 'assoc' => $ass_array));
        } else {
          $html = TO_BE_SIGNED;
        }
      }
      if (empty($final_html)) {
        $final_html = theme('sign_for_acknowledgement_views', array ('content' => $html));
      }
      $view->result[$i]->field_field_sfa_status[0]['rendered'][] = array ('#markup' => $final_html, '#access' => '1');
    }
    if ($msg == TRUE || $continue == TRUE) {
      continue;
    }
    $msg = TRUE;
    $view->attachment_before = '<span class="unsigned">' . filter_xss(variable_get('sign_for_acknowledgement_marked', t('The documents marked with *** are to be signed by you.'))) . '</span>';
  }
}
