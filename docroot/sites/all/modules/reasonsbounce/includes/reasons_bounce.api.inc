<?php

/**
 * Add new bounce form.
 * 
 * @param int $nid
 */
function reasons_bounce_add_bounce_form($nid) {
  $contain = variable_get(BOUNCE_FORM_CONTAIN_NIDS, array());
  if (!in_array($nid, $contain)) {
    $contain[] = $nid;
    variable_set(BOUNCE_FORM_CONTAIN_NIDS, $contain);
  }  
}

/**
 * Delete bounce form.
 * 
 * @param int $nid
 */
function reasons_bounce_delete_bounce_form($nid) {
  $contain = variable_get(BOUNCE_FORM_CONTAIN_NIDS, array());
  $index = array_search($nid, $contain);
  if ($index !== FALSE) {
    unset($contain[$index]);
    variable_set(BOUNCE_FORM_CONTAIN_NIDS, $contain);
    variable_del('reasons_bounce_template_' . $nid . '_visibility');
    variable_del('reasons_bounce_template_' . $nid . '_access_pages');
    variable_del('reasons_bounce_template_' . $nid . '_node_types');
    variable_del('reasons_bounce_template_' . $nid . '_roles');
    variable_del('reasons_bounce_template_' . $nid . '_message');
  }
}

/**
 * Check whether there is bounce form
 * 
 * @param type $nid
 * 
 * @return mixed
 *   int - if form exists, FALSE - not
 */
function reasons_bounce_check_bounce_form($nid) {
  $contain = variable_get(BOUNCE_FORM_CONTAIN_NIDS, array());
  $is_exist = in_array($nid, $contain);
  return $is_exist;
}

/**
 * Get templates of form.
 * 
 * @return array
 */
function reasons_bounce_get_templates() {
  $templates = array();
  $nids = variable_get(BOUNCE_FORM_CONTAIN_NIDS, array());
  foreach ($nids as $nid) {
    $templates[$nid]['template'] = $nid;
    $templates[$nid]['visibility'] = variable_get('reasons_bounce_template_' . $nid . '_visibility');
    $templates[$nid]['access_pages'] = variable_get('reasons_bounce_template_' . $nid . '_access_pages');
    $templates[$nid]['node_types'] = variable_get('reasons_bounce_template_' . $nid . '_node_types');
    $templates[$nid]['roles'] = variable_get('reasons_bounce_template_' . $nid . '_roles');
    $templates[$nid]['message'] = variable_get('reasons_bounce_template_' . $nid . '_message');
  }
  return $templates;
}

/**
 * Check environment.
 *
 * @param $pages 
 * @return mixed
 *  FALSE - page havem't template
 *  array - template form
 */
function reasons_bounce_check_environment($node = NULL) {
  $is_show = FALSE;
  // Visibility on pages
  $templates = reasons_bounce_get_templates();
  foreach ($templates as $item) {
    $is_visibility = reasons_bounce_check_visibility($item);
    $is_roles = reasons_bounce_check_roles($item);
    $is_node_type = TRUE;
    if ($node) {
      $is_node_type = reasons_bounce_check_node_type($item, $node->type);
    }  
    if ($is_visibility && $is_roles && $is_node_type) {
      return $item;
    }
  }
  return $is_show;
}

/**
 * Check node type.
 *
 * @param $template
 * @return bool
 */
function reasons_bounce_check_node_type($template, $type) {
  if (!$template['node_types']) {
    return FALSE;
  }
  $types = array_diff($template['node_types'], array(0));
  if (!empty($types)) {
    if (!empty($types[$type])) {
      return TRUE;
    }
    return FALSE;
  }
  return TRUE;
}

/**
 * Check roles of current user.
 *
 * @param $template
 * @return bool
 */
function reasons_bounce_check_roles($template) {
  if (!$template['roles']) {
    return FALSE;
  }
  $roles = array_diff($template['roles'], array(0));
  if (!empty($roles)) {
    global $user;
    foreach ($user->roles as $key => $item) {
      if (in_array($key, $roles)) {
        return TRUE;
      } 
    }
    return FALSE;
  }
  return TRUE;
}

/**
 * Check visibility of template.
 *
 * @param $template
 * @return bool
 */
function reasons_bounce_check_visibility($template) {
  $page_match = FALSE;
  // @see block_block_list_alter().
  if ($template['access_pages']) {
    $pages = drupal_strtolower($template['access_pages']);
    if ($template['visibility'] != BOUNCE_FORM_PHP_FILTER) {
      $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));
      $page_match = drupal_match_path($path, $pages);
      if ($path != $_GET['q']) {
        $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
      }
      $page_match = !($template['visibility'] xor $page_match);
    }
    elseif (module_exists('php')) {
      $page_match = php_eval($pages);
    }
    else {
      $page_match = FALSE;
    }
  }
  else {
    $page_match = TRUE;
  }
  return $page_match;
}

/**
 * Enable tracking for customers who leave
 */
function reasons_bounce_enable_tracking($node = NULL) {
  $template = reasons_bounce_check_environment($node);
  if (empty($template)) {
    return;
  }
  // React with the modal
  // Add modal components  
  ctools_include('ajax');
  ctools_include('modal');
  ctools_modal_add_js();
  
  // Create own theme a modal.
  $reasons_bounce_style = array(
    'CToolsReasonsBounceModal' => array(
      'modalSize' => array(
        'type' => 'fixed',
        'width' => 500,
        'height' => 300,
        'addWidth' => 20,
        'addHeight' => 15,
      ),
      'modalOptions' => array(
        'opacity' => .5,
        'background-color' => '#000',
      ),
      'animation' => 'fadeIn',
      'modalTheme' => 'CToolsReasonsBounceModal',
      'throbberSrc' => ctools_image_path('throbber.gif'),
      'throbber' => theme('image', array(
        'path' => base_path() . ctools_image_path('ajax-loader.gif', 'ctools_ajax_sample'), 
        'alt' => t('Loading...'), 
        'title' => t('Loading')
      )),

    ),
  );
  drupal_add_js($reasons_bounce_style, 'setting');
  
  $reasons_bounce_settings = array(
    'reasons_bounce' => array(
      'message' => t($template['message']),
      'template' => t($template['template']),
    ),
  );
  drupal_add_js($reasons_bounce_settings, 'setting');
  
  drupal_add_css(drupal_get_path('module', 'reasons_bounce') . '/includes/css/reasons_bounce.css');
  drupal_add_js(drupal_get_path('module', 'reasons_bounce') . '/includes/js/reasons_bounce.close.js');
}