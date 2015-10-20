<?php
// $Id: template.php $
function glassical_preprocess_page(&$vars) {
 // Generate menu tree from source of Primary Links
 $vars['primary_links_tree'] = menu_tree(variable_get('menu_primary_links_source','primary-links'));
 // Generate menu tree from source of Secondary Links
 $vars['secondary_links_tree'] = menu_tree(variable_get('menu_secondary_links_source','secondary-links'));
}