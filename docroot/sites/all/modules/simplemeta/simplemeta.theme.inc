<?php

/**
 * @file
 * Theming
 */

/**
 * Output list of saved SimpleMeta configurations.
 */
function theme_simplemeta_meta_list($vars) {
  $items = $vars['items'];
  $header = array(t('Path'), t('Alias'));
  $language_enabled = variable_get('simplemeta_language_enable', FALSE);
  if ($language_enabled) {
    $header[] = t('Language');
  }
  $header[] = t('Action');

  $rows = array();
  foreach ($items as $meta) {
    $row = array();
    // @todo think about how to determine that path is pattern, like node/%/edit
    $pattern = (strpos($meta->path, '%') !== FALSE);
    $alias = drupal_get_path_alias($meta->path);
    $row[] = (!$pattern) ? l($meta->path, $meta->path) : check_plain($meta->path);
    $row[] = ((!$pattern) && ($alias != $meta->path)) ? l($alias, $meta->path) : '-';
    if ($language_enabled) {
      // Langcode... there is no need to sanitize it.
      $row[] = ($meta->language) ? $meta->language : '-';
    }
    $row[] = l(t('Edit'), 'admin/content/simplemeta/' . $meta->sid . '/edit') . ' | ' . l(t('Delete'), 'admin/content/simplemeta/' . $meta->sid . '/delete');
    $rows[] = $row;
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('There is no saved meta data'), 'colspan' => count($header)));
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Theme meta title for output in the page's head.
 * 
 * Title is set via simplemeta_preprocess_html() i.e. differently from other
 * so no need to return anything, just return empty string.
 */
function theme_simplemeta_meta_title($vars) {
  $meta = $vars['meta'];
  return '';
}

/**
 * Theme meta description for output in the page's head.
 */
function theme_simplemeta_meta_description($vars) {
  $meta = $vars['meta'];
  if (!empty($meta->data['description'])) {
    return '<meta name="description" content="' . trim(strip_tags(decode_entities($meta->data['description']))) . '" />';
  }
  return '';
}

/**
 * Theme meta keywords for output in the page's head.
 */
function theme_simplemeta_meta_keywords($vars) {
  $meta = $vars['meta'];
  if (!empty($meta->data['keywords'])) {
    return '<meta name="keywords" content="' . trim(strip_tags(decode_entities($meta->data['keywords']))) . '" />';
  }
  return '';
}
