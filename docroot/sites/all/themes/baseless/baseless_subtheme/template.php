<?php
/**
 * @file
 * Theme Functions
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * IMPORTANT! Change baseless_subtheme to your theme name
 *
 */
 
/**
 * Implements hook_css_alter().
 *
 */
function baseless_subtheme_css_alter(&$css) {
  $exclude = array(
    'misc/vertical-tabs.css' => FALSE,
    'modules/aggregator/aggregator.css' => FALSE,
    'modules/block/block.css' => FALSE,
    'modules/book/book.css' => FALSE,
    'modules/color/color.css' => FALSE,
    'modules/comment/comment.css' => FALSE,
    //'modules/contextual/contextual.css' => FALSE,
    //'modules/dashboard/dashboard.css' => FALSE,
    'modules/dblog/dblog.css' => FALSE,
    'modules/field/theme/field.css' => FALSE,
    'modules/field_ui/field_ui.css' => FALSE,
    'modules/file/file.css' => FALSE,
    'modules/filter/filter.css' => FALSE,
    'modules/forum/forum.css' => FALSE,
    'modules/help/help.css' => FALSE,
    'modules/image/image.css' => FALSE,
    'modules/locale/locale.css' => FALSE,
    'modules/menu/menu.css' => FALSE,
    'modules/node/node.css' => FALSE,
    'modules/openid/openid.css' => FALSE,
    //'modules/overlay/overlay-child.css' => FALSE,
    //'modules/overlay/overlay-child-rtl.css' => FALSE,
    //'modules/overlay/overlay-parent.css' => FALSE,
    'modules/poll/poll.css' => FALSE,
    'modules/profile/profile.css' => FALSE,
    'modules/search/search.css' => FALSE,
    'modules/shortcut/shortcut.css' => FALSE,
    'modules/simpletest/simpletest.css' => FALSE,
    'modules/statistics/statistics.css' => FALSE,
    'modules/syslog/syslog.css' => FALSE,
    'modules/system/admin.css' => FALSE,
    'modules/system/maintenance.css' => FALSE,
    'modules/system/system.css' => FALSE,
    'modules/system/system.admin.css' => FALSE,
    'modules/system/system.base.css' => FALSE,
    'modules/system/system.maintenance.css' => FALSE,
    'modules/system/system.menus.css' => FALSE,
    'modules/system/system.messages.css' => FALSE,
    'modules/system/system.theme.css' => FALSE,
    'modules/taxonomy/taxonomy.css' => FALSE,
    //'modules/toolbar/toolbar.css' => FALSE,
    'modules/tracker/tracker.css' => FALSE,
    'modules/update/update.css' => FALSE,
    'modules/user/user.css' => FALSE,
  );
  $css = array_diff_key($css, $exclude);
}


/**
 * Returns HTML for a breadcrumb trail.
 *
 * @param $variables
 *   An associative array containing:
 *   - breadcrumb: An array containing the breadcrumb links.
 */
function baseless_subtheme_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<div id="breadcrumb">';

    $output .= '<h2 class="element-invisible">' . t('You are here') . '</h2>';

    $output .= implode(' > ', $breadcrumb);

    $output .= '</div>';

    return $output;
  }
}


/**
 * Preprocess variables for region.tpl.php
 *
 * Prepare the values passed to the theme_region function to be passed into a
 * pluggable template engine. Uses the region name to generate a template file
 * suggestions. If none are found, the default region.tpl.php is used.
 *
 * @see drupal_region_class()
 * @see region.tpl.php
 */
function baseless_subtheme_preprocess_region(&$variables) {
  // Create the $content variable that templates expect.
}



