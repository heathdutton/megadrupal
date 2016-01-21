<?php

/**
 * @file
 * packery.theme.inc
 */

/**
 * Theme declaration for Packery.
 */
function theme_packery($variables) {
  $output = array(
    '#output' => array(
      'instance' => $variables['settings']['id']['default'],
      'settings' => $variables['settings']['instance'],
      'items' => $variables['items']
    ),
    '#pre_render' => array('packery_pre_render_element')
  );

  return drupal_render($output);
}

/**
 * Pre render Packery output.
 */
function packery_pre_render_element($element) {
  $settings = $element['#output']['settings'];
  $instance = $element['#output']['instance'];

  $output = '';
  foreach ($element['#output']['items'] as $key => $item) {
    $output .= theme('packery_item', array(
      'item' => $item['row'],
      'attributes' => array(
        'class' => array('packery-item', 'item-' . $key)
      )
    ));
  }

  // Load Packery settings.
  $config = packery_settings($instance, $settings);

  // Build wrapper with Packery items.
  $element['#markup'] = packery_build($instance, $output);
  $element['#children'] = drupal_render($element['#markup']);
  $element['#attached'] = array(
    'js' => array(
      array(
        'data' => drupal_get_path('module', 'packery') . '/includes/js/packery.settings.js',
        'type' => 'file',
        'scope' => 'footer'
      ),
      array(
        'data' => array('packery' => array(
          $instance => $config
        )),
        'type' => 'setting'
      )
    ),
    'library' => array(
      array(
        'packery',
        'packery'
      ),
    ),
  );

  // Provide support for imagesLoaded plugin.
  if ($config['settings']['imagesloaded']) {
    $element['#attached']['js'][] = array(
      'data' => libraries_get_path('imagesloaded') . '/imagesloaded.pkgd.min.js',
      'type' => 'file',
      'scope' => 'footer'
    );
  }

  return $element;
}

/**
 * Build final output array.
 */
function packery_build($id, $output) {
  return array(
    '#id' => $id,
    '#output' => $output,
    '#theme_wrappers' => array('packery_wrapper'),
  );
}

/**
 * Theme declaration for Packery wrapper.
 */
function theme_packery_wrapper($variables) {
  $attributes = array(
    'id' => $variables['id'], 
    'class' => array('packery')
  );

  return '<div' . drupal_attributes($attributes) . '>' . $variables['output'] . '</div>';
}

/**
 * Theme declaration for Packery item.
 */
function theme_packery_item($variables) {
  return '<div' . drupal_attributes($variables['attributes']) . '>' . $variables['item'] . '</div>';
}
