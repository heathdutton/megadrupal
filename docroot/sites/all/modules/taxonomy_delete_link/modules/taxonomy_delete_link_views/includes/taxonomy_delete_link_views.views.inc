<?php
/**
 * @file
 * Provide views handler for taxonomy_delete_link.module.
 */

/**
 * Implements hook_views_data().
 */
function taxonomy_delete_link_views_views_data() {
  $data = array();
  $data['taxonomy_term_data']['delete_term_link'] = array(
    'title' => t('Term delete link'),
    'help' => t('Provide a simple link to delete the term.'),
    'field' => array(
      'handler' => 'TaxonomyDeleteLinkViewsHandlerDeleteTermLink',
    ),
  );
  return $data;
}
