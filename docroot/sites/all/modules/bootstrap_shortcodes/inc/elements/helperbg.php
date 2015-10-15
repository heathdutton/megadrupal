<?php

function bs_shortcodes_shortcode_helpersbg($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'style' => '',
      'helpersbg' => ''
          ), $attrs
  );

  if ($text) {
    $style = empty($attrs['style']) ? check_plain($text) : check_plain($attrs['style']);
    $style = $style;
    return '<p class="bg-' . $style . '">' . $text . '</p>';
  }
  return $helpersbg;
}

function bs_shortcodes_shortcode_helpersbg_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[helpersbg style="primary"]Has a blue background, see?[/helpersbg]. Available styles, primary, info, success, danger, warning.') . '</strong>';
  if ($long) {
    $output[] = t('Adds contextual background colour to text.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Adds contextual background colour to text.') . '</p>';
  }

  return implode(' ', $output);
}
