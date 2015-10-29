<?php
/**
 * @file
 * Global functionality for the Rijkshuisstijl theme.
 */

/**
 * Implements hook_preprocess().
 *
 * This function checks to see if a hook has a preprocess file associated with
 * it, and if so, loads it.
 *
 * @param Array $vars
 *   The variables array, by reference.
 *
 * @param String $hook
 *   The hook that called the function.
 */
function rijkshuisstijl_preprocess(&$vars, $hook) {
  $path = drupal_get_path('theme', 'rijkshuisstijl');
  $include_file =  $path . '/preprocess/preprocess-' . str_replace('_', '-', $hook) . '.inc';
  if (file_exists($include_file)) {
    include $include_file;
  }
}

/**
 * Implements hook_process_html().
 */
function rijkshuisstijl_process_html(&$vars) {
  // Flatten attributes arrays.
  $vars['html_attributes'] = empty($vars['html_attributes_array']) ? '' : drupal_attributes($vars['html_attributes_array']);

  // $rdf_namespaces is kept to maintain backwards compatibility, and because we
  // only want this to print once in html.tpl.php, and not in every conditional
  // comment for IE.
  $vars['rdf_namespaces'] = empty($vars['rdf_namespaces_array']) ? '' : drupal_attributes($vars['rdf_namespaces_array']);

  // If the color module is enabled, allow users to change the color scheme of
  // this theme.
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}

/**
 * Implements hook_process_page().
 */
function rijkshuisstijl_process_page(&$variables) {
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
}


/**
 * Implements hook_css_alter().
 */
function rijkshuisstijl_css_alter(&$css) {
  // Unsetting these could really do some damage, here for completeness,
  // don't unset unless you really know what you are doing.
  $disallowed_paths = array(
    'aggregator.css',
    'modules/block/block.css',
    // 'modules/book/book.css',
    'modules/color/color.css',
    'modules/comment/comment.css',
    // 'modules/contextual/contextual.css',
    'modules/dashboard/dashboard.css',
    'modules/dblog/dblog.css',
    // 'modules/field/theme/field.css',
    // 'modules/field_ui/field_ui.css',
    // 'modules/file/file.css',
    'modules/filter/filter.css',
    'modules/forum/forum.css',
    'modules/help/help.css',
    'modules/image/image.admin.css',
    'modules/image/image.css',
    // 'modules/locale/locale.css',
    'modules/menu/menu.css',
    'modules/node/node.css',
    'modules/openid/openid.css',
    'modules/poll/poll.css',
    'modules/profile/profile.css',
    'modules/search/search.css',
    // 'modules/shortcut/shortcut.admin.css',
    // 'modules/shortcut/shortcut.css',
    'modules/simpletest/simpletest.css',
    // 'modules/system/system.admin.css',
    // 'modules/system/system.base.css',
    'modules/system/system.maintenance.css',
    'modules/system/system.menus.css',
    'modules/system/system.messages.css',
    'modules/system/system.theme.css',
    'modules/taxonomy/taxonomy.css',
    // 'modules/toolbar/toolbar.css',
    'modules/tracker/tracker.css',
    'modules/update/update.css',
    'modules/user/user.css',
    'misc/vertical-tabs',
  );
  foreach ($css as $path => $options) {
    foreach ($disallowed_paths as $disallowed_path) {
      if (stristr($path, $disallowed_path)) {
        unset($css[$path]);
      }
    }
  }
}

/**
 * This helper function to 'cache' external files locally.
 *
 * It will copy the external file to Drupal's public file directory.
 *
 * @param string $external_url
 *   - The URL to the externally hosted file.
 */
function _rijkshuisstijl_load_file_locally(&$external_url) {
  $basename = drupal_basename($external_url);
  $target_dir = 'public://rijkshuisstijl';
  $destination = $target_dir . '/' . $basename;

  // If the file doesn't already exist attempt to create the containing
  // directory and try to copy the external file to the files directory.
  if (!file_exists($destination)) {
    if (file_prepare_directory($target_dir, FILE_CREATE_DIRECTORY)) {
      file_put_contents($destination, file_get_contents($external_url));
    }
  }

  // If everything went well we can safely overwrite the URL with a stream wrapper
  // URI to our locally stored file.
  if (file_exists($destination)) {
    $external_url = $destination;
  }
}

/**
 * Implements theme_menu_link().
 */
function rijkshuisstijl_menu_link(&$variables) {

  $element = $variables['element'];
  $sub_menu = '';

  // Strip sub-items from the main menu
  if ($element['#below'] && $element['#original_link']['menu_name'] != 'main-menu') {

    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}


function rijkshuisstijl_breadcrumb(&$variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<p id="breadcrumb"><span class="element-invisible">' . t('You are here') . '</span>' . implode('', $breadcrumb) . '</p>';

    if (count($breadcrumb) > 1 ){
      $output .= '<span class="topic-home">';
      $output .= end($breadcrumb);
      $output .= '</span>';
    }

    return $output;
  }
}
