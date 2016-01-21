<?php

/**
 * Callback for the "add parent subsites" breadcrumb action to generate the crumbs.
 *
 * @param array $arguments
 * @return array
 */
function dominion_hansel_action_add_parent_subsites_get_crumbs($arguments) {
  global $_domain, $_hansel_flags;
  if (in_array('alias', $_hansel_flags) || in_array('menu', $_hansel_flags)) {
    // Do not add crumbs when generating the alias token or menu path.
    return array();
  }
  $domain_id = $_domain['domain_id'];
  $crumbs = array();
  while ($subsite = dominion_get_info($domain_id)) {
    if ($domain_id != $_domain['domain_id'] || (!empty($arguments['include_self']) && $arguments['include_root'])) {
      array_unshift($crumbs, array(
        'title' => $subsite->sitename,
        'href' => $subsite->scheme . '://' . $subsite->subdomain . '/' . $subsite->directory,
      ));
    }
    if ($subsite->parent) {
      if ($subsite->parent != $subsite->root || !empty($arguments['include_root'])) {
        $domain_id = $subsite->parent;
        continue;
      }
    }
    break;
  }
  return $crumbs;
}

/**
 * Callback for the "add parent subsites" breadcrumb action to generate the config form.
 *
 * @param array $arguments
 * @return array
 */
function dominion_hansel_action_add_parent_subsites_config_form($arguments) {
  $form = array();
  
  $form['include_self'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include current domain'),
    '#default_value' => !empty($arguments['include_self']),
  );
  
  $form['include_root'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include root domain'),
    '#default_value' => !empty($arguments['include_root']),
  );
  
  return $form;
}

/**
 * Callback for the "blueprint" switch.
 * 
 * @param array $arguments
 * @param string $value
 * @return bool
 */
function dominion_hansel_switch_url_blueprint_compare($arguments, $value) {
  if ($subsite = dominion_get_info()) {
    if ($value == $subsite->blueprint) {
      return TRUE;
    }
  }
  return FALSE;
}
