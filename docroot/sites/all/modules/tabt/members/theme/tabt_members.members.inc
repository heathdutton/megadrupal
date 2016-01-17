<?php
/**
 * @file
 * Theme the memberlist.
 */

/**
 * Theme the memberlist.
 */
function theme_tabt_members($variables) {
  $members = $variables['members'];

  $output = '';
  $member_seasons = tabt_members_get_seasons();
  $member_seasons[$variables['season']] = TRUE;

  $output .= '<center>' . tabt_get_seasons_form($variables['season'], variable_get('tabt_members_memberspage', 'members'), $member_seasons) . '</center>';

  $header = array(
    '#',
    t('Index'),
    t('Player Id'),
    t('Name'),
    t('Rank', array(), array('context' => 'game')),
    t('Age<br />Category'),
    t('Gender')
  );
  $rows = array();
  foreach ($members as $member) {
    $row = array(
      check_plain($member->Position),
      check_plain($member->RankingIndex),
      check_plain($member->UniqueIndex)
    );
    if ($member->Link) {
      $row[] = l($member->Name, $member->Link);
    }
    else {
      $row[] = check_plain($member->Name);
    }
    array_push(
      $row,
      str_replace('NC', t('NC', array(), array('context' => 'abbreviation Not Classified')), check_plain($member->Ranking)),
      check_plain($member->Category),
      check_plain($member->Gender)
    );
    $rows[] = $row;
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= _tabt_get_footer();
  return $output;
}
