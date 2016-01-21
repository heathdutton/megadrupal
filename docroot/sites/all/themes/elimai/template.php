<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 */

/**
 * Preprocesses variables for page.tpl.php.
 */
function elimai_preprocess_html(&$variables) {
  _elimai_load_bootstrap();
}

/**
 * Implements THEME_preprocess_node().
 */
function elimai_preprocess_node(&$variables) {
  $variables['user_picture'] = FALSE;
  $variables['submitted'] = FALSE;
  $variables['content']['field_tags']['#title'] = FALSE;
  $variables['content']['links']['comment'] = FALSE;

  $node = $variables['node'];

  $variables['date_day'] = format_date($node->created, 'custom', 'j');
  $variables['date_month'] = format_date($node->created, 'custom', 'F');
  $variables['date_year'] = format_date($node->created, 'custom', 'Y');

  $variables['content']['field_tags']['#theme'] = 'links';
  $variables['content']['field_image'][0]['#attributes']['class'][] = 'img-polaroid';
  // Let's get that read more link out of the generated links variable!
  unset($variables['content']['links']['node']['#links']['node-readmore']);

  // Now let's put it back as it's own variable! So it's actually versatile!
  $variables['newreadmore'] = '<footer>' . l(t('Read More'), 'node/' . $node->nid,
    array('attributes' => array('class' => array('btn btn-mini')))
  ) . '</footer>';
}

/**
 * Implements template_preprocess_page().
 */
function elimai_preprocess_page(&$variables) {

  // Social Block Settings.
  $variables['facebook_url'] = check_url(theme_get_setting('facebook_url', 'elimai'));
  $variables['twitter_url'] = check_url(theme_get_setting('twitter_url', 'elimai'));
  $variables['gplus_url'] = check_url(theme_get_setting('gplus_url', 'elimai'));
  $variables['pinterest_url'] = check_url(theme_get_setting('pinterest_url', 'elimai'));

  // Social Icons path.
  $variables['path'] = base_path() . path_to_theme('theme', 'elimai');

  // Carousel Settings.
  $carousel_enabled = theme_get_setting('carousel_enabled', 'elimai');
  if ($carousel_enabled) {
    $slide_count = check_plain(theme_get_setting('slide_count', 'elimai'));
    $variables['arrow_enabled'] = theme_get_setting('arrow_enabled', 'elimai');
    $variables['bullets_enabled'] = theme_get_setting('bullets_enabled', 'elimai');

    $carousel_items = array();
    // Range indexes start's with 0, hence the slide_count+1 in the for
    // condition.
    for ($index = 1; $index <= $slide_count + 1; $index++) {
      $carousel_image = theme_get_setting('carousel_image_' . $index, 'elimai');
      if (!empty($carousel_image)) {
        $carousel_image = file_load($carousel_image);

        $carousel_items[$index] = array(
          'carousel_image' => $carousel_image,
          'carousel_alt_text' => check_plain(strip_tags(theme_get_setting('carousel_alt_text_' . $index, 'elimai'))),
          'carousel_caption' => check_plain(strip_tags(theme_get_setting('slide_caption_' . $index, 'elimai'))),
        );
      }
    }
    $variables['carousel_items'] = $carousel_items;
    $variables['bootstrap_carousel'] = theme('bootstrap_carousel', $variables);
  }
}

/**
 * Theme override.
 * 
 * Add custom class to image styles.
 */
function elimai_image_style($variables) {

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
    $variables['attributes']['class'] += array('img-polaroid', $variables['style_name']);
  }
  else {
    $variables['attributes']['class'] = array('img-polaroid', $variables['style_name']);
  }

  /* End custom snippet */
  return theme('image', $variables);
}

/**
 * Implements THEME_form_alter().
 */
function elimai_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    $form['#attributes']['class'][] = 'form-search';
    $form['search_block_form']['#attributes']['class'][] = 'span3 search-query';
    $form['actions']['submit']['#attributes']['class'][] = 'btn';
  }

  if ($form_id == 'search_form') {
    $form['basic']['submit']['#attributes']['class'][] = 'btn';
    $form['advanced']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('value for advanced search button'),  /* and the last one :) */
      '#prefix' => '<div class="action">',
      '#suffix' => '</div>',
      '#weight' => 100,
      '#attributes' => array('class' => array('btn')),
    );
  }
}

/**
 * Implements theme_item_list().
 */
