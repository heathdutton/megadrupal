<?php

/**
 * Holds callback function.
 */

/**
 * Renders single page.
 */
function single_page_site_callback() {
  $settings = variable_get('single_page_site_settings', array());
  $items = array();
  $current_item_count = 1;
  if (empty($settings)) {
    // If settings aren't set.
    return t('You have to <a href="@url">configure</a> single page before you can use it.',
        array('@url' => url('admin/config/system/single-page-site')));
  }

  $messages = drupal_get_messages();

  $menu_name = $settings['menu'];
  $tree = menu_tree($menu_name);

  foreach ($tree as $key => $item) {
    $output = NULL;
    // Check if menu item has to be rendered.
    $render_menu_item = FALSE;
    if (is_numeric($key)) {
      if ($item['#href'] != '<front>') {
        if (empty($settings['class'])) {
          // If class is empty => all menu items.
          $render_menu_item = TRUE;
        }
        elseif (!empty($item['#title']) && !empty($item['#localized_options']['attributes']['class']) && in_array($settings['class'],
                $item['#localized_options']['attributes']['class'])) {
          // If class is filled out => Only menu items with class.
          $render_menu_item = TRUE;
        }
      }
    }
    if ($render_menu_item) {
      $href = check_plain($item['#href']);
      $anchor = check_plain(_single_page_site_generate_anchor(drupal_get_path_alias($href)));
      $item_title = _single_page_site_get_title($item);
      $item_tag = check_plain($settings['tag']);

      // Set the q to ... required to get the block system working.
      $q_orig = $_GET['q'];
      $_GET['q'] = $href;

      // Collect generated messages.
      $messages += drupal_get_messages();

      // We mimic menu_execute_active_handler().
      $router_item = menu_get_item($href);
      if ($router_item['access']) {
        if ($router_item['include_file']) {
          require_once DRUPAL_ROOT . '/' . $router_item['include_file'];
        }
        $page_callback_result = call_user_func_array($router_item['page_callback'],
            $router_item['page_arguments']);
      }
      else {
        // Access denied.
        $page_callback_result = MENU_ACCESS_DENIED;
      }

      if (is_array($page_callback_result)) {
        // Render array.
        $output .= drupal_render($page_callback_result);
      }
      elseif (is_string($page_callback_result)) {
        // Render string.
        $output .= $page_callback_result;
      }
      else {
        // Report error.
        $output .= '<p>' . t("An error '!callback' occurred for %link",
                array('!callback' => $page_callback_result, '%link' => $item['#href'])) . '</p>';
      }

      // Let other modules makes changes to $output.
      drupal_alter('single_page_site_output', $output, $current_item_count);

      // Build renderable array.
      $item = array(
        'output' => $output,
        'anchor' => $anchor,
        'title' => $item_title,
        'tag' => $item_tag,
        'hide' => !empty($item['#localized_options']['attributes']['class']) &&
        in_array('hide', $item['#localized_options']['attributes']['class']) ? 'no-menu-item' : 'menu-item',
      );

      $current_item_count++;
      $items[] = $item;
      $_GET['q'] = $q_orig;
    }
  }

  // Reinject the messages?.
  foreach ($messages as $type => $data) {
    foreach ($data as $message) {
      drupal_set_message($message, $type);
    }
  }

  // Render output and attach JS files.
  $js_settings = array(
    'singlePage' => array(
      'menuClass' => $settings['menuclass'],
      'distanceUp' => $settings['up'],
      'distanceDown' => $settings['down'],
      'updateHash' => $settings['updatehash'],
      'offsetSelector' => $settings['offset_selector'],
    ),
  );

  $js = array(
    array(
      'data' => drupal_get_path('module', 'single_page_site') . '/lib/jquery-scrollspy.js',
      'type' => 'file',
    ),
    array(
      'data' => drupal_get_path('module', 'single_page_site') . '/js/single-page-site-scrollspy.js',
      'type' => 'file',
    ),
    array(
      'data' => $js_settings,
      'type' => 'setting',
    ),
  );

  if ($settings['smoothscrolling']) {
    $js[] = array(
      'data' => drupal_get_path('module', 'single_page_site') . '/js/single-page-site-scroll.js',
      'type' => 'file',
      'weight' => 100,
    );
  }

  $page_content = array(
    '#theme' => 'single_page_site',
    '#items' => $items,
    '#attached' => array(
      'js' => $js,
      'css' => array(
        array(
          'data' => $settings['menuclass'] . ' ul li .hide{display: none;}',
          'type' => 'inline',
        ),
      ),
    ),
  );

  drupal_set_title(t('!title', array('!title' => $settings['title'])));
  return $page_content;
}
