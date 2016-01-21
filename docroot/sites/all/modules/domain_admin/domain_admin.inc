<?php

/**
 * @file
 * domain_admin.inc: Generate links for the domain admin block or shortcut bar
 */

function domain_admin_links($domain_id) {
   
  global $user; 
  $links = array();
  $show = variable_get('domain_admin_showlinks', domain_admin_linkdefaults());

    // Main admin pages
    // We declare the old 2.x-compatible paths first.
    // and then we make the adjustments for 3.x
    if (_domain_admin_access('access domain overview page', $domain_id) && !empty($show['domadm_overview'])) {
      $links['domadm_overview'] = array(
        'title' => t('My domains'),
        'href' => 'admin/structure/domain',
        'weight' => -10,
      );
    }
    if (_domain_admin_access('edit own domain record', $domain_id) && !empty($show['domadm_settings'])) {
      $links['domadm_settings'] = array(
        'title' => t('Edit domain record'),
        'href' => 'admin/structure/domain/edit/' . $domain_id,
        'weight' => -9,
      );
    }
    if (_domain_admin_access('configure domain settings', $domain_id) && !empty($show['domadm_conf'])) {
      $links['domadm_conf'] = array(
        'title' => t('Domain settings'),
        'href' => 'admin/structure/domain/conf/' . $domain_id,
        'weight' => -8,
      );
      if (db_table_exists('domain_export')) {  // 3.x
         $links['domadm_conf']['href'] = 'admin/structure/domain/view/'. $domain_id .'/config';
      }
    }
    if (module_exists('domain_theme') && _domain_admin_access('configure domain themes', $domain_id) && !empty($show['domadm_themes'])) {
      $links['domadm_themes'] = array(
        'title' => t('Domain themes'),
        'href' => 'admin/structure/domain/theme/' . $domain_id,
        'weight' => -8,
      );
      if (db_table_exists('domain_export')) { // 3.x
        $links['domadm_themes']['href'] = 'admin/structure/domain/view/'. $domain_id .'/theme';
      }
    }

    // Load entire domain variable array and initialize links
    $domain_vars = domain_conf_variable_get($domain_id, '', TRUE, FALSE);
    $nid = arg(1);

    // Menu links for book pages in domain
    if (module_exists('book')) {

      // We try a couple of approaches to get the current book id:
      // 1. If the nid is available to us, we get it from that
      // 2. If not (eg in the case of node/add pages) we set bid = nid of domain frontpage:
//     we first see if it the frontpage is in 'node/nid' form, if not then we get it from the path alias
      if (arg(0) == 'node' && is_numeric($nid)) {
        $bid = db_query("SELECT bid FROM {book} WHERE nid = :nid", array(':nid' => $nid))->fetchField();
      }
      else if (!empty($domain_vars['site_frontpage'])) {
         $bid = _domain_admin_frontpage_book($domain_vars['site_frontpage'], $domain_vars['language_default']); 
      }      

      if (!empty($bid) && _domain_admin_access('rearrange book pages on domain', $domain_id) && !empty($show['domadm_arrange'])) {
        // Book ID is the same as top level node ID
        $links['domadm_arrange'] = array(
          'title' => t('Rearrange book pages'),
          'href' => 'admin/content/book/' . $bid,
          'weight' => -7,
        );
      }

      if (domain_admin_haschildren($nid) && (_domain_admin_node_access($nid, 'rearrange book pages on domain', $domain_id) || user_access('administer book outlines')) && !empty($show['domadm_child'])) {
        // Rearrange child pages of the current node
        $links['domadm_child'] = array(
          'title' => t('Rearrange child pages'),
          'href' => 'admin/content/book/' . $nid,
          'weight' => -6,
        );
      }
    }

    // If the node being added is a book type and referring page is not a book page,
    // then the page will be a subpage of the domain home page.
    if (arg(0) == 'node' && !(arg(1) == 'add' || arg(2) == 'edit')) {
      $parent_node = node_load($nid);
    }
    else if (!empty($bid)) {   
      $parent_node = node_load($bid);
    }
    if (isset($parent_node->book)) {
      $parent_menu_id = $parent_node->book['mlid'];
    }  

    // Generates edit link for each node type the user has permission for.
    // If the current node is currently part of a book structure the new page
    // will be its child.
    $node_types = node_type_get_names();
    foreach ($node_types as $key => $value) {

      $keylink = str_replace('_', '-', $key);

      // Note: not going to check if user is in domain here
      // The domain module 'publish content...' permissions willl do the domain limiting for us
      if ((_domain_admin_access('create ' . $key . ' content') || _domain_admin_access("create $key content on assigned domains")) && !empty($show['domadm_create_' . $key])) {

        $links['domadm_' . $key] = array(
          'title' => t('Add ' . drupal_strtolower($value)),
          'href' => 'node/add/' . $keylink,
          'weight' => -5,
        );
        if (!empty($parent_menu_id) && book_type_is_allowed($key)) {
          $links['domadm_' . $key]['query'] = array('parent' => $parent_menu_id);
        }
      }
    }

    if (module_exists('menu') && _domain_admin_access('administer domain primary links menu', $domain_id) && !empty($show['domadm_primary_links']) && $domain_vars['menu_main_links_source'] != 'domain-conf-ignore') {
      $links['domadm_primary_links'] = array(
        'title' => t('Edit main menu'),
        'href' => 'admin/structure/menu-customize/' . $domain_vars['menu_main_links_source'],
        'weight' => -4,
      );
    }

    if (module_exists('menu') && _domain_admin_access('administer domain secondary links menu', $domain_id) && !empty($show['domadm_secondary_links']) && $domain_vars['menu_secondary_links_source'] != 'domain-conf-ignore') {
      $links['domadm_secondary_links'] = array(
        'title' => t('Edit sub-menu'),
        'href' => 'admin/structure/menu-customize/' . $domain_vars['menu_secondary_links_source'],
        'weight' => -3,
      );
    }

    if (module_exists('locale') && _domain_admin_access('translate domain language content', $domain_id) && !empty($show['domadm_translate']) && !empty($domain_vars['language_default'])) {
      $links['domadm_translate'] = array(
        'title' => t('Translate site'),
        'href' => 'admin/config/regional/translate/translate',
        'weight' => -1,
      );
    }
    // Link to help pages
    $helppage = variable_get('domain_admin_help_pages', '');
    if (!empty($helppage)) {
      $links['domadm_help'] = array(
        'title' => t('Help pages'),
        'href' => check_plain($helppage),
        'attributes' => array('target' => '_blank'),
        'weight' => 0,
      );
    }  

    // Allow other modules to add links via hook_domainadminlinks.
    $modules = module_implements('domainadminlinks');
    if (!empty($modules)) {
      foreach ($modules as $module) {
        $func = $module . '_domainadminlinks';
        $links = array_merge($links, $func());
      }
    }
    usort($links, 'domain_admin_weights');

    return $links;
}     

