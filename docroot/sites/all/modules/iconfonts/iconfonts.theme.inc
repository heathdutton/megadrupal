<?php

/**
 * @file
 * Theme and preprocess functions for the iconfonts module.
 */

/**
 * Implements hook_preprocess_HOOK() for theme_iconfonts_char_list().
 */
function template_preprocess_iconfonts_char_list(&$vars) {

  $characters = $vars['characters'];

  if (!empty($characters)) {

    // Saves the font id and name, it might be needed later.
    $vars['fid'] = current($characters)->fid;
    $vars['module'] = iconfonts_get_module_name_by_fid(current($characters)->fid);

    $options = array(
      'query' => array(
        // Need to set destination explicitly as $_GET['destination'] is
        // already set on the font edit page.
        'destination' => current_path(),
        ),
      'html' => TRUE,
      );

     // Saves a render array to $vars['render'] to make it easier for submodules
    // to modify the output. theme_iconfonts_char_list() uses this render array
    // to display characters.
    $render = array();
    foreach ($characters as $character) {

      $link_title = '<span class="' . iconfonts_get_generic_char_class($character) . '"></span>';
      $link_url = 'admin/iconfonts/' . $character->cid . '/edit';

      $render[] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $link_title,
          '#href' => $link_url,
          '#options' => $options,
        ),
      );

    }

    $vars['render'] = $render;

  }

  // Adds an empty render array in case there are no characters.
  else {

    $vars['render'] = array();

  }

}

/**
 * Returns HTML for iconfonts character lists.
 *
 * @param array $vars
 *   An associative array containing:
 *   - characters: An array of character objects, keyed by the character id.
 *   - enabled: A boolean indicating whether the characters are enabled.
 *   - render: An array of render arrays constructed in the preprocess.
 *
 * @see template_preprocess_iconfonts_char_list()
 * @ingroup themeable
 */
function theme_iconfonts_char_list($vars) {

  if ($vars['enabled']) {

    $title = t('Enabled characters');
    $css_id = 'iconfonts-enabled';

  }

  else {

    $title = t('Disabled characters');
    $css_id = 'iconfonts-disabled';

  }

  $items = array();
  foreach ($vars['render'] as $character) {

    $items[] = drupal_render($character);

  }

  $item_list = array(
    'items' => $items,
    'title' => $title,
    'attributes' => array(
      'id' => $css_id,
    ),
  );

  return theme('item_list', $item_list);

}
