<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 */

/**
 * Implements THEME_preprocess_node().
 */
function katturai_preprocess_node(&$variables) {
  $node = $variables['node'];
  $variables['user_picture'] = FALSE;
  $variables['submitted'] = FALSE;
  $variables['content']['field_tags']['#title'] = FALSE;
  $variables['content']['links']['comment'] = FALSE;
  $variables['content']['field_tags']['#theme'] = 'links';
  unset($variables['content']['links']['node']['#links']['node-readmore']);
  $variables['newreadmore'] = l(t('Read More'), 'node/' . $node->nid);
  $variables['date_day'] = format_date($node->created, 'custom', 'j');
  $variables['date_month'] = format_date($node->created, 'custom', 'F');
  $variables['date_year'] = format_date($node->created, 'custom', 'Y');
}

/**
 * Implements THEME_status_messages().
 */
function katturai_status_messages($variables) {
  $display = $variables['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
  );
  foreach (drupal_get_messages($display) as $type => $messages) {
    $output .= "<div class=\"messages container $type\">\n";
    if (!empty($status_heading[$type])) {
      $output .= '<h2 class="element-invisible">' . $status_heading[$type] . "</h2>\n";
    }
    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= $messages[0];
    }
    $output .= "</div>\n";
  }
  return $output;
}

/**
 * Implements template_preprocess_page().
 */
function katturai_preprocess_page(&$variables) {
  $variables['content']['links']['#links']['comment-reply']['#attributes']['class'] = array('btn btn-default');
}

/**
 * Implements theme_item_list().
 */
function katturai_item_list($vars) {
  if (isset($vars['attributes']['class']) &&
    is_array($vars['attributes']['class']) &&
    in_array('pager', $vars['attributes']['class'])) {
    // Adjust pager output.
    $vars['attributes']['class'] = array('pagination pagination-sm');
    foreach ($vars['items'] as &$item) {
      if (in_array('pager-current', $item['class'])) {
        $item['class'] = array('active');
        $item['data'] = '<span>' . $item['data'] . '</span>';
      }
      elseif (in_array('pager-ellipsis', $item['class'])) {
        $item['class'] = array('disabled');
        $item['data'] = '<span>' . $item['data'] . '</span>';
      }
    }
    return '<div class="custom-pagination">' . theme_item_list($vars) . '</div>';
  }
  return theme_item_list($vars);
}

function katturai_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    unset($form['search_block_form']['#theme_wrappers']);
    $form['actions']['submit']['#attributes']['class'] = array('btn btn-default');
    $form['search_block_form']['#attributes']['class'] = array('form-control');
    $form['search_block_form']['#size'] = FALSE;
    $form['#attributes']['class'] = array('form-inline');
  }
} 

/**
 * Preprocesses variables for page.tpl.php.
 */
function katturai_preprocess_html(&$variables) {
  _katturai_load_bootstrap();
}

/**
 * Loads Twitter Bootstrap library.
 */
function _katturai_load_bootstrap() {
  $version = check_plain(theme_get_setting('bootstrap_version'));
  $js_path = '/js/bootstrap.min.js';
  $js_options = array(
    'group' => JS_LIBRARY,
  );
  $css_path = '/css/bootstrap.min.css';
  $cssr_path = '/css/bootstrap-responsive.min.css';
  $css_options = array(
    'group' => CSS_THEME,
    'weight' => -1000,
    'every_page' => TRUE,
  );
  switch (theme_get_setting('bootstrap_source')) {
    case 'bootstrapcdn':
      $bootstrap_path = '//netdna.bootstrapcdn.com/bootstrap/' . $version;
      $js_options['type'] = 'external';
      $css_path = '/css/bootstrap.min.css';
      unset($cssr_path);
      $css_options['type'] = 'external';
      break;

    case 'libraries':
      if (module_exists('libraries')) {
        $bootstrap_path = libraries_get_path('bootstrap');
      }
      break;

    case 'theme';
      $bootstrap_path = path_to_theme() . '/libraries/bootstrap';
      break;

    default:
      return;
  }
  _katturai_add_asset('js', $bootstrap_path . $js_path, $js_options);
  _katturai_add_asset('css', $bootstrap_path . $css_path, $css_options);
  if (isset($cssr_path)) {
    _katturai_add_asset('css', $bootstrap_path . $cssr_path, $css_options);
  }
}

