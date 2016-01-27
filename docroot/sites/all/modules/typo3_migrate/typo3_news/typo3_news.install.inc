<?php
/**
 * @file
 * Set up for the news migration.
 */
function typo3_news_migrate_install() {
  typo3_news_migrate_content_type();
  typo3_news_migrate_category();

  typo3_news_migrate_fields();
}

function typo3_news_migrate_uninstall() {
  if ($vids = taxonomy_vocabulary_load_multiple(array(), array('machine_name' => 'news'))) {
    // Grab key of the first returned vocabulary.
    taxonomy_vocabulary_delete(key($vids));
  }
  typo3_news_migrate_content_type_delete();
}

/**
 *
 * Create content type "news".
 */
function typo3_news_migrate_content_type() {
  $types = array(
    array(
      'type' => 'news',
      'name' => st('News'),
      'base' => 'node_content',
      'description' => st("News from TYPO3 tt_news."),
      'custom' => 1,
      'modified' => 1,
      'locked' => 1,
    ),
  );
  foreach ($types as $type) {
    $type = node_type_set_defaults($type);
    node_type_save($type);
    node_add_body_field($type);
  }
}

/**
 *
 * Create fields for the news content type.
 */
function typo3_news_migrate_fields() {
  // field for news images
  if (!field_info_field('field_news_images')) {
    $field = array(
      'field_name' => 'field_news_images',
      'type' => 'image',
      'cardinality' => 1,
      'translatable' => TRUE,
      'indexes' => array('fid' => array('fid')),
      'settings' => array(
        'uri_scheme' => 'public',
        'default_image' => FALSE,
      ),
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'field_news_images', 'news')) {
    $instance = array(
      'field_name' => 'field_news_images',
      'entity_type' => 'node',
      'label' => 'News Image',
      'bundle' => 'news',
      'description' => 'Upload an image for this news item.',
      'settings' => array(
        'file_directory' => 'field/typo3_news/image',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'alt_field' => TRUE,
        'title_field' => '',
      ),

      'widget' => array(
        'type' => 'image_image',
        'settings' => array(
          'progress_indicator' => 'throbber',
          'preview_image_style' => 'thumbnail',
        ),
        'weight' => -1,
      ),

      'display' => array(
        'full' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'type' => 'image_link_content__medium',
          'settings' => array(),
          'weight' => -1,
        ),
        'rss' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'search_index' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'search_results' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
      ),
    );
    field_create_instance($instance);
  }

  // field for news files
  if (!field_info_field('field_news_files')) {
    $field = array(
      'field_name' => 'field_news_files',
      'type' => 'file',
      'cardinality' => 1,
      'translatable' => TRUE,
      'indexes' => array('fid' => array('fid')),
      'settings' => array(
        'uri_scheme' => 'public',
      ),
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'field_news_files', 'news')) {
    $instance = array(
      'field_name' => 'field_news_files',
      'entity_type' => 'node',
      'label' => 'News File',
      'bundle' => 'news',
      'description' => 'Upload a file for this news item.',
      'settings' => array(
        'file_directory' => 'field/typo3_news/file',
        'file_extensions' => 'txt pdf',
        'max_filesize' => '',
      ),

      'widget' => array(
        'type' => 'file_generic',
        'settings' => array(
          'progress_indicator' => 'throbber'
        ),
        'weight' => -1,
      ),
    );
    field_create_instance($instance);
  }

  // field for news sub-headers
  if (!field_info_field('field_news_sub_header')) {
    $field = array(
      'field_name' => 'field_news_sub_header',
      'type' => 'text',
      'cardinality' => -1,
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'field_news_sub_header', 'news')) {
    $instance = array(
      'field_name' => 'field_news_sub_header',
      'entity_type' => 'node',
      'label' => 'News Sub Header',
      'bundle' => 'news',
      'description' => 'News Sub Header.',

      'widget' => array(
        'type' => 'text_textfield',
      ),
    );
    field_create_instance($instance);
  }

  // field for news author
  if (!field_info_field('field_news_author')) {
    $field = array(
      'field_name' => 'field_news_author',
      'type' => 'text',
      'size' => '60',
      'cardinality' => -1,
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'field_news_author', 'news')) {
    $instance = array(
      'field_name' => 'field_news_author',
      'entity_type' => 'node',
      'label' => 'News Author',
      'bundle' => 'news',
      'description' => 'News Author.',

      'widget' => array(
        'type' => 'text_textfield',
      ),
    );
    field_create_instance($instance);
  }

  // field for news author email
  if (!field_info_field('field_news_author_email')) {
    $field = array(
      'field_name' => 'field_news_author_email',
      'type' => 'text',
      'size' => '60',
      'cardinality' => -1,
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'field_news_author_email', 'news')) {
    $instance = array(
      'field_name' => 'field_news_author_email',
      'entity_type' => 'node',
      'label' => 'Author Email',
      'bundle' => 'news',
      'description' => 'Author Email.',

      'widget' => array(
        'type' => 'text_textfield',
      ),
    );
    field_create_instance($instance);
  }

  if (module_exists('node_reference')) {
    // nodereference field for related articles.
    if (!field_info_field('field_news_related_articles')) {
      $field = array(
        'field_name'  => 'field_news_related_articles',
        'type'        => 'node_reference',
        'cardinality' => -1,
        'settings' => array(
          'referenceable_types' => array('news'),
        ),
      );
      field_create_field($field);
    }

    if (!field_info_instance('user', 'field_news_related_articles', 'news')) {
      $instance = array(
        'field_name' => 'field_news_related_articles',
        'entity_type' => 'user',
        'label' => 'Related Articles',
        'bundle' => 'news',
        'widget' => array(
          'type' => 'node_reference_autocomplete',
        ),
      );
      field_create_instance($instance);
    }
  }


}

function typo3_news_migrate_content_type_delete() {
  $bundle = 'news';
  $field_names = array('field_news_images', 'field_news_files', 'field_news_sub_header', 'field_news_author', 'field_news_author_email', 'field_news_related_articles');
  foreach ($field_names as $field_name) {
    $instance = field_info_instance('node', $field_name, $bundle);
    field_delete_instance($instance);
    field_delete_field($field_name);
  }
  node_type_delete($bundle);
}

/**
 *
 * Create "News Category" vocabulary.
 */
function typo3_news_migrate_category() {

  $description = st('News categories.');
  $help = st('Enter a comma-separated list of words to describe your content.');
  $vocabulary = (object) array(
    'name' => 'News Category',
    'description' => $description,
    'machine_name' => 'news_category',
    'help' => $help,

  );
  taxonomy_vocabulary_save($vocabulary);

  if (!field_info_field('news_category')) {
    $field = array(
      'field_name' => $vocabulary->machine_name,
      'type' => 'taxonomy_term_reference',
      // Set cardinality to unlimited for tagging.
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
      'settings' => array(
        'allowed_values' => array(
          array(
            'vid' => $vocabulary->vid,
            'parent' => 0,
          ),
        ),
      ),
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'news_category', 'news')) {
    $instance = array(
      'field_name' => $vocabulary->machine_name,
      'entity_type' => 'node',
      'label' => $vocabulary->name,
      'bundle' => 'news',
      'description' => $vocabulary->help,
      'widget' => array(
        'type' => 'taxonomy_autocomplete',
      ),
    );
    field_create_instance($instance);
  }

}
