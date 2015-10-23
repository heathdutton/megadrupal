<?php
/**
 * @file
 * CMS theme template file
 */

/**
 * Add a js to the exposed form.
 */
function cms_theme_preprocess_views_exposed_form(&$vars) {
  drupal_add_js(drupal_get_path('theme', $GLOBALS['theme_key']) . "/assets/scripts/cms-views.js", "theme");
}


/**
 * Preprocess variables for page.tpl.php.
 */
function cms_theme_preprocess_page(&$variables) {
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['class_left'] = theme_get_setting('cms_theme_layout_width_left');
    $variables['class_right'] = theme_get_setting('cms_theme_layout_width_right');
    $variables['class_content'] = theme_get_setting('cms_theme_layout_width_center');

  }
  elseif (!empty($variables['page']['sidebar_first'])) {
    $variables['class_left'] = theme_get_setting('cms_theme_layout_width_left');
    $variables['class_content'] = 'span-' . (12 - substr($variables['class_left'], 5, strlen($variables['class_left'])));
  }
  elseif (!empty($variables['page']['sidebar_second'])) {
    $variables['class_right'] = theme_get_setting('cms_theme_layout_width_right');
    $variables['class_content'] = 'span-' . (12 - substr($variables['class_right'], 5, strlen($variables['class_right'])));
  }
  else {
    $variables['class_content'] = 'span-12';
  }

  $variables['logo_url'] = '<front>';
  $variables['site_name'] .= '';

  if (module_exists('ofed_switcher')) {
    $variables['logo_url'] = 'cms/startpage';
    $variables['site_name'] .= ' : ' . t('Start page');
  }
}


/**
 * Add body classes if certain regions have content.
 */
function cms_theme_preprocess_html(&$variables) {
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
function cms_theme_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array('charset' => 'utf-8');
}

/**
 * Remove unnecessary CSS file.
 */
function cms_theme_css_alter(&$css) {
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
function cms_theme_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    $first = urlencode('<firstchild>');
    $nolink = urlencode('<nolink>');

    $display = '<ul>';

    if (theme_get_setting('cms_theme_breadcrumb_label')):
      $display .= '<li>' . t('You are here') . '</li>';
    endif;

    $counter = 1;
    $count = count($breadcrumb);
    foreach ($breadcrumb as $item) {
      if (strpos($item, $first) || strpos($item, $nolink)) {
        $item = strip_tags($item);
      }
      switch ($counter) {
        case "1":
          $class = 'class="first"';
          break;

        case $count:
          $class = 'class="last"';
          break;

        default:
          $class = '';
      }
      $display .= '<li ' . $class . ' >' . $item . '</li>';
      $counter++;
    }

    $display .= '</ul>';
    return $display;
  }
}

/**
 * Replace the breadcrumb root link to the CMS' startpage instead of the home.
 */
function cms_theme_block_view_easy_breadcrumb_easy_breadcrumb_alter(&$data, $block) {
  $arg_0 = arg(0);

  // Replace HOME by START PAGE.
  if ($arg_0 == 'admin' || $arg_0 == 'cms' || path_is_admin(current_path())) {
    $breadcrumb = $data['content']['easy_breadcrumb']['#breadcrumb'];
    $breadcrumb[0]['content'] = t('Start Page');
    $breadcrumb[0]['url'] = 'cms/startpage';

    if ($arg_0 == 'cms') {
      $arg_1 = arg(1);
      // Save 'START PAGE' for later.
      $start = array_shift($breadcrumb);
      // Remove 'CMS' from breadcrumb.
      array_shift($breadcrumb);

      if (!empty($arg_1) && $arg_1 == 'startpage') {
        // Remove trailing 'START PAGE' from breadcrumb.
        array_shift($breadcrumb);
      }
      array_unshift($breadcrumb, $start);
    }
    else if ($arg_0 == 'node') {
      $arg_1 = arg(1);

      if (!empty($arg_1) && $arg_1 == 'add') {
        // Save 'START PAGE' for later.
        $start = array_shift($breadcrumb);
        // Remove 'node' from breadcrumb.
        array_shift($breadcrumb);
        array_unshift($breadcrumb, $start);
      }
    }
    $data['content']['easy_breadcrumb']['#breadcrumb'] = $breadcrumb;
    $data['content']['easy_breadcrumb']['#segments_quantity'] = count($breadcrumb);
  }
}


