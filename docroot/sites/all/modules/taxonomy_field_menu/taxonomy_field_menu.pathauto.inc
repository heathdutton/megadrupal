<?php

/**
 * @file
 * taxonomy field menu token and pathauto support.
 */

module_load_include('inc', 'pathauto');

/**
 * Implements hook_token_info().
 */
function taxonomy_field_menu_token_info() {
  // Add our own tokens at editing page.
  $type = array(
    'name' => t('taxonomy field menus'),
    'description' => t("Tokens related to individual taxonomy field menu's term page."),
    'needs-data' => 'taxonomy_field_menu',
  );
  $taxonomy_field_menus['taxonomy-field-menu-machine'] = array(
    'name' => t("taxonomy field menu's machine name"),
    'description' => t('The machine name for this taxonomy field menu, as what you see in default menu UI path URL.'),
  );
  $taxonomy_field_menus['taxonomy-field-menu-title'] = array(
    'name' => t("taxonomy field menu's title"),
    'description' => t('Title for this taxonomy field menu.'),
  );
  $taxonomy_field_menus['node-type-machine'] = array(
    'name' => t("Content type's machine name"),
    'description' => t("The choosen/first node type's machine name where this taxonomy field menu is enabled with taxonomy field."),
  );
  $taxonomy_field_menus['node-type-name'] = array(
    'name' => t("Content type's human name"),
    'description' => t("The choosen/first node type's human name where this taxonomy field menu is enabled with taxonomy field."),
  );
  $taxonomy_field_menus['field-name'] = array(
    'name' => t('Taxonomy field machine name'),
    'description' => t('The taxonomy field where taxonomy field menu is enabled.'),
  );
  $taxonomy_field_menus['field-instance-label'] = array(
    'name' => t('Taxonomy field label'),
    'description' => t('The choosen/first taxonomy field instance where taxonomy field menu is enabled.'),
  );
  $taxonomy_field_menus['vocabulary-machine'] = array(
    'name' => t('Vocabulary machine name'),
    'description' => t('Vocabulary mahcine name of this taxonomy field menu.'),
  );
  $taxonomy_field_menus['vocabulary-name'] = array(
    'name' => t('Vocabulary name'),
    'description' => t('Vocabulary name for this taxonomy field menu.'),
  );
  $taxonomy_field_menus['vocabulary-id'] = array(
    'name' => t('Vocabulary id'),
    'description' => t('Vocabulary id for this taxonomy field menu.'),
  );
  $taxonomy_field_menus['term-name'] = array(
    'name' => t("Term's name"),
    'description' => t("The taxonomy term's name this path is for."),
  );
  $taxonomy_field_menus['tid'] = array(
    'name' => t("Term's tid"),
    'description' => t("The taxonomy term's tid this path is for."),
  );
  return array(
    'types' => array('taxonomy_field_menu' => $type),
    'tokens' => array('taxonomy_field_menu' => $taxonomy_field_menus),
  );
}

/**
 * Implements hook_tokens().
 */
