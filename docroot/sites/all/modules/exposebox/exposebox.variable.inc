<?php
/**
 * @file
 * exposeit.variable.inc
 */

function exposeit_default_variable_assets() {
  $export = array();

  $variable = new stdClass();
  $variable->name = 'comment_anonymous_news_article';
  $variable->value = 0;
  $export['comment_anonymous_news_article'] = $variable;

  return $export;
}
