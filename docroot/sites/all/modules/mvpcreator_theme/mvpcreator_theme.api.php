<?php

/**
 * @file
 * Contains documentation about the MVPCreator Theme module's hooks.
 */

/**
 * @defgroup mvpcreator_theme MVPCreator Theme
 * @{
 */

/**
 * Register color schemes for use by MVPCreator Theme.
 *
 * This will affect the color options shown in the 'Background color' and
 * 'Background image' style plugins.
 *
 * The chosen color will add a class to the region or pane based on the
 * machine name of the color. For example, if the user choses the 'blue'
 * color scheme as the 'Background color' for a region, then this class
 * 'region-mvpcreator-theme-bgcolor-blue' will be added.
 *
 * @return array
 *   An associative array with a unique machine name as key and a human
 *   readable label as the value.
 *
 * @see hook_mvpcreator_theme_colors_alter()
 */
function hook_mvpcreator_theme_colors() {
  return array(
    'blue' => t('Blue'),
    'orange' => t('Orange'),
    'black_and_white' => t('Black and white'),
  );
}

/**
 * Alter the list of color schemes for use by MVPCreator Theme.
 *
 * This will affect the color options shown in the 'Background color' and
 * 'Background image' style plugins.
 *
 * If you remove the 'custom' scheme, then users can't specific an arbitrary
 * color.
 *
 * @param array &$colors
 *   An associative array of color schemes like the one returned by
 *   hook_mvpcreator_theme_colors().
 *
 * @see hook_mvpcreator_theme_colors().
 */
function hook_mvpcreator_theme_colors_alter(&$colors) {
  // We don't want our users specifying any old color!
  unset($colors['custom']);
}
 
/**
 * @}
 */
