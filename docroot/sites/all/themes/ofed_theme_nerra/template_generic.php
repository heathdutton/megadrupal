<?php

/**
 * @file
 * Needs to be documented.
 */

/**
 * Preprocess variables for page.tpl.php.
 */
function nerra_preprocess_page(&$variables) {
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['class_left'] = theme_get_setting('nerra_layout_width_left');
    $variables['class_right'] = theme_get_setting('nerra_layout_width_right');
    $variables['class_content'] = theme_get_setting('nerra_layout_width_center');
  }
  elseif (!empty($variables['page']['sidebar_first'])) {
    $variables['class_left'] = theme_get_setting('nerra_layout_width_left');
    $variables['class_content'] = 'span-' . (12 - substr($variables['class_left'], 5, strlen($variables['class_left'])));
  }
  elseif (!empty($variables['page']['sidebar_second'])) {
    $variables['class_right'] = theme_get_setting('nerra_layout_width_right');
    $variables['class_content'] = 'span-' . (12 - substr($variables['class_right'], 5, strlen($variables['class_right'])));
  }
  else {
    $variables['class_content'] = 'span-12';
  }
}

/**
 * Add body classes if certain regions have content.
 */
function nerra_preprocess_html(&$variables) {
  foreach ($variables['classes_array'] as $key => $class) {
    if ($class == 'no-sidebars') {
      unset($variables['classes_array'][$key]);
    }
  }

  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['classes_array'][] = 'two-sidebars sidebar_first sidebar_second';
  }
  elseif (!empty($variables['page']['sidebar_first'])) {
    $variables['classes_array'][] = 'one-sidebar sidebar_first';
  }
  elseif (!empty($variables['page']['sidebar_second'])) {
    $variables['classes_array'][] = 'one-sidebar sidebar_second';
  }
  else {
    $variables['classes_array'][] = 'no-sidebars';
  }

  // Add conditional stylesheets for IE.
  drupal_add_css(
    path_to_theme() . '/css/ie.css',
    array(
      'group' => CSS_THEME,
      'browsers' => array(
        'IE' => 'lte IE 7',
        '!IE' => FALSE,
      ),
      'preprocess' => FALSE,
    )
  );
}

/**
 * Replace the meta content-type tag for Drupal 7.
 */
function nerra_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array('charset' => 'utf-8');
}

/**
 * Remove unnecessary CSS file.
 */
function nerra_css_alter(&$css) {
  unset($css[drupal_get_path('module', 'text_resize') . '/text_resize.css']);
  unset($css[drupal_get_path('module', 'locale') . '/locale.css']);
  // Turn off some styles from the system module.
  if (!user_is_logged_in()) {
    unset($css[drupal_get_path('module', 'system') . '/system.base.css']);
    unset($css[drupal_get_path('module', 'system') . '/system.menus.css']);
    unset($css[drupal_get_path('module', 'system') . '/system.theme.css']);
  }
}

/**
 * Returns HTML for a breadcrumb trail.
 *
 * @param array $variables
 *   An associative array containing:
 *   - breadcrumb: An array containing the breadcrumb links.
 */
function nerra_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    $first = urlencode('<firstchild>');
    $nolink = urlencode('<nolink>');

    $display = '<ul>';

    if (theme_get_setting('nerra_breadcrumb_label')) {
      $display .= '<li>' . t('You are here') . '</li>';
    }

    $counter = 1;
    $count = count($breadcrumb);
    foreach ($breadcrumb as $item) {
      if (strpos($item, $first) || strpos($item, $nolink)) {
        $item = strip_tags($item);
      }
      switch ($counter) {
        case '1':
          $class = 'class="first"';
          break;

        case $count:
          $class = 'class="last"';
          break;

        default:
          $class = '';
          break;
      }
      $display .= '<li ' . $class . ' >' . $item . '</li>';
      $counter++;
    }

    $display .= '</ul>';
    return $display;
  }
}

/**
 * Return a string limit by number of word and add a "read more" if necessary.
 *
 * @param string $string
 *   Text to show.
 * @param numeric $length
 *   Length to render.
 * @param string $more_link
 *   Can be a path for the more link.
 *
 * @return string
 *   Needs to be documented.
 */
