<?php

/**
 * @file
 * Implement Wysiwyg module hooks.
 */

/**
 * Implements hook_wysiwyg_module_plugin().
 */
function wysiwyg_boxout_boxout_plugin() {
  $form = drupal_get_form('wysiwyg_boxout_form');
  $markup = drupal_render($form);
  $plugins['boxout'] = array(
    'title' => t('Boxout'),
    'vendor url' => 'http://drupal.org/project/wysiwyg_boxout',
    'icon title' => t('Add Boxout to content'),
    'css path' => drupal_get_path('module', 'wysiwyg_boxout'),
    'css file' => 'wysiwyg_boxout.css',
    'settings'   => array(
      'form_markup' => $markup,
      'boxout_markup' =>
        '<div class="boxout [style]">
          <[element_type] class="boxout-title">[header]</[element_type]>
          [body]
        </div>',
    ),
  );
  return $plugins;
}
