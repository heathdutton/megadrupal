<?php

/**
 * @file Hook documentation for this module.
 */

/**
 * Provide THEME-based image styles for reuse throughout Drupal.
 *
 * This hook allows your theme to provide image styles. This may be useful if
 * you require images to fit within exact dimensions. Note that you should
 * attempt to re-use the default styles provided by Image module whenever
 * possible, rather than creating image styles that are specific to your module.
 * Image provides the styles "thumbnail", "medium", and "large".
 *
 * NOTE: This hook implementation must reside in a file called:
 *
 *   THEMENAME.image_styles.inc
 *
 * Which MUST be at the ROOT of your theme directory.
 *
 * @return array An array of image styles, keyed by the style name.
 *
 * @see image_image_default_styles()
 */
function hook_theme_image_default_styles() {
  $styles = array();

  $styles['mytheme_preview'] = array(
    'label' => 'My theme preview',
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 400, 'height' => 400, 'upscale' => 1),
        'weight' => 0,
      ),
      array(
        'name' => 'image_desaturate',
        'data' => array(),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Modify any image styles provided by other modules, themes or the user.
 *
 * This hook allows THEMES to modify, add, or remove image styles. This may
 * be useful to modify default styles provided by other modules or enforce
 * that a specific effect is always enabled on a style. Note that modifications
 * to these styles may negatively affect the user experience, such as if an
 * effect is added to a style through this hook, the user may attempt to remove
 * the effect but it will be immediately be re-added.
 *
 * The best use of this hook is usually to modify default styles, which are not
 * editable by the user until they are overridden, so such interface
 * contradictions will not occur. This hook can target default (or user) styles
 * by checking the $style['storage'] property.
 *
 * If your module needs to provide a new style (rather than modify an existing
 * one) use hook_theme_image_default_styles() instead.
 *
 * @see hook_theme_image_default_styles()
 */
function hook_theme_image_styles_alter(&$styles) {
  // Check that we only affect a default style.
  if ($styles['thumbnail']['storage'] == IMAGE_STORAGE_DEFAULT) {
    // Add an additional effect to the thumbnail style.
    $styles['thumbnail']['effects'][] = array(
      'name' => 'image_desaturate',
      'data' => array(),
      'weight' => 1,
      'effect callback' => 'image_desaturate_effect',
    );
  }
}