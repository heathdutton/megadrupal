<?php

/**
 * @file
 * Term menu token and pathauto support.
 */

module_load_include('inc', 'pathauto');

/**
 * Implements hook_token_info().
 */
function term_menus_token_info() {
  // Add our own tokens at editing page.
  $type = array(
    'name' => t('Term menus'),
    'description' => t("Tokens related to individual term menu's term page."),
    'needs-data' => 'term_menus',
  );
  $term_menus['term-menu-machine'] = array(
    'name' => t("Term menu's machine name"),
    'description' => t('The machine name for this term menu, as what you see in default menu UI path URL.'),
  );
  $term_menus['term-menu-title'] = array(
    'name' => t("Term menu's title"),
    'description' => t('Title for this term menu.'),
  );
  $term_menus['node-type-machine'] = array(
    'name' => t("Content type's machine name"),
    'description' => t("The choosen/first node type's machine name where this term menu is enabled with taxonomy field."),
  );
  $term_menus['node-type-name'] = array(
    'name' => t("Content type's human name"),
    'description' => t("The choosen/first node type's human name where this term menu is enabled with taxonomy field."),
  );
  $term_menus['field-name'] = array(
    'name' => t('Taxonomy field machine name'),
    'description' => t('The taxonomy field where term menu is enabled.'),
  );
  $term_menus['field-instance-label'] = array(
    'name' => t('Taxonomy field label'),
    'description' => t('The choosen/first taxonomy field instance where term menu is enabled.'),
  );
  $term_menus['vocabulary-machine'] = array(
    'name' => t('Vocabulary machine name'),
    'description' => t('Vocabulary mahcine name of this term menu.'),
  );
  $term_menus['vocabulary-name'] = array(
    'name' => t('Vocabulary name'),
    'description' => t('Vocabulary name for this term menu.'),
  );
  $term_menus['vocabulary-id'] = array(
    'name' => t('Vocabulary id'),
    'description' => t('Vocabulary id for this term menu.'),
  );
  $term_menus['term-name'] = array(
    'name' => t("Term's name"),
    'description' => t("The taxonomy term's name this path is for."),
  );
  $term_menus['tid'] = array(
    'name' => t("Term's tid"),
    'description' => t("The taxonomy term's tid this path is for."),
  );
  return array(
    'types' => array('term_menus' => $type),
    'tokens' => array('term_menus' => $term_menus),
  );
}

/**
 * Implements hook_tokens().
 */
function term_menus_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();
  if ($type == 'term_menus' && !empty($data['term_menu'])) {
    $term_menu = $data['term_menu'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'term-menu-machine':
          $replacements[$original] = $term_menu->machine_name;
          break;

        case 'term-menu-title':
          $menu = menu_load($term_menu->machine_name);
          $replacements[$original] = $menu->title;
          break;

        case 'node-type-machine':
          $replacements[$original] = $term_menu->node_type;
          break;

        case 'node-type-name':
          $node_type = node_type_load($term_menu->node_type);
          $replacements[$original] = $node_type->name;
          break;

        case 'field-name':
          $replacements[$original] = $term_menu->field_name;
          break;

        case 'field-instance-label':
          $instance = field_info_instance('node', $term_menu->field_name, $term_menu->node_type);
          $replacements[$original] = $instance['label'];
          break;

        case 'vocabulary-machine':
          $vocabulary = taxonomy_vocabulary_load($term_menu->vid);
          $replacements[$original] = $vocabulary->machine_name;
          break;

        case 'vocabulary-name':
          $vocabulary = taxonomy_vocabulary_load($term_menu->vid);
          $replacements[$original] = $vocabulary->name;
          break;

        case 'vocabulary-id':
          $replacements[$original] = $term_menu->vid;
          break;

        case 'term-name':
          $replacements[$original] = $term_menu->term->name;
          break;

        case 'tid':
          $replacements[$original] = $term_menu->term->tid;
          break;

        default:
          break;
      }
    }
  }
  return $replacements;
}

/**
 * Implements hook_pathauto().
 */
