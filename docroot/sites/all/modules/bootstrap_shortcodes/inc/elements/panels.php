<?php

function bs_shortcodes_shortcode_panel($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'panel' => '',
      'heading' => '',
      'style' => '',
          ), $attrs
  );

  $panel = url($attrs['panel']);
  if ($text) {
    if ($attrs['heading'] == '<none>') {
      $heading = '';
      $style = empty($attrs['style']) ? check_plain($text) : check_plain($attrs['style']);
      $style = $style;
      return '<div class="panel panel-' . $style . '"><div class="panel-body">' . $text . '</div></div>';
    } else {
      $style = empty($attrs['style']) ? check_plain($text) : check_plain($attrs['style']);
      $style = $style;
      $heading = empty($attrs['heading']) ? check_plain($text) : check_plain($attrs['heading']);
      $heading = '<div class="panel-heading">' . $heading . '</div>';
      $text = empty($attrs['text']) ? check_plain($text) : check_plain($attrs['text']);
      $text = $text;
      return '<div class="panel panel-' . $style . '">' . $heading . '<div class="panel-body">' . $text . '</div></div>';
    }
    return $panel;
  }

  function bs_shortcodes_shortcode_panel_tip($format, $long) {
    $output = array();
    $output[] = '<p><strong>' . t('[panel]This is your normal panel area, it has no settings[/panel]') . '</strong>';
    if ($long) {
      $output[] = t('Wraps content in a styled panel.') . '</p>';
      $output[] = '<p>&nbsp</p>';
    } else {
      $output[] = t('Wraps content in a styled panel.') . '</p>';
    }

    return implode(' ', $output);
  }

}

