<?php
/**
 * @file
 * stickynot-confirm.theme.inc
 */


/**
 * Return confirmation links for deletion.
 *
 * @param array $variables
 *   Sticky note tokens and configuration form.
 */
function wetkit_omega_stickynote_confirm($variables) {
  $stickynote = $variables['stickynote'];
  $output = '';

  $token = drupal_get_token($stickynote->snid);
  $output .= '<span id="confirm-delete-' . $stickynote->snid . '" style="text-align: left;">';
  $output .= 'Are you sure?&nbsp;';
  $output .= l(t('Delete'), 'stickynote/' . $stickynote->snid . '/delete', array(
    'attributes' => array('class' => array('use-ajax')),
    'query' => array('token' => $token),
  ));
  $output .= '&nbsp;&nbsp;&nbsp;&nbsp;';
  $output .= l(t('Cancel'), 'stickynote/' . $stickynote->snid . '/cancel', array(
    'attributes' => array('class' => array('use-ajax'))));
  $output .= '</span>';

  return $output;
}
