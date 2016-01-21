<?php
/**
 * @file
 * ns_article.features.inc
 */

/**
 * Implements hook_views_api().
 */
function ns_article_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function ns_article_node_info() {
  $items = array(
    'ns_article' => array(
      'name' => t('Article'),
      'base' => 'node_content',
      'description' => t('Articles are NodeStream\'s main content. They can contain many different types of content, which are described more in depth when you create the article.'),
      'has_title' => '1',
      'title_label' => t('Headline'),
      'help' => '',
    ),
  );
  return $items;
}
