<?php
/**
 * @file
 * File to deal with the page callback for comments.
 *
 * The comments page callback is for the comments per node type
 * and per node type -> taxonomy term.
 */

/**
 * Page callback function for admin/content/%node_type/comment.
 */
function content_admin_tree_node_type_comments($node_type) {
  $content = array();
  // Get the views block for comments.
  $views_block = views_embed_view('comments', 'block', $node_type->type);
  $content['views_block'] = array(
    '#type' => 'markup',
    '#markup' => $views_block,
    '#prefix' => '<div class="cat-views-block-comments">',
    '#suffix' => '</div>',
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
      ),
      'js' => array(
        drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
      ),
    ),
  );
  // Set the current breadcrumb correctly.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(t('Administration'), 'admin');
  $breadcrumb[] = l(t('Content'), 'admin/content');
  $breadcrumb[] = l($node_type->name, 'admin/content/' . $node_type->type);
  drupal_set_breadcrumb($breadcrumb);
  // Set the title for the current page.
  drupal_set_title(t('Comments'));
  return $content;
}
/**
 * Page callback function for admin/content/%node_type/comment/approval.
 */
function content_admin_tree_node_type_comments_approval($node_type) {
  $content = array();
  // Get the views block for comments.
  $views_block = views_embed_view('comments_unapproved', 'block', $node_type->type);
  $content['views_block'] = array(
    '#type' => 'markup',
    '#markup' => $views_block,
    '#prefix' => '<div class="cat-views-block-comments">',
    '#suffix' => '</div>',
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
      ),
      'js' => array(
        drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
      ),
    ),
  );
  // Set the current breadcrumb correctly.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(t('Administration'), 'admin');
  $breadcrumb[] = l(t('Content'), 'admin/content');
  $breadcrumb[] = l($node_type->name, 'admin/content/' . $node_type->type);
  $breadcrumb[] = l(t('Comments'), 'admin/content/' . $node_type->type . '/comment');
  drupal_set_breadcrumb($breadcrumb);
  // Set the title for the current page.
  drupal_set_title(t('Unapproved comments'));
  return $content;
}
/**
 * Page callback function.
 *
 * admin/content/%node_type/overview/%taxonomy_term/comment.
 */
function content_admin_tree_node_type_taxonomy_term_comments($node_type, $taxonomy_term) {
  $content = array();
  // Get the views block for comments.
  $views_block = views_embed_view('comments', 'block', $node_type->type, $taxonomy_term->tid);
  // Add the block to the content array.
  $content['views_block'] = array(
    '#type' => 'markup',
    '#markup' => $views_block,
    '#prefix' => '<div class="cat-views-block-comments">',
    '#suffix' => '</div>',
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
      ),
      'js' => array(
        drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
      ),
    ),
  );
  // Set the current breadcrumb correctly.
  $breadcrumb = array();
  $tax_term_alias = drupal_strtolower(str_replace(' ', '-', $taxonomy_term->name));
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(t('Administration'), 'admin');
  $breadcrumb[] = l(t('Content'), 'admin/content');
  $breadcrumb[] = l($node_type->name, 'admin/content/' . $node_type->type);
  $breadcrumb[] = l($taxonomy_term->name, 'admin/content/' . $node_type->type .
                    '/overview/' . $tax_term_alias);
  drupal_set_breadcrumb($breadcrumb);
  // Set the title for the current page.
  drupal_set_title(t('Comments'));
  return $content;
}
/**
 * Page callback for unapproved comments page.
 */
function content_admin_tree_node_type_taxonomy_term_comments_approval($node_type, $taxonomy_term) {
  $content = array();
  // Get the views block for comments.
  $views_block = views_embed_view('comments_unapproved', 'block', $node_type->type, $taxonomy_term->tid);
  // Add the block to the content array.
  $content['views_block'] = array(
    '#type' => 'markup',
    '#markup' => $views_block,
    '#prefix' => '<div class="cat-views-block-comments">',
    '#suffix' => '</div>',
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
      ),
      'js' => array(
        drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
      ),
    ),
  );
  // Set the current breadcrumb correctly.
  $breadcrumb = array();
  $tax_term_alias = drupal_strtolower(str_replace(' ', '-', $taxonomy_term->name));
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(t('Administration'), 'admin');
  $breadcrumb[] = l(t('Content'), 'admin/content');
  $breadcrumb[] = l($node_type->name, 'admin/content/' . $node_type->type);
  $breadcrumb[] = l($taxonomy_term->name, 'admin/content/' . $node_type->type .
                    '/overview/' . $tax_term_alias);
  $breadcrumb[] = l(t('Comments'), 'admin/content/' . $node_type->type . '/comment');
  drupal_set_breadcrumb($breadcrumb);
  // Set the title for the current page.
  drupal_set_title(t('Unapproved comments'));
  return $content;
}
