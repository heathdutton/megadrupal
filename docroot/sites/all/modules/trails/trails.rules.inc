<?php
/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function trails_rules_action_info() {
  return array(
    'trails_create_crumb' => array(
      'label' => t('Create a crumb'),
      'parameter' => array(
        'title' => array(
          'type' => 'text',
          'label' => t('Title'),
          'description' => t('The link text for the crumb.'),
          'default mode' => 'input',
          'sanitize' => TRUE,
        ),
        'path' => array(
          'type' => 'uri',
          'label' => t('Path'),
          'description' => t('Internal Drupal path or external URL being linked to. Use %front for home page. If this is omitted, the crumb is not linked.', array('%front' => '<front>')),
          'default mode' => 'input',
          'optional' => TRUE,
        ),
        'html' => array(
          'type' => 'boolean',
          'label' => t('HTML'),
          'description' => t('Whether the title text is used as HTML.'),
          'default mode' => 'input',
          'optional' => TRUE,
        ),
      ),
      'provides' => array(
        'created_crumb' => array(
          'type' => 'trails_crumb',
          'label' => t('Created crumb'),
        ),
      ),
      'group' => t('Trails'),
    ),
    'trails_get_trail' => array(
      'label' => t('Get currently applied trail'),
      'provides' => array(
        'trail' => array(
          'type' => 'list<trails_crumb>',
          'label' => t('Current trail'),
          'description' => t('The list of crumbs configured so far as applied to the site.'),
        ),
      ),
      'base' => 'trails_rules_get_trail',
      'group' => t('Trails'),
    ),
    'trails_set_trail' => array(
      'label' => t('Apply trail'),
      'parameter' => array(
        'trail' => array(
          'type' => 'list<trails_crumb>',
          'label' => t('Trail'),
          'description' => t('The trail of crumbs to set as active.'),
        ),
      ),
      'group' => t('Trails'),
    ),
    'trails_add_crumb' => array(
      'label' => t('Add crumb'),
      'parameter' => array(
        'crumb' => array(
          'type' => 'trails_crumb',
          'label' => t('Crumb'),
          'description' => t('Individual crumb to add to the currently applied trail.'),
        ),
        'prepend' => array(
          'type' => 'boolean',
          'label' => t('Prepend'),
          'description' => t('Whether to insert the crumb at the beginning of the current trail.'),
          'optional' => TRUE,
        ),
      ),
      'group' => t('Trails'),
    ),
    'trails_add_trail' => array(
      'label' => t('Add trail'),
      'parameter' => array(
        'trail' => array(
          'type' => 'list<trails_crumb>',
          'label' => t('Trail'),
          'description' => t('List of crumbs to add to the currently applied trail.'),
        ),
        'prepend' => array(
          'type' => 'boolean',
          'label' => t('Prepend'),
          'description' => t('Whether to insert the trail at the beginning of the current trail.'),
          'optional' => TRUE,
        ),
      ),
      'group' => t('Trails'),
    ),
    'trails_get_menu_trail' => array(
      'label' => t('Get trail from menu'),
      'parameter' => array(
        'menu_name' => array(
          'type' => 'text',
          'label' => t('Menu'),
          'description' => t('Menu to use for building a list of crumbs from the active menu trail.'),
          'options list' => 'trails_list_menus',
        ),
      ),
      'provides' => array(
        'menu_trail' => array(
          'type' => 'list<trails_crumb>',
          'label' => t('Menu trail'),
          'description' => t('List of crumbs built from the active menu trail. If the menu has no active trail, this will be empty.'),
        ),
      ),
      'base' => 'trails_rules_get_menu_trail',
      'group' => t('Trails'),
    ),
    'trails_set_breadcrumb' => array(
      'label' => t('Set trail as breadcrumb'),
      'parameter' => array(
        'trail' => array(
          'type' => 'list<trails_crumb>',
          'label' => t('Trail'),
          'description' => t('Trail of crumbs to set as page breadcrumb.'),
          'restriction' => 'selector',
          'optional' => TRUE,
        ),
      ),
      'group' => t('Trails'),
    ),
  );
}

