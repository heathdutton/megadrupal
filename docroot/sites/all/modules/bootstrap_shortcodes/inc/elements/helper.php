<?php

function bs_shortcodes_shortcode_helpers($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'style' => '',
      'helpers' => ''
          ), $attrs
  );

  if ($text) {
    $style = empty($attrs['style']) ? check_plain($text) : check_plain($attrs['style']);
    $style = $style;
    return '<p class="text-' . $style . '">' . $text . '</p>';
  }
  return $helpers;
}

function bs_shortcodes_shortcode_helpers_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[helpers style="muted"]See the icon?[/helpers]. Available styles, muted, primary, info, success, danger, warning.') . '</strong>';
  if ($long) {
    $output[] = t('Adds contextual colour to text.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Adds contextual colour to text.') . '</p>';
  }

  return implode(' ', $output);
}
