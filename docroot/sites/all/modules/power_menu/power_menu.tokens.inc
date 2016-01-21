<?php

/**
 * @file
 * Builds placeholder replacement tokens for power_menu.
 */

/**
 * Implements hook_token_info().
 */
function power_menu_token_info() {

  $types['power_menu'] = array(
    'name' => t("Power Menu"),
    'description' => t("Power Menu related tokens."),
  );

  $tokens['menu_path'] = array(
    'name' => t("Menu Path"),
    'description' => t("The menu path or alias form selected menu as token (Works at the moment only with nodes!)."),
  );

  $tokens['menu_item_names_path'] = array(
    'name' => t("Menu Item Names"),
    'description' => t("The menu names from the active menu trail."),
  );

  return array(
    'types' => $types,
    'tokens' => array(
      'power_menu' => $tokens,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function power_menu_tokens($type, $tokens, array $data = array(), array $options = array()) {
  global $base_url;
  $replacements = array();

  if ($type == 'power_menu') {

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'menu_path':
          $alias = '';

          // TODO: Implement generic handling. Not only for nodes
          if (isset($data['node'])) {
            $path = power_menu_get_path($data['node'], 'node', 'node/' . $data['node']->nid, FALSE);
            $alias = drupal_get_path_alias($path);
          }

          $replacements[$original] = $alias;
          break;

        case 'menu_item_names_path':
          $alias = '';
          $parents = array();

          // TODO: Implement generic handling. Not only for nodes
          if (isset($data['node'])) {

            $node = $data['node'];
            $mlid = NULL;

            // Is the node attached to a menu item, use this
            if (!empty($node->menu['mlid'])) {
              $parents = token_menu_link_load_all_parents($node->menu['mlid']);
            }
            elseif (($mlid = db_select('menu_links', 'ml')
                ->condition('ml.link_path', 'node/' . $node->nid)
                ->condition('ml.menu_name', 'main-menu')
                ->fields('ml', array('mlid'))
                ->execute()->fetchField()) !== FALSE
            ) {

              $parents = token_menu_link_load_all_parents($mlid);
              $parents = _power_menu_translate_menu_link_titles($parents, $node->language);
            }
            else {
              // Get the activated path activated by power menu
              $path = power_menu_get_path($node, 'node', 'node/' . $node->nid, FALSE);
              // Set menu, when only one is set
              $menu = count(variable_get('power_menu_handlers_menus', array())) == 1 ? current(variable_get('power_menu_handlers_menus', array())) : NULL;

              if (!empty($path)) {
                $mlid = FALSE;

                if (module_exists('i18n_menu')) {
                  // Lookup for path in given language
                  $query = db_select('menu_links', 'ml')
                    ->condition('ml.link_path', $path)
                    ->condition('ml.menu_name', $menu)
                    ->condition('ml.language', $node->language, '=')
                    ->fields('ml', array('mlid'));

                  $mlid = $query->execute()->fetchField();

                  // Lookup for path with no language when no one is found with given language
                  if($mlid === FALSE) {
                    $query = db_select('menu_links', 'ml')
                      ->condition('ml.link_path', $path)
                      ->condition('ml.menu_name', $menu)
                      ->condition('ml.language', LANGUAGE_NONE, '=')
                      ->fields('ml', array('mlid'));

                    $mlid = $query->execute()->fetchField();
                  }
                }
                else {
                  $menu_link = menu_link_get_preferred($path, $menu);

                  if($menu_link !== FALSE) {
                    $mlid = $menu_link['mlid'];
                  }
                }

                if ($mlid !== FALSE) {
                  // Load the parents to get all menu-link titles
                  $menu_item = token_menu_link_load($mlid);
                  $parents = token_menu_link_load_all_parents($menu_item['mlid']);
                  $parents[$menu_item['mlid']] = $menu_item['link_title'];

                  $parents = _power_menu_translate_menu_link_titles($parents, $node->language);
                }

              }
              else {
                $alias = 'no_active_menu_item_found';
              }
            }
          }

          // Generate path from given menu-items
          if (count($parents) > 0) {
            $parents_sanitized = array();

            foreach ($parents as $parent) {
              $parents_sanitized[] = pathauto_cleanstring($parent);
            }

            $alias = implode('/', $parents_sanitized);
          }

          $replacements[$original] = $alias;
          break;
      }
    }
  }

  return $replacements;
}

/**
 * Translate the menu link titles.
 *
 * @param $parents
 *   An array of titles to translate
 * @param $language
 *   The language
 * @return array
 *   The array with the translated titles
 */
function _power_menu_translate_menu_link_titles($parents, $language) {
  // When the i18n menu translation function exists, use it in case that the token functions do not allow a
  // language parameter. When the i18n sync is enabled all related nodes (translations) of the saved node are
  // also updated (when token is used for path auto patterns). In this case it is important, that we get the title
  // in the proper language and not the current language of the page request.
  if (function_exists('_i18n_menu_link_title')) {

    $parents_translated = array();

    foreach ($parents as $key => $parent) {
      $menu_link = menu_link_load($key);
      $parents_translated[$key] = _i18n_menu_link_title($menu_link, $language);
    }

    return $parents_translated;
  }
}