function nerra_cut_string($string, $length = 150, $more_link = '') {
  ($more_link != '') ? $more = '<br />' . l(t('Read more'), $more_link) . '' : $more = '';

  if (empty ($string)) {
    return "";
  }
  elseif (strlen($string) < $length) {
    return $string;
  }
  elseif (preg_match("/(.{1,$length})\s./ms", $string, $match)) {
    return $match[1] . ' ...' . $more;
  }
  else {
    return substr($string, 0, $length) . ' ...' . $more;
  }
}

/**
 * Override or insert variables into the node template.
 */
function nerra_preprocess_node(&$variables) {
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
    $variables['node']->which_display = '';
  }

  if (isset($variables['node']->view->current_display)) {
    $which_display = $variables['node']->view->name . '-' . $variables['node']->view->current_display;
    $variables['node']->which_display = $which_display;
  }

  if (module_exists('metatag')) {
    render($variables['content']['metatags']);
  }
}

/**
 * Return class name for checkbox input.
 * @return string
 *   Needs to be documented.
 */
function nerra_getThemeableFormCheckboxClass() {
  return ' ' . theme_get_setting('nerra_form_checkbox');
}

/**
 * Return class name for file input.
 * @return string
 *   Needs to be documented.
 */
function nerra_getThemeableFormFileClass() {
  return ' ' . theme_get_setting('nerra_form_file');
}

/**
 * Return class name for password input.
 * @return string
 *   Needs to be documented.
 */
function nerra_getThemeableFormPasswordClass() {
  return ' ' . theme_get_setting('nerra_form_password');
}

/**
 * Return class name for radio input.
 * @return string
 *   Needs to be documented.
 */
function nerra_getThemeableFormRadioClass() {
  return ' ' . theme_get_setting('nerra_form_radio');
}

/**
 * Return class name for select input.
 * @return string
 *   Needs to be documented.
 */
function nerra_getThemeableFormSelectSingleClass() {
  return ' ' . theme_get_setting('nerra_form_select_single');
}

/**
 * Return class name for multi select input.
 * @return string
 *   Needs to be documented.
 */
function nerra_getThemeableFormSelectMultipleClass() {
  return ' ' . theme_get_setting('nerra_form_select_multiple');
}

/**
 * Return class name for textarea input.
 * @return string
 *   Needs to be documented.
 */
function nerra_getThemeableFormTextClass() {
  return ' ' . theme_get_setting('nerra_form_text');
}

/**
 * Return class name for date input.
 * @return string
 *   Needs to be documented.
 */
function nerra_getThemeableFormDateClass() {
  return ' ' . theme_get_setting('nerra_form_date');
}

/**
 * Returns HTML for a checkbox form element.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #return_value, #description, #required,
 *     #attributes.
 *
 * @ingroup themeable
 */
function nerra_checkbox($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'checkbox';
  element_set_attributes($element, array(
    '#id' => 'id',
    '#name' => 'name',
    '#return_value' => 'value',
  ));
  // Unchecked checkbox has #value of integer 0.
  if (!empty($element['#checked'])) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-checkbox', nerra_getThemeableFormCheckboxClass()));
  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Returns HTML for a set of checkbox form elements.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #children, #attributes.
 *
 * @ingroup themeable
 */
function nerra_checkboxes($variables) {
  $element = $variables['element'];
  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  $attributes['class'][] = 'form-checkboxes';
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = array_merge($attributes['class'], $element['#attributes']['class']);
  }
  return '<div' . drupal_attributes($attributes) . '>' . (!empty($element['#children']) ? $element['#children'] : '') . '</div>';
}

/**
 * Returns HTML for a file upload form element.
 *
 * For assistance with handling the uploaded file correctly, see the API
 * provided by file.inc.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #name, #size, #description, #required,
 *     #attributes.
 *
 * @ingroup themeable
 */
