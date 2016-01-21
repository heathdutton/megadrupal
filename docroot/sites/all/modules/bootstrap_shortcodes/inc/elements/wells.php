<?php

function bs_shortcodes_shortcode_well($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'well' => ''
          ), $attrs
  );

  $well = url($attrs['well']);
  if ($text) {
    return '<div class="well">' . $text . '</div>';
  } else {
    return $text;
  }
  return $well;
}

function bs_shortcodes_shortcode_well_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[well]This is your normal well area, it has no settings[/well]') . '</strong>';
  if ($long) {
    $output[] = t('Wraps content in a styled well.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Wraps content in a styled well.') . '</p>';
  }

  return implode(' ', $output);
}

// Large Well
function bs_shortcodes_shortcode_well_large($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'well_large' => ''
          ), $attrs
  );

  $well_large = url($attrs['well_large']);
  if ($text) {
    return '<div class="well well-lg">' . $text . '</div>';
  } else {
    return $text;
  }
  return $well_large;
}

function bs_shortcodes_shortcode_well_large_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[well_large]This is your large well area, it has no settings[/well_large]') . '</strong>';
  if ($long) {
    $output[] = t('Wraps content in a large styled well.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Wraps content in a large styled well.') . '</p>';
  }

  return implode(' ', $output);
}

// Small Well
function bs_shortcodes_shortcode_well_small($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'well_small' => ''
          ), $attrs
  );

  $well_small = url($attrs['well_small']);
  if ($text) {
    return '<div class="well well-sm">' . $text . '</div>';
  } else {
    return $text;
  }
  return $well_small;
}

function bs_shortcodes_shortcode_well_small_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[well_small]This is your small well area, it has no settings[/well_small]') . '</strong>';
  if ($long) {
    $output[] = t('Wraps content in a small styled well.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Wraps content in a small styled well.') . '</p>';
  }

  return implode(' ', $output);
}

