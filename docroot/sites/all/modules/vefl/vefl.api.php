<?php

/**
 * @file
 * Hooks provided by Views exposed form layout module.
 */

/**
 * Provides custom layouts for Views exposed form.
 *
 * @return array
 *   An array with layouts info.
 */
function hook_vefl_layouts() {
  return array(
    'vefl_twocol_stacked' => array(         // Layout machine name.
      'title' => t('Two column stacked'),   // Layout title.
      'regions' => array(                   // Regions.
        'top' => t('Top'),
        'left' => t('Left side'),
        'right' => t('Right side'),
        'bottom' => t('Bottom')
      ),
      'module' => 'VEFL',                   // Optional: Used in Layouts selectbox to separate
    ),                                      // layouts by categories. Will be "VEFL" if empty.
    'vefl_twocol_bricks' => array(
      'title' => t('Two column bricks'),
      'regions' => array(
        'top' => t('Top'),
        'left_above' => t('Left above'),
        'right_above' => t('Right above'),
        'middle' => t('Middle'),
        'left_below' => t('Left below'),
        'right_below' => t('Right below'),
        'bottom' => t('Bottom'),
      ),
      'module' => 'VEFL',
    ),
  );
}
