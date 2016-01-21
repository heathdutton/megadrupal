<?php
/**
 * This plugin array is more or less self documenting
 */
$plugin = array(
  'title' => t('Schedule Day Menu'),
  'single' => TRUE,
  'category' => array(t('Conference Organizing'), -9),
  'edit form' => 'cod_schedule_pane_menu_edit_form',
  'render callback' => 'cod_schedule_menu_pane_render'
);

function cod_schedule_menu_pane_render($subtype, $conf, $context = NULL) {
  $block = new stdClass();
  $block->title = '';

  $content_type = !empty($conf['content_type']) ? $conf['content_type'] : NULL;
  $path_prefix = !empty($conf['path_prefix']) ? $conf['path_prefix'] .'/' : NULL;
  $date_format = !empty($conf['date_format']) ? $conf['date_format'] : 'l';

  if ($event = node_load(array_shift($context))) {
    $block->content = cod_schedule_menu_render($event, array_shift($context), $content_type, $path_prefix, $date_format);
  } else {
    $block->content = t('Invalid Event Type');
  }
  return $block;
}

/*
 * functions similar to menu_tree_output
 */
function cod_schedule_menu_render($event, $context = NULL, $content_type = NULL, $path_prefix = '', $date_format = 'l') {
  $build = array();
  $items = array();

  // Grab the days.
  if ($days = cod_schedule_days($event, $content_type)) {
    $router_item = menu_get_item();

    // Pull out just the menu links we are going to render so that we
    // get an accurate count for the first/last classes.
    foreach ($days as $k => $day) {
      $items[$k]['link']['href'] = dirname(current_path()) . '/' . $day;
      if ($router_item['href'] == $items[$k]['link']['href']) {
        $items[$k]['link']['in_active_trail'] = TRUE;
      }
      else {
        $items[$k]['link']['in_active_trail'] = FALSE;
      }
      $items[$k]['link']['title'] = date($date_format, strtotime($day));
    }

  }

  $num_items = count($items);
  foreach ($items as $i => $data) {
    $class = array();
    if ($i == 0) {
      $class[] = 'first';
    }
    if ($i == $num_items - 1) {
      $class[] = 'last';
    }
    $class[] = 'leaf';
    // Set a class if the link is in the active trail.
    if ($data['link']['in_active_trail']) {
      $class[] = 'active-trail';
      $data['link']['localized_options']['attributes']['class'][] = 'active-trail';
    }
    // Normally, l() compares the href of every link with $_GET['q'] and sets
    // the active class accordingly. But local tasks do not appear in menu
    // trees, so if the current path is a local task, and this link is its
    // tab root, then we have to set the class manually.
    if ($data['link']['href'] == $router_item['tab_root_href'] && $data['link']['href'] != $_GET['q']) {
      $data['link']['localized_options']['attributes']['class'][] = 'active';
    }

    // Allow menu-specific theme overrides.
    $element['#theme'] = 'menu_link__'. 'cod_schedule';
    $element['#attributes']['class'] = $class;
    $element['#title'] = $data['link']['title'];
    $element['#href'] = $path_prefix . $data['link']['href'];
    $element['#localized_options'] = !empty($data['link']['localized_options']) ? $data['link']['localized_options'] : array();
    $element['#original_link'] = $data['link'];
    $element['#below'] = array();
    // Index using the link's unique mlid.
    // Index using the link's unique mlid.
    $build[] = $element;
  }
  if ($build) {
    // Make sure drupal_render() does not re-order the links.
    $build['#sorted'] = TRUE;
    // Add the theme wrapper for outer markup.
    // Allow menu-specific theme overrides.
    $build['#theme_wrappers'][] = 'menu_tree__' . 'cod_schedule';
  }

  return $build;
}

function cod_schedule_pane_menu_edit_form($form, &$form_state) {
  $conf = &$form_state['conf'];

  $form['content_type'] = array(
    '#title' => t('Enter an optional content type'),
    '#type' => 'textfield',
    '#maxlength' => 64,
    '#default_value' => !empty($conf['content_type']) ? $conf['content_type'] : '',
    '#weight' => -11,
  );

  $form['path_prefix'] = array(
    '#title' => t('Enter an path prefix'),
    '#type' => 'textfield',
    '#maxlength' => 64,
    '#description' => 'Add an optional path prefix before the date. Do not append a slash',
    '#default_value' => !empty($conf['path_prefix']) ? $conf['path_prefix'] : '',
    '#weight' => -2,
  );

  $form['date_format'] = array(
    '#title' => t('Optional day title format'),
    '#type' => 'textfield',
    '#maxlength' => 32,
    '#default_value' => !empty($conf['date_format']) ? $conf['date_format'] : '',
    '#weight' => -10,
  );

  return $form;
}

function cod_schedule_pane_menu_edit_form_submit($form, &$form_state) {
  // This will either be the value set previously or a value set by the
  // validator.
  $form_state['conf']['content_type'] = $form_state['values']['content_type'];
  $form_state['conf']['path_prefix'] = $form_state['values']['path_prefix'];
  $form_state['conf']['date_format'] = $form_state['values']['date_format'];
}