/**
 * Check that node has child pages
 */
function domain_admin_haschildren($nid) {
  $has_children = db_query('SELECT ml.has_children FROM {book} b INNER JOIN {menu_links} ml ON b.mlid = ml.mlid WHERE b.nid = :nid', array(':nid' => $nid))->fetchField();
  return $has_children;
}

/**
 * Sort links in admin block by weight
 * Allows external modules to assign weight to links
 * and thus determine their position in the admin block
 */
function domain_admin_weights($link1, $link2) {
  if ($link1['weight'] == $link2['weight']) {
    return 0;
  }
  return ($link1['weight'] < $link2['weight']) ? -1 : 1;
}


function _domain_admin_frontpage_book($frontpage, $language = '') {
     
  $bid = '';   
  if (strpos($frontpage, 'node/') === 0) {
    $pathargs = explode('/', $frontpage);
    $bid = $pathargs[1]; 
  } 
  else if (!empty($language)) {
    $path = drupal_get_normal_path($frontpage, $language = '');
  }  
  if (empty($path)) {
    $path = drupal_get_normal_path($frontpage);  
  }  
  if (strpos($path, 'node/') === 0) {
    $pathargs = explode('/', $path);
    $bid = $pathargs[1]; 
  }  
      
  return $bid;  
}      


