<?php
/**
 * Override or insert PHPTemplate variables into the templates.
 */
function gogland_preprocess_page(&$vars) {
  $vars['tabs2'] = menu_secondary_local_tasks();
  // Hook into color.module
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs. Overridden to split the secondary tasks.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
  return menu_primary_local_tasks();
}

function gogland_preprocess_node(&$vars) {
  //print_r($vars['user']);die;
  $vars['title_attributes']['class'] = array(''); 
  $vars['author'] = '<p><em>' . t('By') . '</em><span class="author vcard"> ' . $vars['user']->name . '</span></p>';
  $vars['authered_on'] = date('d F Y', $vars['created']);
}

