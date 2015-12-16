<?php

function bs_shortcodes_shortcode_abbreviation($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'abbreviation' => '',
      'title' => '',
          ), $attrs
  );

  $abbreviation = url($attrs['abbreviation']);
  if ($text) {
    $title = empty($attrs['title']) ? check_plain($text) : check_plain($attrs['title']);
    $title = $title;
    return '<abbr title="' . $title . '">' . $text . '</abbr>';
  }
  return $abbreviation;
}

function bs_shortcodes_shortcode_abbreviation_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[abbreviation title="HyperText Markup Language"]HTML[/abbreviation]') . '</strong>';
  if ($long) {
    $output[] = t('Wraps content as a styled abbreviation.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Wraps content as a styled abbreviation.') . '</p>';
  }

  return implode(' ', $output);
}

