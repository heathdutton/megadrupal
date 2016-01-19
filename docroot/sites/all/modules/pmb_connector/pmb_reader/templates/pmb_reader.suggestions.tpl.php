<?php

/**
 * @file
 * PMB reader account suggestions template.
 */

// Display suggestions.
$header = array(
  t('Title'),
  t('Author'),
  t('Publisher'),
  t('State'),
  t('View/Edit'),
);
$rows = array();
foreach ($suggestions as $asuggestion) {
  $link = ($asuggestion->sugg_state == 1) ?
      l(t('View/Edit'), _pmb_p('reader/') . $reader->uid . _pmb_p('/suggestion/') . $asuggestion->sugg_id) :
      l(t('View'), _pmb_p('reader/') . $reader->uid . _pmb_p('/suggestion/') . $asuggestion->sugg_id);
  $rows[] = array(
    check_plain($asuggestion->sugg_title),
    check_plain($asuggestion->sugg_author),
    check_plain($asuggestion->sugg_editor),
    check_plain($asuggestion->sugg_state_caption),
    $link,
  );
}
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No suggestion.')));
$template .= '<br />';

// Display link to add a suggestion.
$template .= l(t('Add a suggestion'), _pmb_p('reader/') . $reader->uid . _pmb_p('/suggestion/add'));