/**
 * Lists menus to use for getting a trail.
 */
function trails_list_menus() {
  $options = array();
  $result = db_query('SELECT * FROM {menu_custom} ORDER BY title');
  foreach ($result as $row) {
    $options[$row->menu_name] = $row->title;
  }
  return $options;
}

/**
 * Creates a new crumb.
 */
function trails_create_crumb($title, $path = NULL, $html = FALSE) {
  return array(
    'created_crumb' => array(
      'title' => $title,
      'path' => $path,
      'options' => array('html' => $html),
    ),
  );
}

/**
 * Gets menu trail for Rules.
 */
function trails_rules_get_trail() {
  return array('trail' => trails_get_trail());
}

/**
 * Gets menu trail for Rules.
 */
function trails_rules_get_menu_trail($menu_name) {
  $trail = trails_get_menu_trail($menu_name);
  return array('menu_trail' => $trail);
}

/**
 * Implements hook_rules_condition_info().
 */
function trails_rules_condition_info() {
  return array(
    'trails_page_is_front' => array(
      'label' => t('Path is front page'),
      'base' => 'drupal_is_front_page',
      'group' => t('Path'),
    ),
    'trails_page_is_admin' => array(
      'label' => t('Path is administrative'),
      'group' => t('Path'),
    ),
    'trails_theme_is_active' => array(
      'label' => t('Theme is active'),
      'parameter' => array(
        'theme' => array(
          'type' => 'token',
          'label' => t('Theme'),
          'description' => t('The theme to check against the active theme for the path when the condition is evaluated.'),
          'options list' => 'trails_list_themes',
          'optional' => TRUE,
        ),
      ),
      'group' => t('Path'),
    ),
  );
}

/**
 * Checks if the current path is administrative.
 */
function trails_page_is_admin() {
  return path_is_admin(current_path());
}

/**
 * Lists themes for selection in the condition.
 */
function trails_list_themes() {
  $options = array(NULL => t('[@theme]', array('@theme' => t('Default theme'))), '__admin' => t('[@theme]', array('@theme' => t('Administration theme'))));
  $themes = list_themes();
  foreach ($themes as $name => $theme) {
    $options[$name] = $theme->info['name'];
  }
  return $options;
}

/**
 * Checks if the given theme is active.
 */
function trails_theme_is_active($theme = NULL) {
  // Check for admin theme option.
  if ($theme == '__admin') {
    $theme = variable_get('admin_theme', 0);
  }
  // Use default theme.
  if (empty($theme)) {
    $theme = variable_get('theme_default', 'bartik');
  }
  // Check theme.
  return $GLOBALS['theme'] == $theme;
}

/**
 * Implements hook_rules_data_info().
 */
function trails_rules_data_info() {
  return array(
    'trails_crumb' => array(
      'label' => t('Crumb'),
      'wrap' => TRUE,
      'property info' => trails_crumb_data_info(),
      'group' => t('Trails'),
    ),
  );
}

/**
 * Declares crumb properties for Rules data.
 */
function trails_crumb_data_info() {
  return array(
    'title' => array(
      'type' => 'text',
      'label' => t('Title'),
      'description' => t('The link text for the crumb.'),
      'sanitize' => TRUE,
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'path' => array(
      'type' => 'uri',
      'label' => t('Path'),
      'description' => t('Internal Drupal path or external URL being linked to. Use %front for home page.', array('%front' => '<front>')),
      'required' => FALSE,
      'setter callback' => 'entity_property_verbatim_set',
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function trails_rules_event_info() {
  return array(
    'trails_preprocess_page' => array(
      'label' => t('Before rendering page'),
      'group' => t('System'),
    ),
  );
}
