<?php

/**
 * @file
 * Page callbacks for the Round Robin module.
 */

/**
 * Returns HTML for a round robin rankings table
 *
 * @param $variables
 *   An associative array containing:
 *   - node: A round robin node object.
 *   - participants: An array of participants ranking data
 *     keyed by participant ID
 *
 */
function theme_round_robin($variables) {
  foreach ($variables['participants'] as $id => $participant) {
    $groups[$participant->gid][] = $participant;
  }

  $header = array(
    array('data' => t('Name'), 'align' => 'left'),
    array('data' => t('Pts'), 'align' => 'center'),
    array('data' => t('Pld'), 'align' => 'center'),
    array('data' => t('W'), 'align' => 'center'),
    array('data' => t('D'), 'align' => 'center'),
    array('data' => t('L'), 'align' => 'center'),
    array('data' => t('RF:RA'), 'align' => 'center'),
  );

  $output = '';
  foreach ($groups as $group => $participants) {
    $rows = array();
    foreach ($participants as $participant) {
      $row = array(
        'name' => array('data' => $participant->name, 'width' => '30%'),
        array('data' => $participant->points, 'align' => 'center', 'width' => '10%'),
        array('data' => $participant->pld, 'align' => 'center', 'width' => '10%'),
        array('data' => $participant->win, 'align' => 'center', 'width' => '10%'),
        array('data' => $participant->draw, 'align' => 'center', 'width' => '10%'),
        array('data' => $participant->loss, 'align' => 'center', 'width' => '10%'),
        array('data' => $participant->rf . ':' . $participant->ra, 'align' => 'center', 'width' => '15%'),
      );

      if (!$participant->status) {
        $row['name'] = '<del>' . $row['name']['data'] . '</del>';
      }

      $rows[] = $row;
    }

    $output .= theme('table', array('rows' => $rows, 'header' => $header, 'caption' => t('Group @group', array('@group' => $group)), 'attributes' => array('id' => 'round-robin-group-' . $group)));
  }

  return $output;
}