function elimai_item_list($vars) {
  if (isset($vars['attributes']['class']) &&
    is_array($vars['attributes']['class']) &&
    in_array('pager', $vars['attributes']['class'])) {
    // Adjust pager output.
    unset($vars['attributes']['class']);
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
    return '<div class="pagination pagination-centered">' . theme_item_list($vars) . '</div>';
  }
  return theme_item_list($vars);
}

/**
 * Implements theme_breadcrumb().
 */
function elimai_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  $delimiter = check_plain(theme_get_setting('breadcrumb_delimiter'));

  if (!empty($breadcrumb)) {
    // Use CSS to hide titile .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    // Comment below line to hide current page to breadcrumb.
    $breadcrumb[] = drupal_get_title();
    $output .= '<nav class="breadcrumb">' . implode($delimiter, $breadcrumb) . '</nav>';
    return $output;
  }
}

/**
 * Implements THEMENAME_links__system_secondary_menu().
 */
function elimai_links__system_secondary_menu($variables) {
  $links = $variables['links'];
  $attributes = $variables['attributes'];
  $heading = $variables['heading'];
  global $language_url;
  $output = '';

  if (count($links) > 0) {
    $output = '';

    // Treat the heading first if it is present to prepend it to the
    // list of links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading
        // is a string.
        $heading = array(
          'text' => $heading,

          // Set the default level of the heading.
          'level' => 'h2',
        );
      }
      $output .= '<' . $heading['level'];
      if (!empty($heading['class'])) {
        $output .= drupal_attributes(array('class' => $heading['class']));
      }
      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }

    $output .= '<ul' . drupal_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = array($key);

      // Add first, last and active classes to the list of links to
      // help out themers.
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page())) && (empty($link['language']) || $link['language']->language == $language_url->language)) {
        $class[] = 'active';
      }
      $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        $output .= l($link['title'], $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span>
        // for adding title and class attributes.
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }

      $i++;
      $output .= "<span class='menu-divider'>/</span></li>\n";
    }

    $output .= '</ul>';
  }

  return $output;
}

/**
 * Implements THEMENAME_links__system_main_menu().
 */
function elimai_links__system_main_menu($variables) {
  $links = $variables['links'];
  $attributes = $variables['attributes'];
  $heading = $variables['heading'];
  global $language_url;
  $output = '';

  if (count($links) > 0) {
    $output = '';

    // Treat the heading first if it is present to prepend it to the
    // list of links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading
        // is a string.
        $heading = array(
          'text' => $heading,

          // Set the default level of the heading.
          'level' => 'h2',
        );
      }
      $output .= '<' . $heading['level'];
      if (!empty($heading['class'])) {
        $output .= drupal_attributes(array('class' => $heading['class']));
      }
      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }

    $output .= '<ul' . drupal_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = array($key);

      // Add first, last and active classes to the list of links to help out
      // themers.
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page())) && (empty($link['language']) || $link['language']->language == $language_url->language)) {
        $class[] = 'active';
      }
      $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        $output .= l($link['title'], $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span>
        // for adding title and class attributes.
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }

      $i++;
      $output .= "<span class='menu-divider'>/</span></li>\n";
    }

    $output .= '</ul>';
  }

  return $output;
}

/**
 * Loads Twitter Bootstrap library.
 */
function _elimai_load_bootstrap() {
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
      $bootstrap_path = '//netdna.bootstrapcdn.com/twitter-bootstrap/' . $version;
      $js_options['type'] = 'external';
      $css_path = '/css/bootstrap-combined.min.css';
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
  _elimai_add_asset('js', $bootstrap_path . $js_path, $js_options);
  _elimai_add_asset('css', $bootstrap_path . $css_path, $css_options);
  if (isset($cssr_path)) {
    _elimai_add_asset('css', $bootstrap_path . $cssr_path, $css_options);
  }
}

/**
 * Adds js/css file.
 */
function _elimai_add_asset($type, $data, $options) {
  if (isset($options['browsers']) && !is_array($options['browsers'])) {
    $options['browsers'] = _elimai_browsers_to_array($options['browsers']);
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
        _elimai_add_html_head_bottom(drupal_render($elements));
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
function _elimai_browsers_to_array($browsers) {
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
function _elimai_add_html_head_bottom($data = NULL) {
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
function elimai_page_alter($page) {
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
 * Implements hook_theme().
 */
function elimai_theme() {
  return array(
    'bootstrap_carousel' => array(
      'variables' => array(
        'item' => NULL,
      ),
      'template' => 'templates/bootstrap_carousel',
    ),
  );
}
