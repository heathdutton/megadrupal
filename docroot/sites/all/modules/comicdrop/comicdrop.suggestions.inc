<?php
// $Id$

/**
 * @file
 * Helper functions, to quickly create an initial installation for new users.
 */

function _comicdrop_install_user_configs() {
  // Enable user picture support and set the default to a square thumbnail option.
  variable_set('user_pictures', '1');
  drupal_set_message(t('User profile pictures have been activated.'));
  variable_set('user_picture_dimensions', '1024x1024');
  variable_set('user_picture_file_size', '800');
  variable_set('user_picture_style', 'thumbnail');
  drupal_set_message(t('User profile pictures dimensions have been set.'));
  // Allow visitor account creation without administrative approval.
  variable_set('user_register', USER_REGISTER_VISITORS);
  drupal_set_message(t('Users may now register without admin approval (still must confirm with an email address).'));
  // User personalization, enable signatures.
  variable_set('user_signatures', 1);
  drupal_set_message(t('User profile signatures have been activated.'));
}

function _comicdrop_install_site_configs() {
  // Configure statistics access.
  variable_set('statistics_enable_access_log', 1);
  variable_set('statistics_count_content_views', 1);
  drupal_set_message(t('Statistical data tracking has been activated.'));
  // Under performance, cache pages for anon, cache bloccks, set minimums, set dandwidth ops.
  variable_set('cache', 1);
  variable_set('cache_lifetime', 2700);
  variable_set('page_cache_maximum_age', 2700);
  variable_set('page_compression', TRUE);
  drupal_set_message(t('Caching and page compression has been activated.'));
  // Under site configuration - set #of posts for front page to 1
  // (lack of front page controls means this holds for all summary pages (including blog) )
  // variable_set('default_nodes_main', 1);
  // Under logging and errors set to none
  variable_set('error_level', ERROR_REPORTING_HIDE);
  drupal_set_message(t('Errors are now hidden from end user display.'));
}

function _comicdrop_install_node_thread_configs() {
  // No threaded types, really.
  $types = array_keys(node_type_get_types());
  foreach ($types as $type) {
    variable_set('comment_default_mode_' . $type, 0);
  }
  drupal_set_message(t('Threaded messaging has been disabled for all existant node types.'));
}

function _comicdrop_install_comic_forum_configs() {
  if (!module_exists('forum')) {
    module_enable(array('forum'));
    drupal_set_message(t('Forums have been activated for your site.'));
    cache_clear_all();
    menu_rebuild();
  }
  $forum_vocab = taxonomy_vocabulary_machine_name_load('forums');
  $term = array(
    'name' => t('Comic discussion'),
    'description' => '',
    'parent' => array(0),
    'vid' => $forum_vocab->vid,
    );
  $term = (object) $term;
  taxonomy_term_save($term);
  variable_set('comicdrop_forum_tid', $term->tid);
  drupal_set_message(t('A new forum has been created on your site: Comic discussion'));
  if (!field_info_field('taxonomy_forums')) {
    // Create fields and add them to blogs and comics.
    $field = array(
      'field_name' => 'taxonomy_forums',
      'type' => 'taxonomy_term_reference',
      'cardinality' => 1,
      'settings' => array(
        'allowed_values' => array(
          array(
           'vocabulary' => 'forums',
           'parent' => 0,
          ),
        ),
      ),
    );
    field_create_field($field);
    drupal_set_message(t('A new field has been created for forum taxonomy.'));
  }
  if (!field_read_instance('node', 'taxonomy_forums', 'comic')) {
    $instance = array(
      'field_name' => 'taxonomy_forums',
      'entity_type' => 'node',
      'label' => 'Forum',
      'bundle' => 'comic',
      'description' => '',
      'widget' => array(
        'type' => 'options_select',
        'weight' => -4,
      ),
      'display' => array(
        'default' => array(
          'type' => 'taxonomy_term_reference_link',
          'weight' => 10,
        ),
        'teaser' => array(
          'type' => 'taxonomy_term_reference_link',
          'weight' => 10,
        ),
      ),
    );
    field_create_instance($instance);
    drupal_set_message(t('Forum taxonomy field has been added to comic.'));
  }
}

