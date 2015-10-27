<?php
// $Id$

/**
 * Override search block form
 *
 * @param array $form
 * @param array $form_state
 */
function gordon_form_search_block_form_alter(&$form, &$form_state) {
  $form['search_block_form']['#default_value'] = t('Search');
  $form['actions']['submit']['#type'] = 'image_button';
  $form['actions']['submit']['#src'] = drupal_get_path('theme', 'gordon') . '/img/search_btn.png';
}

/**
 * Change the breadcrumb
 *
 * @param array $breadcrumb
 * @return string
 */
function gordon_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">' . implode('<span></span>', $breadcrumb) . '</div>';
  }
}

/**
 * Implementation of hook_perprocess_html().
 *
 * Add IE6 no more script.
 *
 * @param array $vars
 */
function gordon_preprocess_html(&$vars) {
  // Add IE6 no more script ot not.
  $ie6nomore = theme_get_setting('gordon_ie6nomore');
  $vars['ie6nomore'] = is_null($ie6nomore) ? 1 : $ie6nomore;
}
