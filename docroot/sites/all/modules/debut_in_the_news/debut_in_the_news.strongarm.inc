<?php
/**
 * @file
 * debut_in_the_news.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function debut_in_the_news_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_in_the_news';
  $strongarm->value = 0;
  $export['comment_anonymous_in_the_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_in_the_news';
  $strongarm->value = 1;
  $export['comment_default_mode_in_the_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_in_the_news';
  $strongarm->value = '50';
  $export['comment_default_per_page_in_the_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_in_the_news';
  $strongarm->value = 1;
  $export['comment_form_location_in_the_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_in_the_news';
  $strongarm->value = '1';
  $export['comment_in_the_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_in_the_news';
  $strongarm->value = '1';
  $export['comment_preview_in_the_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_in_the_news';
  $strongarm->value = 1;
  $export['comment_subject_field_in_the_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_in_the_news';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_in_the_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_in_the_news';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_in_the_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_in_the_news';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_in_the_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_in_the_news';
  $strongarm->value = '1';
  $export['node_preview_in_the_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_in_the_news';
  $strongarm->value = 0;
  $export['node_submitted_in_the_news'] = $strongarm;

  return $export;
}