function _comicdrop_install_menu_blogroll_configs() {
  if ( menu_load('menu-blogroll') === FALSE) {
    $menu['menu_name'] = 'menu-blogroll';
    $menu['title'] = 'Blogroll';
    $menu['description'] = 'An example menu, blogroll.';
    menu_save($menu);
    $menu_items = array(
      'link_title' => st('Drupal.org'),
      'link_path' => 'http://drupal.org',
      'menu_name' => 'menu-blogroll',
      'weight' => 0,
    );
    menu_link_save($menu_items);
    menu_rebuild();
    // Add this to blocks.
    $values = array(
        array(
        'module' => 'menu',
        'delta' => 'menu-blogroll',
        'theme' => variable_get('theme_default', 'bartik'),
        'status' => 1,
        'weight' => -7,
        'region' => 'sidebar_first',
        'pages' => '',
        'cache' => 8,
      ),
    );
    $query = db_insert('block')->fields(array('module', 'delta', 'theme', 'status', 'weight', 'region', 'pages', 'cache'));
    foreach ($values as $record) {
      $query->values($record);
    }
    $query->execute();
    drupal_set_message(t('A new menu, menu-blogroll, has been created and initialized with an example blogroll link.'));
  }
}

function _comicdrop_install_node_type_taxonomy_configs() {
  $types = variable_get('comicdrop_types', array('comic'));
  foreach ($types as $type) {
    $cardinality = FIELD_CARDINALITY_UNLIMITED;
    $widget = 'taxonomy_autocomplete';
    $tag = 'tags';
    if (!field_info_field('field_' . $tag)) {
      // Create fields and add them to blogs and comics.
      $field = array(
        'field_name' => 'field_' . $tag,
        'type' => 'taxonomy_term_reference',
        'cardinality' => $cardinality,
        'settings' => array(
          'allowed_values' => array(
            array(
              'vocabulary' => $tag,
              'parent' => 0,
            ),
          ),
        ),
      );
      field_create_field($field);
      drupal_set_message(t('A taxonomy field for tags has been created.'));
    }
    if (!field_read_instance('node', 'field_' . $tag, $type)) {
      $instance = array(
        'field_name' => 'field_' . $tag,
        'entity_type' => 'node',
        'label' => ucfirst($tag),
        'bundle' => $type,
        'description' => $tag,
        'widget' => array(
          'type' => $widget,
          'weight' => -4,
        ),
        'display' => array(
          'default' => array(
            'type' => 'taxonomy_term_reference_link',
            'weight' => 10,
          ),
          'teaser' => array(
            'type' => 'taxonomy_term_reference_link',
            'weight' => 10,
          ),
        ),
      );
      field_create_instance($instance);
      drupal_set_message(t('The taxonomy field for tags has been added to node type ' . $type . '.'));
    }
  }
}

function _comicdrop_install_menu_configs() {
  $clear = FALSE;
  if (!module_exists('forum')) {
    module_enable(array('forum'));
    drupal_set_message(t('Forums have been activated for your site.'));
    $clear = TRUE;
  }
  if (!module_exists('blog')) {
    module_enable(array('blog'));
    drupal_set_message(t('Blogs have been activated for your site.'));
    $clear = TRUE;
  }
  if (!module_exists('contact')) {
    module_enable(array('contact'));
    drupal_set_message(t('Contact pages have been activated for your site.'));
    $clear = TRUE;
  }
  if ($clear) {
    cache_clear_all();
    menu_rebuild();
  }
  $menu_items = array(
    'link_title' => st('Archives'),
    'link_path' => 'archive',
    'menu_name' => 'main-menu',
    'weight' => 1,
  );
  menu_link_save($menu_items);
  $menu_items = array(
    'link_title' => st('Blog'),
    'link_path' => 'blog',
    'menu_name' => 'main-menu',
    'weight' => 3,
  );
  menu_link_save($menu_items);
  $menu_items = array(
    'link_title' => st('Forums'),
    'link_path' => 'forum',
    'menu_name' => 'main-menu',
    'weight' => 6,
  );
  menu_link_save($menu_items);
  $menu_items = array(
    'link_title' => st('Contact Us'),
    'link_path' => 'contact',
    'menu_name' => 'main-menu',
    'weight' => 9,
  );
  menu_link_save($menu_items);
  // Update the menu router information.
  menu_rebuild();
  drupal_set_message(t('New menu options have been added to the main menu for Blog, Forums, and Contact form.'));
}

