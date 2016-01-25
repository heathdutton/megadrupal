<?php
function alpine_preprocess_page(&$vars) {
 // Generate menu tree from source of Primary Links
 //$vars['main_menu_tree'] = menu_tree(variable_get('menu_main_menu_source','main-menu'));
 // Generate menu tree from source of Secondary Links
 //$vars['secondary_menu_tree'] = menu_tree(variable_get('menu_secondary_menu_source','secondary-menu'));
 // Render menu tree from main-menu
  $menu_tree = menu_tree('main-menu');
  $vars['main_menu_tree'] = render($menu_tree);
}