function nerra_file($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'file';
  element_set_attributes($element, array('id', 'name', 'size'));
  _form_set_class($element, array('form-file', nerra_getThemeableFormFileClass()));

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Returns HTML for a link to a file.
 *
 * @param array $variables
 *   An associative array containing:
 *   - file: A file object to which the link will be created.
 *   - icon_directory: (optional) A path to a directory of icons to be used for
 *     files. Defaults to the value of the "file_icon_directory" variable.
 *
 * @ingroup themeable
 */
function nerra_file_link($variables) {
  $file = $variables['file'];

  $icon_directory = drupal_get_path('theme', 'nerra') . '/assets/icons';

  $url = file_create_url($file->uri);
  $icon = theme('file_icon', array('file' => $file, 'icon_directory' => $icon_directory));

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  $options = array(
    'attributes' => array(
      'type' => $file->filemime . '; length=' . $file->filesize,
      'target' => '_blank',
    ),
  );

  // Use the description as the link text if available.
  if (empty($file->description)) {
    $link_text = $file->filename;
  }
  else {
    $link_text = $file->description;
    $options['attributes']['title'] = check_plain($file->filename);
  }

  return '<span class="file">' . $icon . ' ' . l($link_text, $url, $options) . '</span>';
}

/**
 * Returns HTML for a password form element.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #description, #size, #maxlength,
 *     #required, #attributes.
 *
 * @ingroup themeable
 */
function nerra_password($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'password';
  element_set_attributes($element, array(
    'id',
    'name',
    'size',
    'maxlength,
   '));
  _form_set_class($element, array('form-text', nerra_getThemeableFormPasswordClass()));

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Returns HTML for a radio button form element.
 *
 * Note: The input "name" attribute needs to be sanitized before output, which
 *       is currently done by passing all attributes to drupal_attributes().
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #required, #return_value, #value, #attributes, #title,
 *     #description
 *
 * @ingroup themeable
 */
function nerra_radio($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'radio';
  element_set_attributes($element, array(
    '#id' => 'id',
    '#name' => 'name',
    '#return_value' => 'value',
  ));

  if (isset($element['#return_value']) && $element['#value'] !== FALSE && $element['#value'] == $element['#return_value']) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-radio', nerra_getThemeableFormRadioClass()));

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Returns HTML for a select form element.
 *
 * It is possible to group options together; to do this, change the format of
 * $options to an associative array in which the keys are group labels, and the
 * values are associative arrays in the normal $options format.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #options, #description, #extra,
 *     #multiple, #required, #name, #attributes, #size.
 *
 * @ingroup themeable
 */
function nerra_select($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));

  if ($element['#multiple'] == 1) {
    _form_set_class($element, array('form-select', nerra_getThemeableFormSelectMultipleClass()));
  }
  else {
    _form_set_class($element, array('form-select', nerra_getThemeableFormSelectSingleClass()));
  }

  return '<select' . drupal_attributes($element['#attributes']) . '>' . form_select_options($element) . '</select>';
}

/**
 * Returns HTML for a textfield form element.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #description, #size, #maxlength,
 *     #required, #attributes, #autocomplete_path.
 *
 * @ingroup themeable
 */
function nerra_textfield($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'text';
  element_set_attributes($element, array(
    'id',
    'name',
    'value',
    'size',
    'maxlength',
   ));
  _form_set_class($element, array('form-text', nerra_getThemeableFormTextClass()));

  $extra = '';
  if ($element['#autocomplete_path'] && drupal_valid_path($element['#autocomplete_path'])) {
    drupal_add_library('system', 'drupal.autocomplete');
    $element['#attributes']['class'][] = 'form-autocomplete';

    $attributes = array();
    $attributes['type'] = 'hidden';
    $attributes['id'] = $element['#attributes']['id'] . '-autocomplete';
    $attributes['value'] = url($element['#autocomplete_path'], array('absolute' => TRUE));
    $attributes['disabled'] = 'disabled';
    $attributes['class'][] = 'autocomplete';
    $extra = '<input' . drupal_attributes($attributes) . ' />';
  }

  $output = '<input' . drupal_attributes($element['#attributes']) . ' />';

  return $output . $extra;
}

/**
 * Returns HTML for a date selection form element.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #options, #description, #required,
 *     #attributes.
 *
 * @ingroup themeable
 */
function nerra_date($variables) {
  $element = $variables['element'];

  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'container-inline';
  $attributes['class'][] = nerra_getThemeableFormDateClass();

  return '<div' . drupal_attributes($attributes) . '>' . drupal_render_children($element) . '</div>';
}

/**
 * Returns HTML for a button form element.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #attributes, #button_type, #name, #value.
 *
 * @ingroup themeable
 */
function nerra_button($variables) {
  global $language;

  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  $element['#attributes']['class'][] = 'custom-btn';

  $value_source_query = db_query("SELECT ls.source FROM {locales_target} lt INNER JOIN {locales_source} ls ON ls.lid = lt.lid WHERE lt.translation = :value AND lt.language = :lang LIMIT 1", array(':value' => $element['#value'], ':lang' => $language->language));
  $value_source = $value_source_query->fetchObject();
  if ($value_source) {
    $class = str_replace(' ', '_', $value_source->source);
    $element['#attributes']['class'][] = 'custom-btn-' . strtolower($class);
  }
  else {
    $class = str_replace(' ', '_', $element['#value']);
    $element['#attributes']['class'][] = 'custom-btn-' . strtolower($class);
  }
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Returns HTML for a list or nested list of items.
 *
 * @param array $variables
 *   An associative array containing:
 *   - items: An array of items to be displayed in the list. If an item is a
 *     string, then it is used as is. If an item is an array, then the "data"
 *     element of the array is used as the contents of the list item. If an item
 *     is an array with a "children" element, those children are displayed in a
 *     nested list. All other elements are treated as attributes of the list
 *     item element.
 *   - title: The title of the list.
 *   - type: The type of list to return (e.g. "ul", "ol").
 *   - attributes: The attributes applied to the list element.
 */
function nerra_item_list($variables) {
  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $attributes = $variables['attributes'];

  // Only output the list container and title, if there are any list items.
  // Check to see whether the block title exists before adding a header.
  // Empty headers are not semantic and present accessibility challenges.
  // $output = '<div class="item-list">';
  $output = '';
  if (isset($title) && $title !== '') {
    $output .= '<h3>' . $title . '</h3>';
  }

  if (!empty($items)) {
    $output .= "<$type" . drupal_attributes($attributes) . '>';
    $num_items = count($items);
    foreach ($items as $i => $item) {
      $attributes = array();
      $children = array();
      $data = '';
      if (is_array($item)) {
        foreach ($item as $key => $value) {
          if ($key == 'data') {
            $data = $value;
          }
          elseif ($key == 'children') {
            $children = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $data = $item;
      }
      if (count($children) > 0) {
        // Render nested list.
        $data .= theme_item_list(array(
          'items' => $children,
          'title' => NULL,
          'type' => $type,
          'attributes' => $attributes,
        ));
      }
      if ($i == 0) {
        $attributes['class'][] = 'first';
      }
      if ($i == $num_items - 1) {
        $attributes['class'][] = 'last';
      }
      $output .= '<li' . drupal_attributes($attributes) . '>' . $data . "</li>\n";
    }
    $output .= "</$type>";
  }
  $output .= '';
  return $output;
}

/**
 * Returns HTML for a feed icon.
 *
 * @param array $variables
 *   An associative array containing:
 *   - url: An internal system path or a fully qualified external URL of the
 *     feed.
 *   - title: A descriptive title of the feed.
 */
function nerra_feed_icon($variables) {
  $text = t('Subscribe to @feed-title', array('@feed-title' => $variables['title']));
  $image = theme('image', array(
    'path' => drupal_get_path('theme', 'nerra') . '/assets/icons/icon-rss.png',
    'width' => 16,
    'height' => 16,
    'alt' => $text,
  ));
  if ($image) {
    return l($image, $variables['url'], array(
      'html' => TRUE,
      'attributes' => array(
        'class' => array(
          'feed-icon',
        ),
        'title' => $text,
      ),
    ));
  }
}