function term_menus_pathauto($op) {
  switch ($op) {
    case 'settings':
      $settings = array(
        'module' => 'term_menus',
        'token_type' => 'term_menus',
        'groupheader' => t('Term menus custom path'),
        'patterndefault' => '[term_menus:node-type-name]/[term_menus:field-instance-label]/[term_menus:term-name]',
        'patterndescr' => t('Dafault path pattern for all term mneus.'),
        'batch_update_callback' => 'term_menus_pathauto_bulk_update',
        'batch_file' => drupal_get_path('module', 'term_menus') . '/term_menus.pathauto.inc',
      );
      $term_menus = _term_menus_get_menus();
      foreach ($term_menus as $term_menu) {
        $settings['patternitems'][$term_menu->field_name] = t("Pattern for all taxonomy field @field paths.", array('@field' => $term_menu->field_name));
      }
      return (object) $settings;

    default:
      break;
  }
}

 /**
  * Bulk update function for term menus path alias.
  *
  * @param array $context
  *   Batch context array. This param will persist across batch process.
  */
function term_menus_pathauto_bulk_update(&$context) {
  if (!isset($context['sandbox']['term_menus'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['term_menus'] = _term_menus_get_menus();
    $context['sandbox']['max'] = count($context['sandbox']['term_menus']);
  }
  if ($context['sandbox']['count'] == $context['sandbox']['max']) {
    $context['finished'] = 1;
    return;
  }
  $term_menu = array_shift($context['sandbox']['term_menus']);
  // Make sure this term menu make use of url alias.
  if (!$term_menu->custom_page || !$term_menu->url_alias) {
    return;
  }
  $terms = taxonomy_get_tree($term_menu->vid);
  foreach ($terms as $term) {
    $term_menu->term = $term;
    $data['term_menu'] = $term_menu;
    $source = 'term_menus/' . $term_menu->menu_name . '/' . $term->tid;
    pathauto_create_alias('term_menus', 'bulkupdate', $source, $data, $term_menu->field_name);
  }
  $context['sandbox']['count']++;
  if ($context['sandbox']['count'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['max'];
  }
}

 /**
  * Insert custom term page's path alias in to {url_alias}.
  *
  * This function will insert the path alias only when this term_menu
  * has used custom term page.
  *
  * @param object $term
  *   Term object we are creating url alias for.
  *
  * @param object $term_menu
  *   Term menu object this term associated with.
  */
function term_menus_insert_term_path_alias($term, $term_menu) {
  if (!$term_menu->custom_page || !$term_menu->url_alias) {
    return;
  }
  $term_menu->term = $term;
  $data['term_menu'] = $term_menu;
  $source = 'term_menus/' . $term_menu->menu_name . '/' . $term->tid;
  pathauto_create_alias('term_menus', 'insert', $source, $data, $term_menu->field_name);
}

 /**
  * Update custom term page's path alias in to {url_alias}.
  *
  * This function will update the path alias only when this term_menu
  * has used custom term page.
  *
  * @param object $term
  *   Term object we are creating url alias for.
  *
  * @param object $term_menu
  *   Term menu object this term associated with.
  */
function term_menus_update_term_path_alias($term, $term_menu) {
  if (!$term_menu->custom_page || !$term_menu->url_alias) {
    return;
  }
  $term_menu->term = $term;
  $data['term_menu'] = $term_menu;
  $source = 'term_menus/' . $term_menu->menu_name . '/' . $term->tid;
  pathauto_create_alias('term_menus', 'update', $source, $data, $term_menu->field_name);
}

/**
 * Delete custom term page url alias, when a term is deleted.
 *
 * @param object $term
 *   Term object we are creating url alias for.
 *
 * @param object $term_menu
 *   Term menu object this term associated with.
 */
function term_menus_delete_term_path_alias($term, $term_menu) {
  if (!$term_menu->custom_page || !$term_menu->url_alias) {
    return;
  }
  $source = 'term_menus/' . $term_menu->menu_name . '/' . $term->tid;
  $criteria = array('source' => $source);
  path_delete($criteria);
}

/**
 * Delete all terms url alias for a term menu.
 *
 * @param object $term_menu
 *   Term menu object we are process.
 */
function term_menus_delete_terms_path_alias($term_menu) {
  if (!$term_menu->custom_page || !$term_menu->url_alias) {
    return;
  }
  $terms = taxonomy_get_tree($term_menu->vid);
  foreach ($terms as $term) {
    term_menus_delete_term_path_alias($term, $term_menu);
  }
}
