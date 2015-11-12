<?php
/**
 * @file
 * cms_blog_content.features.uuid_term.inc
 */

/**
 * Implements hook_uuid_features_default_terms().
 */
function cms_blog_content_uuid_features_default_terms() {
  $terms = array();

  $terms[] = array(
    'name' => 'Business',
    'description' => '',
    'format' => 'wysiwyg_full',
    'weight' => 0,
    'uuid' => '3456efe3-c665-49de-abc8-0fd9f5075ee5',
    'vocabulary_machine_name' => 'cms_blog_category',
    'url_alias' => array(
      0 => array(
        'alias' => 'blog-category/business',
        'language' => 'und',
      ),
    ),
  );
  $terms[] = array(
    'name' => 'Secrets',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '3f81c575-06ff-43cb-8339-d4790fa56ede',
    'vocabulary_machine_name' => 'tags',
    'url_alias' => array(
      0 => array(
        'alias' => 'tags/secrets',
        'language' => 'und',
      ),
    ),
  );
  $terms[] = array(
    'name' => 'Lifestyle',
    'description' => '',
    'format' => 'wysiwyg_full',
    'weight' => 0,
    'uuid' => '60e20db5-3340-40b2-87e0-ac2b560b1355',
    'vocabulary_machine_name' => 'cms_blog_category',
    'url_alias' => array(
      0 => array(
        'alias' => 'blog-category/lifestyle',
        'language' => 'und',
      ),
    ),
  );
  $terms[] = array(
    'name' => 'Drupal',
    'description' => '',
    'format' => 'wysiwyg_full',
    'weight' => 0,
    'uuid' => '7e721cd9-5007-4853-b262-be15cc60534c',
    'vocabulary_machine_name' => 'cms_blog_category',
    'url_alias' => array(
      0 => array(
        'alias' => 'blog-category/drupal',
        'language' => 'und',
      ),
    ),
  );
  $terms[] = array(
    'name' => 'Inspiration',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '9ee3da11-283d-45d3-85ed-a7e96547ad49',
    'vocabulary_machine_name' => 'tags',
    'url_alias' => array(
      0 => array(
        'alias' => 'tags/inspiration',
        'language' => 'und',
      ),
    ),
  );
  $terms[] = array(
    'name' => 'Design',
    'description' => '',
    'format' => 'wysiwyg_full',
    'weight' => 0,
    'uuid' => 'ab16e580-71a8-4afc-a7f8-775e78c0b54f',
    'vocabulary_machine_name' => 'cms_blog_category',
    'url_alias' => array(
      0 => array(
        'alias' => 'blog-category/design',
        'language' => 'und',
      ),
    ),
  );
  $terms[] = array(
    'name' => 'Oppurtunities',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'c8507d08-9e00-4f1a-938a-5b97d82f8813',
    'vocabulary_machine_name' => 'tags',
    'url_alias' => array(
      0 => array(
        'alias' => 'tags/oppurtunities',
        'language' => 'und',
      ),
    ),
  );
  $terms[] = array(
    'name' => 'Business',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'e32de019-e4d5-46b6-a10d-786c6cc70756',
    'vocabulary_machine_name' => 'tags',
    'url_alias' => array(
      0 => array(
        'alias' => 'tags/business',
        'language' => 'und',
      ),
    ),
  );
  $terms[] = array(
    'name' => 'Travel',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'f870ff3e-acef-4e3e-8958-1e10ef0565ed',
    'vocabulary_machine_name' => 'tags',
    'url_alias' => array(
      0 => array(
        'alias' => 'tags/travel',
        'language' => 'und',
      ),
    ),
  );
  return $terms;
}