/**
 * Adds js/css file.
 */
function _katturai_add_asset($type, $data, $options) {
  if (isset($options['browsers']) && !is_array($options['browsers'])) {
    $options['browsers'] = _katturai_browsers_to_array($options['browsers']);
  }
  switch ($type) {
    case 'css':
      drupal_add_css($data, $options);
      break;

    case 'js':
      if (isset($options['browsers'])) {
        $data = file_create_url($data);
        $elements = array(
          '#markup' => '<script type="text/javascript" src="' . $data . '"></script>',
          '#browsers' => $options['browsers'],
        );
        $elements = drupal_pre_render_conditional_comments($elements);
        _katturai_add_html_head_bottom(drupal_render($elements));
      }
      else {
        drupal_add_js($data, $options);
      }
      break;
  }
}

/**
 * Converts string representation of browsers to the array.
 */
function _katturai_browsers_to_array($browsers) {
  switch ($browsers) {
    case 'modern':
      return array('IE' => 'gte IE 9', '!IE' => TRUE);

    case 'obsolete':
      return array('IE' => 'lt IE 9', '!IE' => FALSE);
  }
  return array('IE' => TRUE, '!IE' => TRUE);
}

/**
 * Allows to add an extra html markup to the bottom of <head>.
 */
function _katturai_add_html_head_bottom($data = NULL) {
  $head_bottom = &drupal_static(__FUNCTION__);
  if (!isset($head_bottom)) {
    $head_bottom = '';
  }

  if (isset($data)) {
    $head_bottom .= $data;
  }
  return $head_bottom;
}

/**
 * Implements hook_page_alter().
 */
function katturai_page_alter($page) {
  $viewport = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1, maximum-scale=1',
    ),
  );
  drupal_add_html_head($viewport, 'viewport');
}

/**
 * Implements THEMENAME_form_comment_form_alter().
 */
function katturai_form_comment_form_alter(&$form, &$form_state) {
  $form['author']['name']['#attributes']['class'] = array('form-control');
  $form['subject']['#attributes']['class'] = array('form-control');
  $form['comment_body']['und'][0]['#attributes']['class'] = array('form-control');
  $form['actions']['submit']['#attributes']['class'] = array('btn btn-default');
  $form['actions']['preview']['#attributes']['class'] = array('btn btn-default');
}

/**
 * Theme override.
 * 
 * Add custom class to image styles.
 */
function katturai_image_style($variables) {

  // Determine the dimensions of the styled image.
  $dimensions = array(
    'width' => $variables['width'],
    'height' => $variables['height'],
  );

  image_style_transform_dimensions($variables['style_name'], $dimensions);

  $variables['width'] = $dimensions['width'];
  $variables['height'] = $dimensions['height'];

  // Determine the URL for the styled image.
  $variables['path'] = image_style_url($variables['style_name'], $variables['path']);

  // Begin custom snippet.
  // Add or append custom classes, to avoid clobbering existing.
  if (isset($variables['attributes']['class'])) {
    $variables['attributes']['class'] += array('img-responsive','img-thumbnail', $variables['style_name']);
  }
  else {
    $variables['attributes']['class'] = array('img-responsive','img-thumbnail', $variables['style_name']);
  }

  /* End custom snippet */
  return theme('image', $variables);
}

/**
 * Implements THEME_preprocess_region().
 */
function katturai_preprocess_region(&$variables, $hook) {
    if($variables['region'] == "header"){
        $variables['classes_array'][] = 'col-md-6';
    }
}

/**
 * Implements THEME_preprocess_comment().
 */
function katturai_preprocess_comment(&$variables) {
  $variables['content']['links']['comment']['#links']['comment-delete']['attributes']['class'] = 'btn btn-xs btn-danger';
  $variables['content']['links']['comment']['#links']['comment-edit']['attributes']['class'] = 'btn btn-xs btn-info';
  $variables['content']['links']['comment']['#links']['comment-reply']['attributes']['class'] = 'btn btn-xs btn-success';
}
