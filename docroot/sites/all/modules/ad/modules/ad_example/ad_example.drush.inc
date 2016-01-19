<?php

/**
 *  Implements hook_drush_help().
 */
function ad_example_drush_help($section) {
  switch ($section) {
    case 'drush:ad-example-install':
      return dt("Install example terms and ads.");
  }
}

/**
 * Implements hook_drush_command().
 */
function ad_example_drush_command() {
  $items['ad-example-install'] = array(
    'description' => "Install example terms and ads.",
  );
  return $items;
}

/**
 * Callback to install examples ads and terms.
 */
function drush_ad_example_install() {
  // Taxonomy vocabularies.
  _ad_example_install_taxonomy();
  // Add example ads.
  _ad_example_install_ads();
  // Enable them.
  _ad_example_install_blocks();
}

/**
 * Install taxonomy data.
 */
function _ad_example_install_taxonomy() {
  $vocabularies = array(
    'advertisement_size' => array(
      'terms' => array(
        array(
          'name' => 'Leaderboard',
          'field_ad_size' => '728x90',
        ),
        array(
          'name' => 'Rectangle',
          'field_ad_size' => '160x120',
        ),
        array(
          'name' => 'Skyscraper',
          'field_ad_size' => '160x600',
        ),
      ),
    ),
  );

  foreach ($vocabularies as $machine_name => $definition) {
    if (!empty($definition['terms'])) {
      $vocabulary = taxonomy_vocabulary_machine_name_load($machine_name);
      _ad_example_install_taxonomy_terms($vocabulary, $definition['terms']);
    }
  }
}

/**
 * Creates the specified taxonomy terms.
 *
 * @param object $vocabulary
 *   The vocabulary object terms should belong to.
 * @param array $terms
 *   An array of term definitions: a term name or a full term array data
 *   structure.
 */
function _ad_example_install_taxonomy_terms($vocabulary, $terms) {
  $weight = 0;
  $tids = array();
  $instances = field_info_instances('taxonomy_term', $vocabulary->machine_name);

  foreach ($terms as $definition) {
    // Init term definition.
    if (is_string($definition)) {
      $definition = array('name' => $definition);
    }
    $definition['vid'] = $vocabulary->vid;
    $definition['weight'] = $weight++;

    // Resolve the parent ID if set.
    if (isset($definition['parent'])) {
      $definition['parent'] = $tids[$definition['parent']];
    }

    // Expand field values.
    foreach ($definition as $key => $value) {
      if (isset($instances[$key]) && !is_array($value)) {
        $definition[$key] = array(LANGUAGE_NONE => array(array('value' => $definition[$key])));
      }
    }

    // Create term.
    $term = (object) $definition;
    taxonomy_term_save($term);
    $tids[] = $term->tid;
  }
}

function _ad_example_install_ads() {
  $ads = array(
    'Rectangle' => array(
      '160x120_1.gif' => 'http://www.apple.com/',
      '160x120_2.jpg' => 'http://www.microsoft.com/',
      '160x120_3.jpg' => 'http://www.google.com/',
    ),
    'Skyscraper' => array(
      '160x600_1.jpg' => 'http://www.facebook.com/',
      '160x600_2.jpg' => 'http://www.yahoo.com/',
      '160x600_3.jpg' => 'http://www.reddit.com/',
    ),
    'Leaderboard' => array(
      '728x90_1.jpg' => 'http://www.twitter.com/',
      '728x90_2.gif' => 'http://www.linkedin.com/',
      '728x90_3.gif' => 'http://www.youtube.com/',
    ),
  );
  $target_dir = "public://ads/";
  file_prepare_directory($target_dir, FILE_CREATE_DIRECTORY);

  foreach ($ads as $size => $ad) {
    $n = 0;
    $term = current(taxonomy_get_term_by_name($size, 'advertisement_size'));
    foreach ($ad as $image => $destination) {
      $n++;
      $image_path = drupal_get_path('module', 'ad_example') . '/images/' . $image;
      $ad = entity_create('node', array(
          'type' => 'ad_example',
          'status' => 1,
          'created' => REQUEST_TIME,
          'changed' => REQUEST_TIME,
          'uid' => 1,
          'title' => $size . ' ' . $n,
          'field_ad_image' => array(LANGUAGE_NONE => array((array) file_save_data(file_get_contents($image_path), $target_dir))),
          'field_ad_link' => array(LANGUAGE_NONE => array(array('url' => $destination))),
          'field_ad_size' => array(LANGUAGE_NONE => array(array('tid' => $term->tid))),
        )
      );
      entity_save('node', $ad);
    }
  }
}

/**
 * Display blocks.
 */
function _ad_example_install_blocks() {
  $theme = variable_get('theme_default', 'bartik');
  db_query("UPDATE {block} SET region = 'sidebar_first', status = 1 WHERE theme = :theme AND delta = 'rectangle_ad_block'", array(':theme' => $theme));
  db_query("UPDATE {block} SET region = 'sidebar_second', status = 1  WHERE theme = :theme AND delta = 'skyscraper_ad_block'", array(':theme' => $theme));
  db_query("UPDATE {block} SET region = 'header', status = 1  WHERE theme = :theme AND delta = 'leaderboard_ad_block'", array(':theme' => $theme));
}