/**
 * Return a string limit by number of word and add a "read more" if necessary.
 *
 * @param string $string
 *   Text to show
 * @param numeric $length
 *   Length to render
 * @param string $more_link
 *   Can be a path for the more link
 *
 * @return string
 *   A shortened string to the appropriate length
 */
function cms_theme_cut_string($string, $length = 150, $more_link = '') {
  $more = ($more_link != '') ? '<br />' . l(t('Read more'), $more_link) . '' : '';

  if (empty($string)) {
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
function cms_theme_preprocess_node(&$variables) {
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
 */
function cms_theme_getThemeableFormCheckboxClass() {
  return ' ' . theme_get_setting('cms_theme_form_checkbox');
}


/**
 * Return class name for file input.
 */
function cms_theme_getThemeableFormFileClass() {
  return ' ' . theme_get_setting('cms_theme_form_file');
}


/**
 * Return class name for password input.
 */
function cms_theme_getThemeableFormPasswordClass() {
  return ' ' . theme_get_setting('cms_theme_form_password');
}
/**
 * Return class name for radio input.
 */
function cms_theme_getThemeableFormRadioClass() {
  return ' ' . theme_get_setting('cms_theme_form_radio');
}


/**
 * Return class name for select input.
 */
function cms_theme_getThemeableFormSelectSingleClass() {
  return ' ' . theme_get_setting('cms_theme_form_select_single');
}


/**
 * Return class name for multi select input.
 */
function cms_theme_getThemeableFormSelectMultipleClass() {
  return ' ' . theme_get_setting('cms_theme_form_select_multiple');
}


/**
 * Return class name for textarea input.
 */
function cms_theme_getThemeableFormTextClass() {
  return ' ' . theme_get_setting('cms_theme_form_text');
}


/**
 * Return class name for date input.
 */
function cms_theme_getThemeableFormDateClass() {
  return ' ' . theme_get_setting('cms_theme_form_date');
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
function cms_theme_checkbox($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'checkbox';
  $attributes = array('id', 'name', '#return_value' => 'value');
  element_set_attributes($element, $attributes);

  // Unchecked checkbox has #value of integer 0.
  if (!empty($element['#checked'])) {
    $element['#attributes']['checked'] = 'checked';
  }
  $class = array('form-checkbox', cms_theme_getThemeableFormCheckboxClass());
  _form_set_class($element, $class);

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
function cms_theme_checkboxes($variables) {
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
function cms_theme_file($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'file';
  element_set_attributes($element, array('id', 'name', 'size'));
  $class = array('form-file', cms_theme_getThemeableFormFileClass());
  _form_set_class($element, $class);

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
function cms_theme_file_link($variables) {
  $file = $variables['file'];
  $icon_directory = $variables['icon_directory'];

  $url = file_create_url($file->uri);
  $icon = theme('file_icon', array('file' => $file, 'icon_directory' => $icon_directory));

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples.
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
function cms_theme_password($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'password';
  element_set_attributes($element, array('id', 'name', 'size', 'maxlength'));
  $class = array('form-text', cms_theme_getThemeableFormPasswordClass());
  _form_set_class($element, $class);

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
function cms_theme_radio($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'radio';
  $attributes = array('id', 'name', '#return_value' => 'value');
  element_set_attributes($element, $attributes);

  if (isset($element['#return_value']) && $element['#value'] !== FALSE && $element['#value'] == $element['#return_value']) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-radio', cms_theme_getThemeableFormRadioClass()));

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
function cms_theme_select($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));

  if ($element['#multiple'] == 1) {
    $class = array(
      'form-select',
      cms_theme_getThemeableFormSelectMultipleClass(),
    );
    _form_set_class($element, $class);
  }
  else {
    $class = array(
      'form-select',
      cms_theme_getThemeableFormSelectSingleClass(),
    );
    _form_set_class($element, $class);
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
function cms_theme_textfield($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'text';
  $attributes = array('id', 'name', 'value', 'size', 'maxlength');
  element_set_attributes($element, $attributes);
  $class = array('form-text', cms_theme_getThemeableFormTextClass());
  _form_set_class($element, $class);

  $extra = '';
  if ($element['#autocomplete_path'] && drupal_valid_path($element['#autocomplete_path'])) {
    drupal_add_library('system', 'drupal.autocomplete');
    $element['#attributes']['class'][] = 'form-autocomplete';

    $attributes = array();
    $attributes['type'] = 'hidden';
    $attributes['id'] = $element['#attributes']['id'] . '-autocomplete';
    $options = array('absolute' => TRUE);
    $attributes['value'] = url($element['#autocomplete_path'], $options);
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
function cms_theme_date($variables) {
  $element = $variables['element'];

  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'container-inline';
  $attributes['class'][] = cms_theme_getThemeableFormDateClass();

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
function cms_theme_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  $element['#attributes']['class'][] = 'custom-btn';
  $class = str_replace(' ', '_', $element['#value']);
  $element['#attributes']['class'][] = 'custom-btn-' . strtolower($class);
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
function cms_theme_item_list($variables) {
  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $attributes = $variables['attributes'];

  // Only output the list container and title, if there are any list items.
  // Check to see whether the block title exists before adding a header.
  // Empty headers are not semantic and present accessibility challenges.
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
        $items = array(
          'items' => $children,
          'title' => NULL,
          'type' => $type,
          'attributes' => $attributes,
        );
        // Render nested list.
        $data .= theme_item_list($items);
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
function cms_theme_feed_icon($variables) {
  $substitution = array('@feed-title' => $variables['title']);
  $text = t('Subscribe to @feed-title', $substitution);
  $variables = array(
    'path' => drupal_get_path('theme', 'cms_theme') . '/assets/icons/icon-rss.png',
    'width' => 16,
    'height' => 16,
    'alt' => $text,
  );

  if ($image = theme('image', $variables)) {
    $options = array(
      'html' => TRUE,
      'attributes' => array(
        'class' => array('feed-icon'),
        'title' => $text,
      ),
    );
    return l($image, $variables['url'], $options);
  }
}

/**
 * Alters hook_field_extra_fields().
 *
 * Makes sure Language field appears above Title field by default in node forms.
 *
 * Default values before altering are respectively 0 (from i18n) and -5 (from
 * node module). After altering, Language will precede Title instead.
 */
function cms_theme_field_extra_fields_alter(&$extra) {
  if (!empty($extra['node'])) {
    foreach ($extra['node'] as $module => &$fields) {
      if (isset($fields['form']['language']['weight']) && isset($fields['form']['title']['weight']) && ($fields['form']['language']['weight'] > $fields['form']['title']['weight'])) {
        $fields['form']['language']['weight'] = $fields['form']['title']['weight'] - 1;
      }
    }
  }
}

/**
 * Implements hook_form_alter.
 *
 * @see cms_theme_node_validate_menu_link()
 */
function cms_theme_form_alter(&$form, &$form_state, $form_id) {
  // Add a new default value if this is the node add form or the node edit form
  // and the menu link option had not been checked.
  if (substr($form_id, -10) == '_node_form' && !empty($form['menu']['link']['parent']['#options']) && ($form['nid']['#value'] == NULL || $form['menu']['enabled']['#default_value'] == 0)) {
    // Add an empty value to the beginning of the options array.
    $form['menu']['link']['parent']['#options'] = array_reverse($form['menu']['link']['parent']['#options'], TRUE);
    $form['menu']['link']['parent']['#options'][''] = t('Select a parent item...');
    $form['menu']['link']['parent']['#options'] = array_reverse($form['menu']['link']['parent']['#options'], TRUE);
    $form['menu']['link']['parent']['#default_value'] = '';
    $form['#validate'][] = 'cms_theme_node_validate_menu_link';
  }
}

/**
 * Validate handler for default menu link option.
 *
 * @see cms_theme_form_alter()
 */
function cms_theme_node_validate_menu_link($form, &$form_state) {
  if ($form_state['values']['menu']['enabled'] == 1 && $form_state['values']['menu']['parent'] == '') {
    form_set_error('menu][parent', t('Please select a Parent item for the menu link.'));
  }
}
