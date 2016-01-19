<?php
/**
 * @file
 * easy_blog_social.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function easy_blog_social_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'easy_social_article_enable';
  $strongarm->value = 0;
  $export['easy_social_article_enable'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'easy_social_blog_post_enable';
  $strongarm->value = 1;
  $export['easy_social_blog_post_enable'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'easy_social_global_order';
  $strongarm->value = array(
    0 => 'twitter',
    1 => 'facebook',
    2 => 'googleplus',
    3 => 'linkedin',
  );
  $export['easy_social_global_order'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'easy_social_page_enable';
  $strongarm->value = 0;
  $export['easy_social_page_enable'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'variable_module_list';
  $strongarm->value = array(
    'easy_social' => array(
      0 => 'easy_social_ignore_paths',
      1 => 'easy_social_global_type',
      2 => 'easy_social_global_widgets',
      3 => 'easy_social_global_order',
      4 => 'easy_social_blog_post_enable',
      5 => 'easy_social_comment_blog_post_enable',
      6 => 'easy_social_blog_post_count',
      7 => 'easy_social_blog_post_type',
      8 => 'easy_social_blog_post_widgets',
      9 => 'easy_social_article_enable',
      10 => 'easy_social_comment_article_enable',
      11 => 'easy_social_article_count',
      12 => 'easy_social_article_type',
      13 => 'easy_social_article_widgets',
      14 => 'easy_social_page_enable',
      15 => 'easy_social_comment_page_enable',
      16 => 'easy_social_page_count',
      17 => 'easy_social_page_type',
      18 => 'easy_social_page_widgets',
      19 => 'easy_social_block_count',
      20 => 'easy_social_block_1_title',
      21 => 'easy_social_block_1_override',
      22 => 'easy_social_block_1_type',
      23 => 'easy_social_block_1_widgets',
      24 => 'easy_social_block_2_title',
      25 => 'easy_social_block_2_override',
      26 => 'easy_social_block_2_type',
      27 => 'easy_social_block_2_widgets',
      28 => 'easy_social_block_3_title',
      29 => 'easy_social_block_3_override',
      30 => 'easy_social_block_3_type',
      31 => 'easy_social_block_3_widgets',
      32 => 'easy_social_block_4_title',
      33 => 'easy_social_block_4_override',
      34 => 'easy_social_block_4_type',
      35 => 'easy_social_block_4_widgets',
      36 => 'easy_social_block_5_title',
      37 => 'easy_social_block_5_override',
      38 => 'easy_social_block_5_type',
      39 => 'easy_social_block_5_widgets',
      40 => 'easy_social_block_6_title',
      41 => 'easy_social_block_6_override',
      42 => 'easy_social_block_6_type',
      43 => 'easy_social_block_6_widgets',
      44 => 'easy_social_block_7_title',
      45 => 'easy_social_block_7_override',
      46 => 'easy_social_block_7_type',
      47 => 'easy_social_block_7_widgets',
      48 => 'easy_social_block_8_title',
      49 => 'easy_social_block_8_override',
      50 => 'easy_social_block_8_type',
      51 => 'easy_social_block_8_widgets',
      52 => 'easy_social_block_9_title',
      53 => 'easy_social_block_9_override',
      54 => 'easy_social_block_9_type',
      55 => 'easy_social_block_9_widgets',
      56 => 'easy_social_block_10_title',
      57 => 'easy_social_block_10_override',
      58 => 'easy_social_block_10_type',
      59 => 'easy_social_block_10_widgets',
      60 => 'easy_social_twitter_account_via',
      61 => 'easy_social_twitter_account_related',
      62 => 'easy_social_twitter_account_description',
      63 => 'easy_social_twitter_count_show',
      64 => 'easy_social_facebook_appid',
      65 => 'easy_social_facebook_widget_type',
      66 => 'easy_social_facebook_send_button',
      67 => 'easy_social_facebook_width_vertical',
      68 => 'easy_social_facebook_width_horizontal',
      69 => 'easy_social_facebook_show_faces',
      70 => 'easy_social_facebook_verb',
      71 => 'easy_social_facebook_color_scheme',
      72 => 'easy_social_facebook_font',
      73 => 'easy_social_googleplus_annotation',
      74 => 'easy_social_googleplus_html5',
      75 => 'easy_social_linkedin_count_show',
    ),
  );
  $export['variable_module_list'] = $strongarm;

  return $export;
}
