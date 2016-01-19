<?php

/**
 * _ntm_m_get_names().
 * _ntm_t_get_names().
 */
function _ntm_m_get_names() {
  $names = menu_get_menus();
  return $names;
}

function _ntm_t_get_names() {
  $options = module_exists('taxonomy') ? taxonomy_vocabulary_get_names() : NULL;
  $names = array();
  if ($options) {
    foreach ($options as $item) {
      $names[$item->vid] = $item->name;
    }
  }
  return $names;
}


/**
 * _ntm_m_get_flat_tree().
 * _ntm_t_get_flat_tree().
 */
function _ntm_m_get_flat_tree($source_id, $iid = 0, $max_depth = NTM_MAX_DEPTH) {
  if ($iid) {
    $root_info = menu_link_load($iid);
  }
  $cache = &drupal_static(__FUNCTION__, array());
  $cache_id = $source_id.$iid.$max_depth;
  if (!isset($cache[$cache_id])) {
    $cache[$cache_id] = menu_build_tree($source_id, array('max_depth' => (isset($root_info) ? $root_info['depth'] + $max_depth : $max_depth)));
  }
  $tree = $cache[$cache_id];
  if ($iid) {
    $tree = _ntm_m_get_sub_tree($tree, $iid);
    $tree = $tree['below'];
  }
  if ($tree) {
    $tree = _ntm_m_items_collect(menu_tree_output($tree));
 /* depth correct */
    if (isset($root_info)) {
      foreach ($tree as $item) {
        $item->depth-= $root_info['depth'];
      }
    }
 /* active trail */
    $menu_active_trail = menu_get_active_trail();
    $active_item = end($menu_active_trail);
    if (isset($active_item['mlid'])) {
      if ($active_parents = _ntm_m_get_pids($active_item['mlid'])) {
        foreach ($tree as $item) {
          $item->active_trail = (int)isset($active_parents[$item->iid]);
        }
      }
    }
  }
  return $tree;
}

function _ntm_t_get_flat_tree($source_id, $iid = 0, $max_depth = NTM_MAX_DEPTH) {
  if (module_exists('taxonomy')) {
    if (module_exists('i18n_taxonomy')) {
      $i18n_mode = i18n_taxonomy_vocabulary_mode($source_id);
      switch ($i18n_mode) {
        case I18N_MODE_NONE:
        case I18N_MODE_LOCALIZE: $tree = taxonomy_get_tree($source_id, $iid, $max_depth); break;
        case I18N_MODE_TRANSLATE:
        case I18N_MODE_LANGUAGE: $tree = i18n_taxonomy_get_tree($source_id, $GLOBALS['language']->language, $iid, $max_depth); break;
      }
    } else {
      $tree = taxonomy_get_tree($source_id, $iid, $max_depth);
    }
    foreach ($tree as $term) {
      $term->depth++;
      $term->iid = $term->tid;
      $term->url = "taxonomy/term/$term->iid";
      $term->title = isset($i18n_mode) && $i18n_mode == I18N_MODE_LOCALIZE ? i18n_taxonomy_term_name($term, $GLOBALS['language']->language) : $term->name;
    }
 /* active trail */
    if (arg(0) == 'taxonomy' && arg(1) == 'term' && is_numeric(arg(2))) {
      $active_id = arg(2);
      if ($active_parents = _ntm_t_get_pids($active_id)) {
        foreach ($tree as $item) {
          $item->active_trail = (int)isset($active_parents[$item->iid]);
        }
      }
    }
  }
  return $tree;
}


/**
 * _ntm_m_get_pids().
 * _ntm_t_get_pids().
 */
function _ntm_m_get_pids($c_pid) {
  $pids = array();
  while (TRUE) {
    $c_pid = db_select('menu_links', 'm')->fields('m', array('plid'))->condition('m.mlid', $c_pid)->execute()->fetchField();
    if ($c_pid) $pids[$c_pid] = $c_pid; else break;
  }
  return $pids;
}

function _ntm_t_get_pids($c_pid) {
  $pids = array();
  while (TRUE) {
    $c_pid = db_select('taxonomy_term_hierarchy', 't')->fields('t', array('parent'))->condition('t.tid', $c_pid)->execute()->fetchField();
    if ($c_pid) $pids[$c_pid] = $c_pid; else break;
  }
  return $pids;
}


/**
 * ntm_tree_render().
 * theme_ntm_tree().
 */
function ntm_tree_render($tree, $decor, $style, $wrappers) {
  return '<div class="menu-ntm menu-ntm-style-'.$style.' menu-ntm-decor-'.$decor.' menu-ntm-decor-'.$decor.'-style-'.$style.' clearfix">'.
            theme('ntm_tree', array('tree' => $tree, 'wrappers' => $wrappers)).
         '</div>';
}

function theme_ntm_tree($variables) {
  $tree = $variables['tree'];
  $wrappers = $variables['wrappers'];
  $output = '<ul>';
  foreach ($tree as $item) {
 /* wrappers */
    $wrapper_level_name = $item->depth == 1 ? 'level_1' : 'level_2';
    $wr_opn = isset($wrappers[$wrapper_level_name]) ? '<'. implode('><',  $wrappers[$wrapper_level_name]).'>' : '';
    $wr_cls = isset($wrappers[$wrapper_level_name]) ? '</'.implode('></', array_reverse($wrappers[$wrapper_level_name])).'>' : '';
 /* classes */
    $class = array();
    if (isset($item->active_trail) && $item->active_trail == 1) $class[]= 'active_trail';
    if (count($item->child_set)) $class[]= 'has_child';
    if ($item == reset($tree)) $class[]= 'first';
    if ($item == end($tree)) $class[]= 'last';
    if (in_array('active_trail', $class) && $item == reset($tree)) $class[]= 'active_trail-first';
    if (in_array('active_trail', $class) && $item == end($tree)) $class[]= 'active_trail-last';
 /* current level markup */
    $output.= '<li'.(count($class) ? ' class="'.implode(' ', $class).'"' : '').'>'.
                l("$wr_opn$item->title$wr_cls", $item->url, array('html' => TRUE)).
                (count($item->child_set) ? theme_ntm_tree(array('tree' => $item->child_set, 'wrappers' => $wrappers)) : '').
              "</li>\n";
  }
  $output.= '</ul>';
  return $output;
}


/**
 * support funcs
 */
function _ntm_flat_to_tree($items) {
  $tree = array();
  foreach ($items as $item) {
    $item = clone $item;
    $item->child_set = array();
    if ($item->depth == 1) {$tree[]= $item; $p1 = &$item->child_set;}
    if ($item->depth >= 2) {${'p'.($item->depth-1)}[]= $item; ${'p'.$item->depth} = &$item->child_set;}
  }
  return $tree;
}

function _ntm_m_get_sub_tree($tree, $iid) {
  foreach ($tree as $item) {
    if ($item['link'] && $item['link']['mlid'] == $iid) return $item;
    if ($item['below']) {
      $item = _ntm_m_get_sub_tree($item['below'], $iid);
      if ($item) return $item;
    }
  }
}

function _ntm_m_items_collect($tree, &$info = array()) {
  foreach ($tree as $item) {
    if (isset($item['#original_link'])) {
      $link = $item['#original_link'];
      $info[]= (object)array(
        'iid' => $link['mlid'],
        'title' => $link['title'],
        'url' => $link['href'],
        'depth' => $link['depth'],
      );
    }
    if (isset($item['#below'])) {
      _ntm_m_items_collect($item['#below'], $info);
    }
  }
  return $info;
}