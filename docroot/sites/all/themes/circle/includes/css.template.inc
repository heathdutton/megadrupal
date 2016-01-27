<?php

/**
 * @file
 * Contains the theme function overrides for css.
 */

/**
 * Implements hook_css_alter().
 */
function circle_css_alter(&$css) {
  // Exclude all the unused core and modules css.
  if (theme_get_setting('circle_clearcss')) {
    $exclude = array(
      'misc/vertical-tabs.css' => FALSE,
      'modules/aggregator/aggregator.css' => FALSE,
      'modules/block/block.css' => FALSE,
      'modules/book/book.css' => FALSE,
      'modules/comment/comment.css' => FALSE,
      'modules/dblog/dblog.css' => FALSE,
      'modules/file/file.css' => FALSE,
      'modules/filter/filter.css' => FALSE,
      'modules/forum/forum.css' => FALSE,
      'modules/help/help.css' => FALSE,
      'modules/menu/menu.css' => FALSE,
      'modules/node/node.css' => FALSE,
      'modules/openid/openid.css' => FALSE,
      'modules/poll/poll.css' => FALSE,
      'modules/profile/profile.css' => FALSE,
      'modules/search/search.css' => FALSE,
      'modules/statistics/statistics.css' => FALSE,
      'modules/syslog/syslog.css' => FALSE,
      'modules/system/admin.css' => FALSE,
      'modules/system/maintenance.css' => FALSE,
      'modules/system/system.css' => FALSE,
      'modules/system/system.admin.css' => FALSE,
      'modules/system/system.maintenance.css' => FALSE,
      'modules/system/system.menus.css' => FALSE,
      'modules/system/system.messages.css' => FALSE,
      'modules/system/system.theme.css' => FALSE,
      'modules/taxonomy/taxonomy.css' => FALSE,
      'modules/tracker/tracker.css' => FALSE,
      'modules/update/update.css' => FALSE,
      'modules/user/user.css' => FALSE,
    );
    $css = array_diff_key($css, $exclude);
  }

  // Combining all the css files into single file.
  if (theme_get_setting('circle_css_onefile')) {
    foreach ($css as $path => $value) {
      if ($css[$path]['media'] == 'all') {
        $css[$path]['media'] = 'screen';
      }
    }
    uasort($css, 'drupal_sort_css_js');
    $i = 0;
    foreach ($css as $name => $style) {
      $css[$name]['weight'] = $i++;
      $css[$name]['group'] = CSS_DEFAULT;
      $css[$name]['every_page'] = FALSE;
    }
  }
}
