<?php
/**
 * @file
 * button.func.php
 */

/**
 * Overrides theme_button().
 */
function wetkit_bootstrap_button($variables) {
  $element = $variables['element'];

  // Add icons before or after the value.
  // @see https://drupal.org/node/2219965
  $value = $element['#value'];
  if (!empty($element['#icon'])) {
    if ($element['#icon_position'] === 'before') {
      $value = $element['#icon'] . ' ' . $value;
    }
    elseif ($element['#icon_position'] === 'after') {
      $value .= ' ' . $element['#icon'];
    }
    elseif ($element['#icon_position'] === 'icon-only') {
      $value = $element['#icon'] . '<span class="wb-inv">' . $value . '</span>';
    }
  }

  // This line break adds inherent margin between multiple buttons.
  return '<button' . drupal_attributes($element['#attributes']) . '>' . $value . "</button>\n";
}
