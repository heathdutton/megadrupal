<?php
/**
 * @file
 * Administration pages for the Copy Prevention module.
 */

/**
 * Module settings form.
 */
function copyprevention_settings_form() {
  $form = array();

  $form['copyprevention_body'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Body tag attributes'),
    '#description' => t('Apply these attributes to body tag.'),
    '#options' => array(
      'selectstart' => t('Disable text selection: onselectstart="return false;"'),
      'copy' => t('Disable copy to clipboard: oncopy="return false;"'),
      'contextmenu' => t('Disable right-click context menu: oncontextmenu="return false;"'),
    ),
    '#default_value' => variable_get('copyprevention_body', array()),
  );
  $form['images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Images protection'),
  );
  $form['images']['copyprevention_images'] = array(
    '#type' => 'checkboxes',
    '#description' => t('Apply these methods to images.'),
    '#options' => array(
      'contextmenu' => t('Disable right-click context menu on images: oncontextmenu="return false;"'),
      'transparentgif' => t('Place transparent gif image above all images'),
    ),
    '#default_value' => variable_get('copyprevention_images', array()),
  );
  $form['images']['copyprevention_images_min_dimension'] = array(
    '#type' => 'select',
    '#title' => t('Minimal image dimension'),
    '#description' => t('Minimal image height or width to activate Copy Prevention.'),
    '#options' => array(10 => 10, 20 => 20, 30 => 30, 50 => 50, 100 => 100, 150 => 150, 200 => 200, 300 => 300, 500 => 500),
    '#default_value' => variable_get('copyprevention_images_min_dimension', 150),
  );
  $form['copyprevention_images_search'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Protect/hide images from search engines'),
    '#description' => t('Select options to hide your images from showing up on image searches.'),
    '#options' => array(
      'httpheader' => t('Set "X-Robots-Tag: noimageindex" HTTP header'),
      'pagehead' => t('Add "noimageindex" robots meta tag to page head'),
      'robotstxt' => t('Disallow images (jpg, png, gif) indexing in robots.txt - requires <a href="!link" target="_blank">RobotsTxt</a> module', array('!link' => 'http://drupal.org/project/robotstxt')),
    ),
    '#default_value' => variable_get('copyprevention_images_search', array()),
  );

  return system_settings_form($form);
}
