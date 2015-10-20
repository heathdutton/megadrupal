<?php

/**
 * @file
 * PMB reader account reservations template.
 */

global $user;

// Display reservations.
$header = array(
  t('Rank'),
  t('Title'),
  t('Information'),
  t('Action'),
);
$rows = array();
foreach ($reservations as $areservation) {
  $title = '';
  if ($areservation->notice_id) {
    if (isset($parameters['notices'][$areservation->notice_id]['notice']['f']['200'])) {
      foreach ($parameters['notices'][$areservation->notice_id]['notice']['f']['200'] as &$afield) {
        if (!isset($afield['a']))
          continue;
        $title .= $afield['a'] . ' ';
      }
    }
  }
  elseif ($areservation->bulletin_id) {
    if (isset($parameters['bulletins'][$areservation->bulletin_id]['bulletin']->bulletin_title))
      $title = $parameters['bulletins'][$areservation->bulletin_id]['bulletin']->bulletin_title;
  }
  $title = trim($title);
  $notice_link = $title;
  if ($areservation->notice_id)
    $notice_link = l($title, _pmb_p('catalog/record/') . $areservation->notice_id);
  elseif ($areservation->bulletin_id)
    $notice_link = l($title, _pmb_p('catalog/issue/') . $areservation->bulletin_id);

  $resa_caption = '';
  if ($areservation->resa_dateend) {
    if (preg_match('/(\d?\d)\/(\d?\d)\/(\d\d\d\d)/', $areservation->resa_dateend, $m)) {
      $time_stamp = mktime(0, 0, 0, $m[2], $m[1], $m[3]);
      if ($time_stamp < REQUEST_TIME)
        $resa_caption = t('Your reservation is overtime.');
    }
    if (!$resa_caption)
      $resa_caption = t('Your item is holded until the') . ' ' . $areservation->resa_dateend;
  }

  if (!$resa_caption)
    $resa_caption = t('In need of approuval');

  $delete_resa_link = l(t('Delete'), _pmb_p('reader/') . $user->uid . _pmb_p('/reservation/delete/') . $areservation->resa_id);

  $rows[] = array(
    $areservation->resa_rank,
    $notice_link,
    $resa_caption,
    $delete_resa_link,
  );
}
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No reservation.')));
