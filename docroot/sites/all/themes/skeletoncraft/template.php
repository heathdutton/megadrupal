<?php

function skeletoncraft_preprocess_page(&$vars) {
 // Generate menu tree from source of Primary Links
 //$vars['main_menu_tree'] = menu_tree(variable_get('menu_main_menu_source','main-menu'));
 // Generate menu tree from source of Secondary Links
 //$vars['secondary_menu_tree'] = menu_tree(variable_get('menu_secondary_menu_source','secondary-menu'));
 // Render menu tree from main-menu
  $menu_tree = menu_tree('main-menu');
  $vars['main_menu_tree'] = render($menu_tree);
  drupal_add_library('system', 'ui');
}

/**
 * Implements hook_html_head_alter().
 * This will overwrite the default meta character type tag with HTML5 version.
 */
function skeletoncraft_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
	'name' => 'viewport',
	'content' => 'width=device-width,initial-scale=1.0',
  );
}