function taxonomy_field_menu_tokens($type, $tokens, array $data = array(), array $options = array()) {
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
  if ($type == 'taxonomy_field_menu' && !empty($data['taxonomy_field_menu'])) {
    $taxonomy_field_menu = $data['taxonomy_field_menu'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'taxonomy-field-menu-machine':
          $replacements[$original] = $taxonomy_field_menu->machine_name;
          break;

        case 'taxonomy-field-menu-title':
          $menu = menu_load($taxonomy_field_menu->machine_name);
          $replacements[$original] = $menu->title;
          break;

        case 'node-type-machine':
          $replacements[$original] = $taxonomy_field_menu->node_type;
          break;

        case 'node-type-name':
          $node_type = node_type_load($taxonomy_field_menu->node_type);
          $replacements[$original] = $node_type->name;
          break;

        case 'field-name':
          $replacements[$original] = $taxonomy_field_menu->field_name;
          break;

        case 'field-instance-label':
          $instance = field_info_instance('node', $taxonomy_field_menu->field_name, $taxonomy_field_menu->node_type);
          $replacements[$original] = $instance['label'];
          break;

        case 'vocabulary-machine':
          $vocabulary = taxonomy_vocabulary_load($taxonomy_field_menu->vid);
          $replacements[$original] = $vocabulary->machine_name;
          break;

        case 'vocabulary-name':
          $vocabulary = taxonomy_vocabulary_load($taxonomy_field_menu->vid);
          $replacements[$original] = $vocabulary->name;
          break;

        case 'vocabulary-id':
          $replacements[$original] = $taxonomy_field_menu->vid;
          break;

        case 'term-name':
          $replacements[$original] = $taxonomy_field_menu->term->name;
          break;

        case 'tid':
          $replacements[$original] = $taxonomy_field_menu->term->tid;
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
function taxonomy_field_menu_pathauto($op) {
  switch ($op) {
    case 'settings':
      $settings = array(
        'module' => 'taxonomy_field_menu',
        'token_type' => 'taxonomy_field_menu',
        'groupheader' => t('taxonomy field menus custom path'),
        'patterndefault' => '[taxonomy_field_menu:node-type-name]/[taxonomy_field_menu:field-instance-label]/[taxonomy_field_menu:term-name]',
        'patterndescr' => t('Dafault path pattern for all term mneus.'),
        'batch_update_callback' => 'taxonomy_field_menu_pathauto_bulk_update',
        'batch_file' => drupal_get_path('module', 'taxonomy_field_menu') . '/taxonomy_field_menu.pathauto.inc',
      );
      $taxonomy_field_menus = _taxonomy_field_menu_get_menus();
      foreach ($taxonomy_field_menus as $taxonomy_field_menu) {
        $settings['patternitems'][$taxonomy_field_menu->field_name] = t("Pattern for all taxonomy field @field paths.", array('@field' => $taxonomy_field_menu->field_name));
      }
      return (object) $settings;

    default:
      break;
  }
}

 /**
  * Bulk update function for taxonomy field menus path alias.
  *
  * @param array $context
  *   Batch context array. This param will persist across batch process.
  */
function taxonomy_field_menu_pathauto_bulk_update(&$context) {
  if (!isset($context['sandbox']['taxonomy_field_menu'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['taxonomy_field_menu'] = _taxonomy_field_menu_get_menus();
    $context['sandbox']['max'] = count($context['sandbox']['taxonomy_field_menu']);
  }
  if ($context['sandbox']['count'] == $context['sandbox']['max']) {
    $context['finished'] = 1;
    return;
  }
  $taxonomy_field_menu = array_shift($context['sandbox']['taxonomy_field_menu']);
  // Make sure this taxonomy field menu make use of url alias.
  if (!$taxonomy_field_menu->custom_page || !$taxonomy_field_menu->url_alias) {
    return;
  }
  $terms = taxonomy_get_tree($taxonomy_field_menu->vid);
  foreach ($terms as $term) {
    $taxonomy_field_menu->term = $term;
    $data['taxonomy_field_menu'] = $taxonomy_field_menu;
    $source = 'taxonomy_field_menu/' . $taxonomy_field_menu->menu_name . '/' . $term->tid;
    pathauto_create_alias('taxonomy_field_menu', 'bulkupdate', $source, $data, $taxonomy_field_menu->field_name);
  }
  $context['sandbox']['count']++;
  if ($context['sandbox']['count'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['max'];
  }
}

 /**
  * Insert custom term page's path alias in to {url_alias}.
  *
  * This function will insert the path alias only when this taxonomy_field_menu
  * has used custom term page.
  *
  * @param object $term
  *   Term object we are creating url alias for.
  *
  * @param object $taxonomy_field_menu
  *   taxonomy field menu object this term associated with.
  */
function taxonomy_field_menu_insert_term_path_alias($term, $taxonomy_field_menu) {
  if (!$taxonomy_field_menu->custom_page || !$taxonomy_field_menu->url_alias) {
    return;
  }
  $taxonomy_field_menu->term = $term;
  $data['taxonomy_field_menu'] = $taxonomy_field_menu;
  $source = 'taxonomy_field_menu/' . $taxonomy_field_menu->menu_name . '/' . $term->tid;
  pathauto_create_alias('taxonomy_field_menu', 'insert', $source, $data, $taxonomy_field_menu->field_name);
}

 /**
  * Update custom term page's path alias in to {url_alias}.
  *
  * This function will update the path alias only when this taxonomy_field_menu
  * has used custom term page.
  *
  * @param object $term
  *   Term object we are creating url alias for.
  *
  * @param object $taxonomy_field_menu
  *   taxonomy field menu object this term associated with.
  */
function taxonomy_field_menu_update_term_path_alias($term, $taxonomy_field_menu) {
  if (!$taxonomy_field_menu->custom_page || !$taxonomy_field_menu->url_alias) {
    return;
  }
  $taxonomy_field_menu->term = $term;
  $data['taxonomy_field_menu'] = $taxonomy_field_menu;
  $source = 'taxonomy_field_menu/' . $taxonomy_field_menu->menu_name . '/' . $term->tid;
  pathauto_create_alias('taxonomy_field_menu', 'update', $source, $data, $taxonomy_field_menu->field_name);
}

/**
 * Delete custom term page url alias, when a term is deleted.
 *
 * @param object $term
 *   Term object we are creating url alias for.
 *
 * @param object $taxonomy_field_menu
 *   taxonomy field menu object this term associated with.
 */
function taxonomy_field_menu_delete_term_path_alias($term, $taxonomy_field_menu) {
  if (!$taxonomy_field_menu->custom_page || !$taxonomy_field_menu->url_alias) {
    return;
  }
  $source = 'taxonomy_field_menu/' . $taxonomy_field_menu->menu_name . '/' . $term->tid;
  $criteria = array('source' => $source);
  path_delete($criteria);
}

/**
 * Delete all terms url alias for a taxonomy field menu.
 *
 * @param object $taxonomy_field_menu
 *   taxonomy field menu object we are process.
 */
function taxonomy_field_menu_delete_terms_path_alias($taxonomy_field_menu) {
  if (!$taxonomy_field_menu->custom_page || !$taxonomy_field_menu->url_alias) {
    return;
  }
  $terms = taxonomy_get_tree($taxonomy_field_menu->vid);
  foreach ($terms as $term) {
    taxonomy_field_menu_delete_term_path_alias($term, $taxonomy_field_menu);
  }
}
