<?php

/**
 * @file
 * A taxonomy term title with safe markup.
 */

$plugin = array(
  'title' => t('Taxonomy Term name as title'),
  'single' => TRUE,
  'icon' => 'icon_term.png',
  'description' => t('Outputs the term name and allows some safe HTML tags in it.'),
  'required context' => new ctools_context_required(t('Term'), 'term'),
  'category' => t('Taxonomy term'),
);

function ns_core_term_title_markup_content_type_render($subtype, $conf, $panel_args, $context) {
  // Get the raw page title.
  $term = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();
  // If we have i18n on, then we need to take that into account.
  if (module_exists('i18nstrings')) {
    $term->name = i18nstrings_string('taxonomy:term:' . $term->tid . ':name', $term->name, FALSE);
  }
  // We use a theme function so that themes can decide how to print it.
  $block->content = theme('ns_core_page_title', array('title' => ns_core_filter_xss($term->name)));
  return $block;
}

function ns_core_term_title_markup_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" term title', array('@s' => $context->identifier));
}

function ns_core_term_title_markup_content_type_edit_form(&$form, &$form_state) {
  // provide a blank form so we have a place to have context setting.
}
