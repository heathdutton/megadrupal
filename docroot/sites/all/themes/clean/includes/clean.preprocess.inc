<?php
/**
 * @file
 * Preprocess functions.
 */

/**
 * Implements hook_preprocess().
 */
function clean_preprocess(&$variables) {
  // Consolidate attributes by moving classes into attributes_array.
  $variables['attributes_array']['class'] = $variables['classes_array'];
}

/**
 * Implements hook_preprocess_html().
 */
function clean_preprocess_html(&$variables) {
  $active_themes = _clean_theme_collector();
  _clean_process_stylesheets($active_themes);

  // Switch the doctype to one that works better for RDF if the module is
  // enabled.
  $variables['doctype'] = (module_exists('rdf')) ? '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">' : '<!DOCTYPE html>';

  $variables['skip_link'] = array(
    '#theme' => 'link',
    '#prefix' => '<div id="skip-link">',
    '#suffix' => '</div>',
    '#href' => '',
    '#text' => t('Skip to main content'),
    '#options' => array(
      'html' => FALSE,
      'fragment' => 'main-content',
      'attributes' => array(
        'class' => array(
          'element-invisible',
          'element-focusable',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_preprocess_page().
 */
function clean_preprocess_page(&$variables) {
  // Use a h1 tag for the site name when the content title is empty.
  $tag = (!empty($variables['title'])) ? 'div' : 'h1';
  // Make the site name into a renderable array.
  $variables['page']['site_name'] = array(
    '#theme' => 'link',
    '#href' => $variables['front_page'],
    '#options' => array(
      'attributes' => array(
        'title' => t('Home'),
        'rel' => 'home',
      ),
      'html' => TRUE,
    ),
    '#text' => '<span>' . $variables['site_name'] . '</span>',
    '#prefix' => '<' . $tag . ' id="site-name">',
    '#suffix' => '</' . $tag . '>',
  );

  // Make the logo into a renderable array.
  $logo_image = array(
    '#theme' => 'image',
    '#path' => $variables['logo'],
    '#alt' => t('Home'),
  );
  $variables['page']['logo'] = array(
    '#theme' => 'link',
    '#href' => $variables['front_page'],
    '#options' => array(
      'attributes' => array(
        'title' => t('Home'),
        'rel' => 'home',
        'id' => 'logo',
      ),
      'html' => TRUE,
    ),
    '#text' => render($logo_image),
  );

  // Make main and secondary menus into renderable arrays.
  $variables['page']['main_menu'] = array(
    '#theme' => 'links__system_main_menu',
    '#links' => $variables['main_menu'],
    '#attributes' => array(
      'id' => 'main-menu',
      'class' => array(
        'links',
        'inline',
        'clearfix',
      ),
    ),
    '#heading' => array(
      'text' => t('Main menu'),
      'level' => 'h2',
    ),
  );
  $variables['page']['secondary_menu'] = array(
    '#theme' => 'links__system_secondary_menu',
    '#links' => $variables['secondary_menu'],
    '#attributes' => array(
      'id' => 'secondary-menu',
      'class' => array(
        'links',
        'inline',
        'clearfix',
      ),
    ),
    '#heading' => array(
      'text' => t('Secondary menu'),
      'level' => 'h2',
    ),
  );
}

/**
 * Implements hook_preprocess_node().
 */
function clean_preprocess_node(&$variables) {
  global $theme_key;

  $node = $variables['node'];

  // Make the title into a link and keep it as a renderable array.
  if (!empty($variables['title'])) {
    $variables['title_link'] = array(
      '#theme' => 'link',
      '#path' => 'node/' . $node->nid,
      '#text' => $variables['title'],
      '#options' => array(
        'html' => FALSE,
        'attributes' => array(),
      ),
    );
  }

  // Add some additional node classes.
  clean_node_classes($variables);

  // Call THEME_preprocess_node_TYPE functions if any exist.
  $function = $theme_key . '_preprocess_node_' . $node->type;
  if (function_exists($function)) {
    call_user_func_array($function, array(&$variables));
  }
}

/**
 * Implements hook_preprocess_comment().
 */
function clean_preprocess_comment(&$variables) {
  $comment = $variables['comment'];

  $variables['attributes_array']['id'] = 'comment-' . $comment->cid;
  $variables['attributes_array']['class'][] = $variables['status'];

  if ($comment->new) {
    $variables['attributes_array']['class'][] = 'comment-new';
  }

  if ($variables['zebra']) {
    $variables['attributes_array']['class'][] = 'comment-' . $variables['zebra'];
  }
}

/**
 * Implements hook_preprocess_block().
 */
function clean_preprocess_block(&$variables) {
  $block = $variables['block'];

  $variables['attributes_array']['id'] = $variables['block_html_id'];
  $variables['title_attributes_array']['class'][] = 'block-title';
  $variables['content_attributes_array']['class'][] = 'block-content';

  $variables['title'] = $block->subject;
}

