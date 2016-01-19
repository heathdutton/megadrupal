<?php

/**
 * @file
 * Implement Wysiwyg module hooks.
 */

/**
 * Implements hook_wysiwyg_module_plugin().
 */
function wysiwyg_smart_paging_seo_wysiwyg_smart_paging_seo_plugin() {
  $form = wysiwyg_smart_paging_seo_form();
  $markup = drupal_render($form);
  $plugins['smart_paging_seo'] = array(
    'title' => t('Smart Paging SEO'),
    'vendor url' => 'http://drupal.org/project/wysiwyg_smart_paging_seo',
    'icon title' => t('Add a page break with individual metatag information'),
    'css path' => drupal_get_path('module', 'wysiwyg_smart_paging_seo'),
    'css file' => 'wysiwyg_smart_paging_seo.css',
    'settings'   => array(
      'form_markup' => $markup,
      'icon_markup' => '<img class="wysiwyg-sp-seo-img" title="Smart Page SEO" ' .
        'src="' . drupal_get_path('module', 'wysiwyg_smart_paging_seo') .
        '/plugins/smart_paging_seo/images/placeholder.gif' . '"' .
        'alt="" title="Smart Page SEO" />',
      'pagebreak_markup' => array(
        'prefix' => '<!--pagebreak',
        'suffix' => '-->',
      ),
      'metatag_markup' => array(
        'prefix' => '<!--smartpagingmeta ',
        'suffix' => '-->',
      ),
      'url_markup' => array(
        'prefix' => '<!--smartpagingurl "',
        'suffix' => '"-->',
      ),
    ),
  );

  return $plugins;
}
