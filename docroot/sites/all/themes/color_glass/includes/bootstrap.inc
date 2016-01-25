<?php
/**
 * @file
 * Adds Bootstrap markup and removes some default markup cruft.
 */

// Include all require files.
require_once __DIR__ . '/helper.inc';

/**
 * Overrides theme_breadcrumb().
 */
function color_glass_breadcrumb($variables) {
  $output = '';
  $breadcrumb = $variables['breadcrumb'];

  // Determine if we are to display the breadcrumb.
  $bootstrap_breadcrumb = _bootstrap_setting('breadcrumb');
  if (($bootstrap_breadcrumb == 1 || ($bootstrap_breadcrumb == 2 && arg(0) == 'admin')) && !empty($breadcrumb)) {
    $output = theme('item_list', array(
      'attributes' => array(
        'class' => array('breadcrumb'),
      ),
      'items' => $breadcrumb,
      'type' => 'ol',
    ));
  }
  return $output;
}

/**
 * Implements hook_preprocess_breadcrumb().
 */
function color_glass_preprocess_breadcrumb(&$variables) {
  $breadcrumb = &$variables['breadcrumb'];

  // Optionally get rid of the homepage link.
  $show_breadcrumb_home = _bootstrap_setting('breadcrumb_home');
  if (!$show_breadcrumb_home) {
    array_shift($breadcrumb);
  }

  if (_bootstrap_setting('breadcrumb_title') && !empty($breadcrumb)) {
    $item = menu_get_item();

    $page_title = !empty($item['tab_parent']) ? check_plain($item['title']) : drupal_get_title();
    if (!empty($page_title)) {
      $breadcrumb[] = array(
        // If we are on a non-default tab, use the tab's title.
        'data' => $page_title,
        'class' => array('active'),
      );
    }
  }
}

/**
 * Implements hook_preprocess_block().
 */
function color_glass_preprocess_block(&$variables) {
  // Use a bare template for the page's main content.
  if ($variables['block_html_id'] == 'block-system-main') {
    $variables['theme_hook_suggestions'][] = 'block__no_wrapper';
  }
  $variables['title_attributes_array']['class'][] = 'block-title';
}

/**
 * Implements hook_process_block().
 */
function color_glass_process_block(&$variables) {
  // Drupal 7 should use a $title variable instead of $block->subject.
  $variables['title'] = $variables['block']->subject;
}

/**
 * Overrides theme_menu_tree().
 */
function color_glass_menu_tree(&$variables) {
  return '<ul class="menu nav">' . $variables['tree'] . '</ul>';
}

/**
 * Bootstrap theme wrapper function for the primary menu links.
 */
function color_glass_menu_tree__primary(&$variables) {
  return '<ul class="menu nav navbar-nav">' . $variables['tree'] . '</ul>';
}

/**
 * Bootstrap theme wrapper function for the secondary menu links.
 */
function color_glass_menu_tree__secondary(&$variables) {
  return '<ul class="menu nav navbar-nav secondary">' . $variables['tree'] . '</ul>';
}

/**
 * Overrides theme_menu_tree() for book module.
 */
function color_glass_menu_tree__book_toc(&$variables) {
  $output = '<div class="book-toc btn-group pull-right">';
  $output .= '  <button type="button" class="btn btn-link dropdown-toggle" data-toggle="dropdown">';
  $output .= t('!icon Outline !caret', array(
    '!icon' => _bootstrap_icon('list'),
    '!caret' => '<span class="caret"></span>',
  ));
  $output .= '</button>';
  $output .= '<ul class="dropdown-menu" role="menu">' . $variables['tree'] . '</ul>';
  $output .= '</div>';
  return $output;
}

/**
 * Overrides theme_menu_tree() for book module.
 */
function color_glass_menu_tree__book_toc__sub_menu(&$variables) {
  return '<ul class="dropdown-menu" role="menu">' . $variables['tree'] . '</ul>';
}

/**
 * Implements hook_preprocess_image().
 */
function color_glass_preprocess_image(&$variables) {
  // Add image shape, if necessary.
  if ($shape = _bootstrap_setting('image_shape')) {
    _bootstrap_add_class($shape, $variables);
  }

  // Add responsiveness, if necessary.
  if (_bootstrap_setting('image_responsive')) {
    _bootstrap_add_class('img-responsive', $variables);
  }
}

/**
 * Implements hook_preprocess_image_srcset().
 */
function color_glass_preprocess_image_srcset(&$variables) {
  // Add image shape, if necessary.
  if ($shape = _bootstrap_setting('image_shape')) {
    $variables['attributes']['class'][] = $shape;
  }
}

/**
 * Implements hook_preprocess_picture().
 */
function color_glass_preprocess_picture(&$variables) {
  // Add responsiveness, if necessary.
  if ($shape = _bootstrap_setting('image_responsive')) {
    $variables['attributes']['class'][] = $shape;
  }
}

/**
 * Implements hook_preprocess_table().
 */
function color_glass_preprocess_table(&$variables) {
  // Prepare classes array if necessary.
  if (!isset($variables['attributes']['class'])) {
    $variables['attributes']['class'] = array();
  }
  // Convert classes to an array.
  elseif (isset($variables['attributes']['class']) && is_string($variables['attributes']['class'])) {
    $variables['attributes']['class'] = explode(' ', $variables['attributes']['class']);
  }

  // Add the necessary classes to the table.
  _bootstrap_table_add_classes($variables['attributes']['class'], $variables);
}

/**
 * Overrides theme_table().
 *
 * Returns HTML for a table.
 *
 * @param array $variables
 *   An associative array containing:
 *   - header: An array containing the table headers. Each element of the array
 *     can be either a localized string or an associative array with the
 *     following keys:
 *     - "data": The localized title of the table column.
 *     - "field": The database field represented in the table column (required
 *       if user is to be able to sort on this column).
 *     - "sort": A default sort order for this column ("asc" or "desc"). Only
 *       one column should be given a default sort order because table sorting
 *       only applies to one column at a time.
 *     - Any HTML attributes, such as "colspan", to apply to the column header
 *       cell.
 *   - rows: An array of table rows. Every row is an array of cells, or an
 *     associative array with the following keys:
 *     - "data": an array of cells
 *     - Any HTML attributes, such as "class", to apply to the table row.
 *     - "no_striping": a boolean indicating that the row should receive no
 *       'even / odd' styling. Defaults to FALSE.
 *     Each cell can be either a string or an associative array with the
 *     following keys:
 *     - "data": The string to display in the table cell.
 *     - "header": Indicates this cell is a header.
 *     - Any HTML attributes, such as "colspan", to apply to the table cell.
 *   - footer: An array containing the table footer. Each element of the array
 *     can be either a localized string or an associative array with the
 *     following keys:
 *     - "data": The localized title of the table column.
 *     - "field": The database field represented in the table column (required
 *       if user is to be able to sort on this column).
 *     - "sort": A default sort order for this column ("asc" or "desc"). Only
 *       one column should be given a default sort order because table sorting
 *       only applies to one column at a time.
 *     - Any HTML attributes, such as "colspan", to apply to the column footer
 *       cell.
 *   - attributes: An array of HTML attributes to apply to the table tag.
 *   - caption: A localized string to use for the <caption> tag.
 *   - colgroups: An array of column groups. Each element of the array can be
 *     either:
 *     - An array of columns, each of which is an associative array of HTML
 *       attributes applied to the COL element.
 *     - An array of attributes applied to the COLGROUP element, which must
 *       include a "data" attribute. To add attributes to COL elements, set the
 *       "data" attribute with an array of columns, each of which is an
 *       associative array of HTML attributes.
 *     Here's an example for $colgroup:
 *     These optional tags are used to group and set properties on columns
 *     within a table. For example, one may easily group three columns and
 *     apply same background style to all.
 *   - sticky: Use a "sticky" table header.
 *   - empty: The message to display in an extra row if table does not have any
 *     rows.
 */
function color_glass_table(array $variables) {
  $header = $variables['header'];
  $rows = $variables['rows'];
  $footer = $variables['footer'];
  $attributes = $variables['attributes'];
  $caption = $variables['caption'];
  $colgroups = $variables['colgroups'];
  $sticky = $variables['sticky'];
  $empty = $variables['empty'];
  $responsive = $variables['responsive'];

  // Add sticky headers, if applicable.
  if (count($header) && $sticky) {
    drupal_add_js('misc/tableheader.js');
    // Add 'sticky-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
    $attributes['class'][] = 'sticky-enabled';
  }

  $output = '';

  if ($responsive) {
    $output .= "<div class=\"table-responsive\">\n";
  }

  $output .= '<table' . drupal_attributes($attributes) . ">\n";

  if (isset($caption)) {
    $output .= '<caption>' . $caption . "</caption>\n";
  }

  // Format the table columns:
  if (count($colgroups)) {
    foreach ($colgroups as $number => $colgroup) {
      $attributes = array();

      // Check if we're dealing with a simple or complex column.
      if (isset($colgroup['data'])) {
        foreach ($colgroup as $key => $value) {
          if ($key == 'data') {
            $cols = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $cols = $colgroup;
      }

      // Build colgroup.
      if (is_array($cols) && count($cols)) {
        $output .= ' <colgroup' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cols as $col) {
          $output .= ' <col' . drupal_attributes($col) . ' />';
        }
        $output .= " </colgroup>\n";
      }
      else {
        $output .= ' <colgroup' . drupal_attributes($attributes) . " />\n";
      }
    }
  }

  // Add the 'empty' row message if available.
  if (!count($rows) && $empty) {
    $header_count = 0;
    foreach ($header as $header_cell) {
      if (is_array($header_cell)) {
        $header_count += isset($header_cell['colspan']) ? $header_cell['colspan'] : 1;
      }
      else {
        ++$header_count;
      }
    }
    $rows[] = array(
      array(
        'data' => $empty,
        'colspan' => $header_count,
        'class' => array('empty', 'message'),
      ),
    );
  }

  // Format the table header:
  if (count($header)) {
    $ts = tablesort_init($header);
    // HTML requires that the thead tag has tr tags in it followed by tbody
    // tags. Using ternary operator to check and see if we have any rows.
    $output .= (count($rows) ? ' <thead><tr>' : ' <tr>');
    foreach ($header as $cell) {
      $cell = tablesort_header($cell, $header, $ts);
      $output .= _theme_table_cell($cell, TRUE);
    }
    // Using ternary operator to close the tags based on whether or not there
    // are rows.
    $output .= (count($rows) ? " </tr></thead>\n" : "</tr>\n");
  }
  else {
    $ts = array();
  }

  // Format the table rows:
  if (count($rows)) {
    $output .= "<tbody>\n";
    foreach ($rows as $row) {
      // Check if we're dealing with a simple or complex row.
      if (isset($row['data'])) {
        $cells = $row['data'];

        // Set the attributes array and exclude 'data' and 'no_striping'.
        $attributes = $row;
        unset($attributes['data']);
        unset($attributes['no_striping']);
      }
      else {
        $cells = $row;
        $attributes = array();
      }
      if (count($cells)) {
        // Build row.
        $output .= ' <tr' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cells as $cell) {
          $cell = tablesort_cell($cell, $header, $ts, $i++);
          $output .= _theme_table_cell($cell);
        }
        $output .= " </tr>\n";
      }
    }
    $output .= "</tbody>\n";
  }

  // Format the table footer:
  if (count($footer)) {
    $output .= "<tfoot>\n";
    foreach ($footer as $row) {
      // Check if we're dealing with a simple or complex row.
      if (isset($row['data'])) {
        $cells = $row['data'];

        // Set the attributes array and exclude 'data'.
        $attributes = $row;
        unset($attributes['data']);
      }
      else {
        $cells = $row;
        $attributes = array();
      }
      if (count($cells)) {
        // Build row.
        $output .= ' <tr' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cells as $cell) {
          $cell = tablesort_cell($cell, $header, $ts, $i++);
          $output .= _theme_table_cell($cell);
        }
        $output .= " </tr>\n";
      }
    }
    // Using ternary operator to close the tags based on whether or not there
    // are rows.
    $output .= "</tfoot>\n";
  }

  $output .= "</table>\n";

  if ($responsive) {
    $output .= "</div>\n";
  }

  return $output;
}

/**
 * Implements hook_preprocess_views_view_table().
 */
function color_glass_preprocess_views_view_table(&$vars) {
  _bootstrap_table_add_classes($vars['classes_array'], $vars);
}


/**
 * Overrides theme_file_widget().
 */
function color_glass_file_widget($variables) {
  $output = '';
  $element = $variables['element'];
  $element['upload_button']['#attributes']['class'][] = 'btn-primary';
  $element['upload_button']['#prefix'] = '<span class="input-group-btn">';
  $element['upload_button']['#suffix'] = '</span>';

  // The "form-managed-file" class is required for proper Ajax functionality.
  if (!empty($element['filename'])) {
    $output .= '<div class="file-widget form-managed-file clearfix">';
    // Add the file size after the file name.
    $element['filename']['#markup'] .= ' <span class="file-size badge">' . format_size($element['#file']->filesize) . '</span>';
  }
  else {
    $output .= '<div class="file-widget form-managed-file clearfix input-group">';
  }

  // Immediately render hidden elements before the rest of the output.
  // The uploadprogress extension requires that the hidden identifier input
  // element appears before the file input element. They must also be siblings
  // inside the same parent element.
  // @see https://www.drupal.org/node/2155419
  foreach (element_children($element) as $child) {
    if (isset($element[$child]['#type']) && $element[$child]['#type'] === 'hidden') {
      $output .= drupal_render($element[$child]);
    }
  }

  // Render the rest of the element.
  $output .= drupal_render_children($element);
  $output .= '</div>';
  return $output;
}

/**
 * Returns HTML for a group of file upload widgets.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: A render element representing the widgets.
 *
 * @ingroup themeable
 */
function color_glass_file_widget_multiple(array $variables) {
  $element = $variables['element'];

  // Special ID and classes for draggable tables.
  $weight_class = $element['#id'] . '-weight';
  $table_id = $element['#id'] . '-table';

  // Build up a table of applicable fields.
  $headers = array();
  $headers[] = t('File information');
  if ($element['#display_field']) {
    $headers[] = array(
      'data' => t('Display'),
      'class' => array('checkbox'),
    );
  }
  $headers[] = t('Weight');
  $headers[] = t('Operations');

  // Get our list of widgets in order (needed when the form comes back after
  // preview or failed validation).
  $widgets = array();
  foreach (element_children($element) as $key) {
    $widgets[] = &$element[$key];
  }
  usort($widgets, '_field_sort_items_value_helper');

  $rows = array();
  foreach ($widgets as $key => &$widget) {
    // Save the uploading row for last.
    if ($widget['#file'] == FALSE) {
      $widget['#title'] = $element['#file_upload_title'];
      $widget['#description'] = $element['#file_upload_description'];
      continue;
    }

    // Delay rendering of the buttons, so that they can be rendered later in the
    // "operations" column.
    $operations_elements = array();
    foreach (element_children($widget) as $sub_key) {
      if (isset($widget[$sub_key]['#type']) && $widget[$sub_key]['#type'] == 'submit') {
        hide($widget[$sub_key]);
        $operations_elements[] = &$widget[$sub_key];
      }
    }

    // Delay rendering of the "Display" option and the weight selector, so that
    // each can be rendered later in its own column.
    if ($element['#display_field']) {
      hide($widget['display']);
    }
    hide($widget['_weight']);

    // Render everything else together in a column, without the normal wrappers.
    $widget['#theme_wrappers'] = array();
    $information = drupal_render($widget);

    // Render the previously hidden elements, using render() instead of
    // drupal_render(), to undo the earlier hide().
    $operations = '';
    foreach ($operations_elements as $operation_element) {
      $operation_element['#attributes']['class'][] = 'btn-xs';
      switch ($operation_element['#value']) {
        case t('Remove'):
          $operation_element['#icon'] = _bootstrap_icon('remove');
          break;
      }
      $operations .= render($operation_element);
    }
    $display = '';
    if ($element['#display_field']) {
      unset($widget['display']['#title']);
      $display = array(
        'data' => render($widget['display']),
        'class' => array('checkbox'),
      );
    }
    $widget['_weight']['#attributes']['class'] = array($weight_class);
    $weight = render($widget['_weight']);

    // Arrange the row with all of the rendered columns.
    $row = array();
    $row[] = $information;
    if ($element['#display_field']) {
      $row[] = $display;
    }
    $row[] = $weight;
    $row[] = $operations;
    $rows[] = array(
      'data' => $row,
      'class' => isset($widget['#attributes']['class']) ? array_merge($widget['#attributes']['class'], array('draggable')) : array('draggable'),
    );
  }

  drupal_add_tabledrag($table_id, 'order', 'sibling', $weight_class);

  $output = '';
  if (!empty($rows)) {
    $table = array(
      '#theme' => 'table',
      '#header' => $headers,
      '#rows' => $rows,
      '#attributes' => array(
        'id' => $table_id,
        'class' => array(
          'managed-files',
        ),
      ),
    );
    $output = drupal_render($table);
  }
  $output .= drupal_render_children($element);
  return $output;
}

/**
 * Returns HTML for a set of filter tips.
 *
 * @param array $variables
 *   An associative array containing:
 *   - tips: An array containing descriptions and a CSS ID in the form of
 *     'module-name/filter-id' (only used when $long is TRUE) for each
 *     filter in one or more text formats. Example:
 *   - long: (optional) Whether the passed-in filter tips contain extended
 *     explanations, i.e. intended to be output on the path 'filter/tips'
 *     (TRUE), or are in a short format, i.e. suitable to be displayed below a
 *     form element. Defaults to FALSE.
 *
 * @see _filter_tips()
 *
 * @ingroup themeable
 */
function color_glass_filter_tips(array $variables) {
  $format_id = arg(2);
  $current_path = current_path();
  $tips = _filter_tips(-1, TRUE);

  // Create a place holder for the tabs.
  $build['tabs'] = array(
    '#theme' => 'item_list',
    '#items' => array(),
    '#attributes' => array(
      'class' => array(
        'nav',
        'nav-tabs',
      ),
      'role' => 'tablist',
    ),
  );

  // Create a placeholder for the panes.
  $build['panes'] = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array(
      'class' => array(
        'tab-content',
      ),
    ),
  );

  foreach ($tips as $name => $list) {
    $machine_name = str_replace('-', '_', drupal_html_class($name));
    $tab = array(
      'data' => array(
        '#type' => 'link',
        '#title' => check_plain($name),
        '#href' => $current_path,
        '#attributes' => array(
          'role' => 'tab',
          'data-toggle' => 'tab',
        ),
        '#options' => array(
          'fragment' => $machine_name,
        ),
      ),
    );
    if (!$format_id || $format_id === $machine_name) {
      $tab['class'][] = 'active';
      $format_id = $machine_name;
    }
    $build['tabs']['#items'][] = $tab;

    // Extract the actual tip.
    $tiplist = array();
    foreach ($list as $tip) {
      $tiplist[] = $tip['tip'];
    }

    // Construct the pane.
    $pane = array(
      '#theme_wrappers' => array('container'),
      '#attributes' => array(
        'class' => array(
          'tab-pane',
          'fade',
        ),
        'id' => $machine_name,
      ),
      'list' => array(
        '#theme' => 'item_list',
        '#items' => $tiplist,
      ),
    );
    if ($format_id === $machine_name) {
      $pane['#attributes']['class'][] = 'active';
      $pane['#attributes']['class'][] = 'in';
      $format_id = $machine_name;
    }
    $build['panes'][] = $pane;
  }

  return drupal_render($build);
}

/**
 * Returns HTML for a link to the more extensive filter tips.
 *
 * @ingroup themeable
 */
function color_glass_filter_tips_more_info() {
  $attributes = array(
    'target' => '_blank',
    'title' => t('Opens in new window'),
  );
  if (_bootstrap_setting('tooltip_enabled')) {
    $attributes['data-toggle'] = 'tooltip';
  }
  return l(_bootstrap_icon('question-sign') . t('More information about text formats'), 'filter/tips', array(
    'html' => TRUE,
    'attributes' => $attributes,
  ));
}

/**
 * Returns HTML for a text format-enabled form element.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: A render element containing #children and #description.
 *
 * @ingroup themeable
 */
function color_glass_text_format_wrapper(array $variables) {
  $element = $variables['element'];
  $output = '<div class="text-format-wrapper">';
  $output .= $element['#children'];
  if (!empty($element['#description'])) {
    $output .= '<p class="help-block">' . $element['#description'] . '</p>';
  }
  $output .= "</div>\n";
  return $output;
}

/**
 * Overrides theme_item_list().
 *
 * Uses an early D8 version of the theme function, which fixes bugs (and was
 *   refused for commit because it was "too late to change theme output)".
 * - Removes first/last, even/odd classes.
 * - Removes useless div.item-list wrapper, allows optional #wrapper_attributes.
 * - Removes hard-coded #title as <h3>, introduce support for #title as an array
 *   containing, text, tag and optional attributes.
 */
function color_glass_item_list($variables) {
  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $list_attributes = $variables['attributes'];

  // Drupal core only supports #title as a string. This implementation supports
  // heading level, and attributes as well.
  $heading = '';
  if (!empty($title)) {
    // If it's a string, normalize into an array.
    if (is_string($title)) {
      $title = array(
        'text' => $title,
      );
    }
    // Set defaults.
    $title += array(
      'level' => 'h3',
      'attributes' => array(),
    );
    // Heading outputs only when it has text.
    if (!empty($title['text'])) {
      $heading .= '<' . $title['level'] . drupal_attributes($title['attributes']) . '>';
      $heading .= empty($title['html']) ? check_plain($title['text']) : $title['text'];
      $heading .= '</' . $title['level'] . '>';
    }
  }

  $output = '';
  if ($items) {
    $output .= '<' . $type . drupal_attributes($list_attributes) . '>';
    foreach ($items as $key => $item) {
      $attributes = array();

      if (is_array($item)) {
        $value = '';
        if (isset($item['data'])) {
          // Allow data to be renderable.
          if (is_array($item['data']) && (!empty($item['data']['#type']) || !empty($item['data']['#theme']))) {
            $value .= drupal_render($item['data']);
          }
          else {
            $value .= $item['data'];
          }
        }
        $attributes = array_diff_key($item, array('data' => 0, 'children' => 0));

        // Append nested child list, if any.
        if (isset($item['children'])) {
          // HTML attributes for the outer list are defined in the 'attributes'
          // theme variable, but not inherited by children. For nested lists,
          // all non-numeric keys in 'children' are used as list attributes.
          $child_list_attributes = array();
          foreach ($item['children'] as $child_key => $child_item) {
            if (is_string($child_key)) {
              $child_list_attributes[$child_key] = $child_item;
              unset($item['children'][$child_key]);
            }
          }
          $value .= theme('item_list', array(
            'items' => $item['children'],
            'type' => $type,
            'attributes' => $child_list_attributes,
          ));
        }
      }
      else {
        $value = $item;
      }

      $output .= '<li' . drupal_attributes($attributes) . '>' . $value . "</li>\n";
    }
    $output .= "</$type>";
  }

  // Output the list and title only if there are any list items.
  if (!empty($output)) {
    $output = $heading . $output;
  }

  return $output;
}

/**
 * Overrides theme_pager().
 */
function color_glass_pager($variables) {
  $output = "";
  $items = array();
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = $variables['quantity'];

  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // Current is the page we are currently paged to.
  $pager_current = $pager_page_array[$element] + 1;
  // First is the first page listed by this pager piece (re quantity).
  $pager_first = $pager_current - $pager_middle + 1;
  // Last is the last page listed by this pager piece (re quantity).
  $pager_last = $pager_current + $quantity - $pager_middle;
  // Max is the maximum page number.
  $pager_max = $pager_total[$element];

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }

  // End of generation loop preparation.
  // @todo add theme setting for this.
  $li_previous = theme('pager_previous', array(
    'text' => (isset($tags[1]) ? $tags[1] : t('previous')),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters,
  ));
  $li_next = theme('pager_next', array(
    'text' => (isset($tags[3]) ? $tags[3] : t('next')),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters,
  ));
  // @todo add theme setting for this.
  if ($pager_total[$element] > 1) {
    // @todo add theme setting for this.
    if ($li_previous) {
      $items[] = array(
        'class' => array('prev'),
        'data' => $li_previous,
      );
    }
    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      if ($i > 1) {
        $items[] = array(
          'class' => array('pager-ellipsis', 'disabled'),
          'data' => '<span>…</span>',
        );
      }
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            // 'class' => array('pager-item'),
            'data' => theme('pager_previous', array(
              'text' => $i,
              'element' => $element,
              'interval' => ($pager_current - $i),
              'parameters' => $parameters,
            )),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            // Add the active class.
            'class' => array('active'),
            'data' => "<span>$i</span>",
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            'data' => theme('pager_next', array(
              'text' => $i,
              'element' => $element,
              'interval' => ($i - $pager_current),
              'parameters' => $parameters,
            )),
          );
        }
      }
      if ($i < $pager_max) {
        $items[] = array(
          'class' => array('pager-ellipsis', 'disabled'),
          'data' => '<span>…</span>',
        );
      }
    }
    // End generation.
    if ($li_next) {
      $items[] = array(
        'class' => array('next'),
        'data' => $li_next,
      );
    }
    // @todo add theme setting for this.
    $build = array(
      '#theme_wrappers' => array('container__pager'),
      '#attributes' => array(
        'class' => array(
          'text-center',
        ),
      ),
      'pager' => array(
        '#theme' => 'item_list',
        '#items' => $items,
        '#attributes' => array(
          'class' => array('pagination'),
        ),
      ),
    );
    return drupal_render($build);
  }
  return $output;
}


/**
 * Overrides theme_status_messages().
 */
function color_glass_status_messages($variables) {
  $display = $variables['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
    'info' => t('Informative message'),
  );

  // Map Drupal message types to their corresponding Bootstrap classes.
  // @see http://twitter.github.com/bootstrap/components.html#alerts
  $status_class = array(
    'status' => 'success',
    'error' => 'danger',
    'warning' => 'warning',
    // Not supported, but in theory a module could send any type of message.
    // @see drupal_set_message()
    // @see theme_status_messages()
    'info' => 'info',
  );

  foreach (drupal_get_messages($display) as $type => $messages) {
    $class = (isset($status_class[$type])) ? ' alert-' . $status_class[$type] : '';
    $output .= "<div class=\"alert alert-block$class messages $type\">\n";
    $output .= "  <a class=\"close\" data-dismiss=\"alert\" href=\"#\">&times;</a>\n";

    if (!empty($status_heading[$type])) {
      $output .= '<h4 class="element-invisible">' . $status_heading[$type] . "</h4>\n";
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
 * Overrides theme_menu_local_action().
 */
function color_glass_menu_local_action($variables) {
  $link = $variables['element']['#link'];

  $options = isset($link['localized_options']) ? $link['localized_options'] : array();

  // If the title is not HTML, sanitize it.
  if (empty($options['html'])) {
    $link['title'] = check_plain($link['title']);
  }

  $icon = _bootstrap_iconize_text($link['title']);

  // Format the action link.
  $output = '';
  if (isset($link['href'])) {
    // Turn link into a mini-button and colorize based on title.
    if ($class = _bootstrap_colorize_text($link['title'])) {
      if (!isset($options['attributes']['class'])) {
        $options['attributes']['class'] = array();
      }
      $string = is_string($options['attributes']['class']);
      if ($string) {
        $options['attributes']['class'] = explode(' ', $options['attributes']['class']);
      }
      $options['attributes']['class'][] = 'btn';
      $options['attributes']['class'][] = 'btn-xs';
      $options['attributes']['class'][] = 'btn-' . $class;
      if ($string) {
        $options['attributes']['class'] = implode(' ', $options['attributes']['class']);
      }
    }
    // Force HTML so we can render any icon that may have been added.
    $options['html'] = !empty($options['html']) || !empty($icon) ? TRUE : FALSE;
    $output .= l($icon . $link['title'], $link['href'], $options);
  }
  else {
    $output .= $icon . $link['title'];
  }

  return $output;
}

/**
 * Overrides theme_form_element().
 */
function color_glass_form_element(&$variables) {
  $element = &$variables['element'];
  $name = !empty($element['#name']) ? $element['#name'] : FALSE;
  $type = !empty($element['#type']) ? $element['#type'] : FALSE;
  $checkbox = $type && $type === 'checkbox';
  $radio = $type && $type === 'radio';

  // Create an attributes array for the wrapping container.
  if (empty($element['#wrapper_attributes'])) {
    $element['#wrapper_attributes'] = array();
  }
  $wrapper_attributes = &$element['#wrapper_attributes'];

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
  );

  // Add wrapper ID for 'item' type.
  if ($type && $type === 'item' && !empty($element['#markup']) && !empty($element['#id'])) {
    $wrapper_attributes['id'] = $element['#id'];
  }

  // Check for errors and set correct error class.
  if ((isset($element['#parents']) && form_get_error($element)) || (!empty($element['#required']) && _bootstrap_setting('forms_required_has_error'))) {
    $wrapper_attributes['class'][] = 'has-error';
  }

  // Add necessary classes to wrapper container.
  $wrapper_attributes['class'][] = 'form-item';
  if ($name) {
    $wrapper_attributes['class'][] = 'form-item-' . drupal_html_class($name);
  }
  if ($type) {
    $wrapper_attributes['class'][] = 'form-type-' . drupal_html_class($type);
  }
  if (!empty($element['#attributes']['disabled'])) {
    $wrapper_attributes['class'][] = 'form-disabled';
  }
  if (!empty($element['#autocomplete_path']) && drupal_valid_path($element['#autocomplete_path'])) {
    $wrapper_attributes['class'][] = 'form-autocomplete';
  }

  // Checkboxes and radios do no receive the 'form-group' class, instead they
  // simply have their own classes.
  if ($checkbox || $radio) {
    $wrapper_attributes['class'][] = drupal_html_class($type);
  }
  elseif ($type && $type !== 'hidden') {
    $wrapper_attributes['class'][] = 'form-group';
  }

  // Create a render array for the form element.
  $build = array(
    '#theme_wrappers' => array('container__form_element'),
    '#attributes' => $wrapper_attributes,
  );

  // Render the label for the form element.
  $build['label'] = array(
    '#markup' => theme('form_element_label', $variables),
  );

  // Increase the label weight if it should be displayed after the element.
  if ($element['#title_display'] === 'after') {
    $build['label']['#weight'] = 10;
  }

  // Checkboxes and radios render the input element inside the label. If the
  // element is neither of those, then the input element must be rendered here.
  if (!$checkbox && !$radio) {
    $prefix = isset($element['#field_prefix']) ? $element['#field_prefix'] : '';
    $suffix = isset($element['#field_suffix']) ? $element['#field_suffix'] : '';
    if ((!empty($prefix) || !empty($suffix)) && (!empty($element['#input_group']) || !empty($element['#input_group_button']))) {
      if (!empty($element['#field_prefix'])) {
        $prefix = '<span class="input-group-' . (!empty($element['#input_group_button']) ? 'btn' : 'addon') . '">' . $prefix . '</span>';
      }
      if (!empty($element['#field_suffix'])) {
        $suffix = '<span class="input-group-' . (!empty($element['#input_group_button']) ? 'btn' : 'addon') . '">' . $suffix . '</span>';
      }

      // Add a wrapping container around the elements.
      $input_group_attributes = &_bootstrap_get_attributes($element, 'input_group_attributes');
      $input_group_attributes['class'][] = 'input-group';
      $prefix = '<div' . drupal_attributes($input_group_attributes) . '>' . $prefix;
      $suffix .= '</div>';
    }

    // Build the form element.
    $build['element'] = array(
      '#markup' => $element['#children'],
      '#prefix' => !empty($prefix) ? $prefix : NULL,
      '#suffix' => !empty($suffix) ? $suffix : NULL,
    );
  }

  // Construct the element's description markup.
  if (!empty($element['#description'])) {
    $build['description'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('help-block'),
      ),
      '#weight' => 20,
      0 => array('#markup' => filter_xss_admin($element['#description'])),
    );
  }

  // Render the form element build array.
  return drupal_render($build);
}


/**
 * Overrides theme_form_element_label().
 */
function color_glass_form_element_label(&$variables) {
  $element = $variables['element'];

  // Extract variables.
  $output = '';
  $title = isset($element['#title']) ? filter_xss_admin($element['#title']) . ' ' : '';
  $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '';
  if ($required) {
    $title .= $required;
  }
  $display = isset($element['#title_display']) ? $element['#title_display'] : 'before';
  $type = !empty($element['#type']) ? $element['#type'] : FALSE;
  $checkbox = $type && $type === 'checkbox';
  $radio = $type && $type === 'radio';

  // Immediately return if the element is not a checkbox or radio and there is
  // no label to be rendered.
  if (!$checkbox && !$radio && ($display === 'none' || (!$title && !$required))) {
    return '';
  }

  // Retrieve the label attributes array.
  $attributes = &_bootstrap_get_attributes($element, 'label_attributes');

  // Add Bootstrap label class.
  $attributes['class'][] = 'control-label';

  // Add the necessary 'for' attribute if the element ID exists.
  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // Checkboxes and radios must construct the label differently.
  if ($checkbox || $radio) {
    if ($display === 'before') {
      $output .= $title;
    }
    elseif ($display === 'none' || $display === 'invisible') {
      $output .= '<span class="element-invisible">' . $title . '</span>';
    }
    // Inject the rendered checkbox or radio element inside the label.
    if (!empty($element['#children'])) {
      $output .= $element['#children'];
    }
    if ($display === 'after') {
      $output .= $title;
    }
  }
  // Otherwise, just render the title as the label.
  else {
    // Show label only to screen readers to avoid disruption in visual flows.
    if ($display === 'invisible') {
      $attributes['class'][] = 'element-invisible';
    }
    $output .= $title;
  }

  // The leading whitespace helps visually separate fields from inline labels.
  return ' <label' . drupal_attributes($attributes) . '>' . $output . "</label>\n";
}


/**
 * Overrides theme_textfield.
 */
function color_glass_textfield($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'text';
  element_set_attributes($element, array(
    'id',
    'name',
    'value',
    'size',
    'maxlength',
  ));
  _form_set_class($element, array('form-text'));

  $output = '<input' . drupal_attributes($element['#attributes']) . ' />';

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
    // Uses icon for autocomplete "throbber".
    if ($icon = _bootstrap_icon('refresh')) {
      $output = '<div class="input-group">' . $output . '<span class="input-group-addon">' . $icon . '</span></div>';
    }
    // Fallback to using core's throbber.
    else {
      $output = '<div class="input-group">' . $output . '<span class="input-group-addon">';
      // The throbber's background image must be set here because sites may not
      // be at the root of the domain (ie: /) and this value cannot be set via
      // CSS.
      $output .= '<span class="autocomplete-throbber" style="background-image:url(' . url('misc/throbber.gif') . ')"></span>';
      $output .= '</span></div>';
    }
    $extra = '<input' . drupal_attributes($attributes) . ' />';
  }

  return $output . $extra;
}

/**
 * Overrides theme_date().
 */
function color_glass_date($variables) {
  $element = $variables['element'];

  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'form-inline';
  $attributes['class'][] = 'form-date';

  return '<div' . drupal_attributes($attributes) . '>' . drupal_render_children($element) . '</div>';
}

/**
 * Overrides theme_exposed_filters().
 */
function color_glass_exposed_filters($variables) {
  $form = $variables['form'];
  $output = '';

  foreach (element_children($form['status']['filters']) as $key) {
    $form['status']['filters'][$key]['#field_prefix'] = '<div class="col-sm-10">';
    $form['status']['filters'][$key]['#field_suffix'] = '</div>';
  }
  $form['status']['actions']['#attributes']['class'][] = 'col-sm-offset-2';
  $form['status']['actions']['#attributes']['class'][] = 'col-sm-10';

  if (isset($form['current'])) {
    $items = array();
    foreach (element_children($form['current']) as $key) {
      $items[] = drupal_render($form['current'][$key]);
    }
    $output .= theme('item_list', array(
      'items' => $items,
      'attributes' => array(
        'class' => array(
          'clearfix',
          'current-filters',
        ),
      ),
    ));
  }
  $output .= drupal_render_children($form);
  return '<div class="form-horizontal">' . $output . '</div>';
}

/**
 * Overrides theme_file_managed_file().
 */
function color_glass_file_managed_file($variables) {
  $output = '';
  $element = $variables['element'];

  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'form-managed-file';
  $attributes['class'][] = 'input-group';

  $element['upload_button']['#attributes']['class'][] = 'btn-primary';
  $element['upload_button']['#prefix'] = '<span class="input-group-btn">';
  $element['upload_button']['#suffix'] = '</span>';
  $element['remove_button']['#prefix'] = '<span class="input-group-btn">';
  $element['remove_button']['#suffix'] = '</span>';
  $element['remove_button']['#attributes']['class'][] = 'btn-danger';

  if (!empty($element['filename'])) {
    $element['filename']['#prefix'] = '<div class="form-control">';
    $element['filename']['#suffix'] = '</div>';
  }

  // This wrapper is required to apply JS behaviors and CSS styling.
  $output .= '<div' . drupal_attributes($attributes) . '>';

  // Immediately render hidden elements before the rest of the output.
  // The uploadprogress extension requires that the hidden identifier input
  // element appears before the file input element. They must also be siblings
  // inside the same parent element.
  // @see https://www.drupal.org/node/2155419
  foreach (element_children($element) as $child) {
    if (isset($element[$child]['#type']) && $element[$child]['#type'] === 'hidden') {
      $output .= drupal_render($element[$child]);
    }
  }

  // Render the rest of the element.
  $output .= drupal_render_children($element);
  $output .= '</div>';
  return $output;
}

/**
 * Returns HTML for help text based on file upload validators.
 *
 * @param array $variables
 *   An associative array containing:
 *   - description: The normal description for this field, specified by the
 *     user.
 *   - upload_validators: An array of upload validators as used in
 *     $element['#upload_validators'].
 *
 * @ingroup themeable
 */
function color_glass_file_upload_help(array $variables) {
  // If popover's are disabled, just theme this normally.
  if (!_bootstrap_setting('popover_enabled')) {
    return theme('file_upload_help', $variables);
  }

  $build = array();
  if (!empty($variables['description'])) {
    $build['description'] = array(
      '#markup' => $variables['description'] . '<br>',
    );
  }

  $descriptions = array();
  $upload_validators = $variables['upload_validators'];
  if (isset($upload_validators['file_validate_size'])) {
    $descriptions[] = t('Files must be less than !size.', array('!size' => '<strong>' . format_size($upload_validators['file_validate_size'][0]) . '</strong>'));
  }
  if (isset($upload_validators['file_validate_extensions'])) {
    $descriptions[] = t('Allowed file types: !extensions.', array('!extensions' => '<strong>' . check_plain($upload_validators['file_validate_extensions'][0]) . '</strong>'));
  }
  if (isset($upload_validators['file_validate_image_resolution'])) {
    $max = $upload_validators['file_validate_image_resolution'][0];
    $min = $upload_validators['file_validate_image_resolution'][1];
    if ($min && $max && $min == $max) {
      $descriptions[] = t('Images must be exactly !size pixels.', array('!size' => '<strong>' . $max . '</strong>'));
    }
    elseif ($min && $max) {
      $descriptions[] = t('Images must be between !min and !max pixels.', array('!min' => '<strong>' . $min . '</strong>', '!max' => '<strong>' . $max . '</strong>'));
    }
    elseif ($min) {
      $descriptions[] = t('Images must be larger than !min pixels.', array('!min' => '<strong>' . $min . '</strong>'));
    }
    elseif ($max) {
      $descriptions[] = t('Images must be smaller than !max pixels.', array('!max' => '<strong>' . $max . '</strong>'));
    }
  }

  if ($descriptions) {
    $id = drupal_html_id('upload-instructions');
    $build['instructions'] = array(
      '#theme' => 'link__file_upload_requirements',
      // @todo remove space between icon/text and fix via styling.
      '#text' => _bootstrap_icon('question-sign') . ' ' . t('More information'),
      '#path' => '#',
      '#options' => array(
        'attributes' => array(
          'data-toggle' => 'popover',
          'data-target' => "#$id",
          'data-html' => TRUE,
          'data-placement' => 'bottom',
          'data-title' => t('File requirements'),
        ),
        'html' => TRUE,
        'external' => TRUE,
      ),
    );
    $build['requirements'] = array(
      '#theme_wrappers' => array('container__file_upload_requirements'),
      '#attributes' => array(
        'id' => $id,
        'class' => array('element-invisible', 'help-block'),
      ),
    );
    $build['requirements']['validators'] = array(
      '#theme' => 'item_list__file_upload_requirements',
      '#items' => $descriptions,
    );
  }

  return drupal_render($build);
}


/**
 * Implements hook_preprocess_icon().
 *
 * Bootstrap requires an additional "glyphicon" class for all icons.
 *
 * @see icon_preprocess_icon_image()
 * @see template_preprocess_icon()
 */
function color_glass_preprocess_icon(&$variables) {
  $bundle = &$variables['bundle'];
  if ($bundle['provider'] === 'bootstrap') {
    $variables['attributes']['class'][] = 'glyphicon';
  }
}

/**
 * Overrides theme_container().
 */
function color_glass_container($variables) {
  $element = $variables['element'];

  // Ensure #attributes is set.
  $element += array('#attributes' => array());

  // Special handling for form elements.
  if (isset($element['#array_parents'])) {
    // Assign an html ID.
    if (!isset($element['#attributes']['id'])) {
      $element['#attributes']['id'] = $element['#id'];
    }

    // Core's "form-wrapper" class is required for states.js to function.
    $element['#attributes']['class'][] = 'form-wrapper';

    // Add Bootstrap "form-group" class.
    $element['#attributes']['class'][] = 'form-group';
  }

  return '<div' . drupal_attributes($element['#attributes']) . '>' . $element['#children'] . '</div>';
}


/**
 * Overrides theme_facetapi_link_active().
 */
function color_glass_facetapi_link_active($variables) {

  // Sanitizes the link text if necessary.
  $sanitize = empty($variables['options']['html']);
  $link_text = ($sanitize) ? check_plain($variables['text']) : $variables['text'];

  // Theme function variables fro accessible markup.
  // @see http://drupal.org/node/1316580
  $accessible_vars = array(
    'text' => $variables['text'],
    'active' => TRUE,
  );

  // Builds link, passes through t() which gives us the ability to change the
  // position of the widget on a per-language basis.
  $replacements = array(
    '!link_text' => '<span class="term">' . $link_text . '</span>',
    '!facetapi_deactivate_widget' => '<button class="close">&times;</button>',
    '!facetapi_accessible_markup' => theme('facetapi_accessible_markup', $accessible_vars),
  );
  $variables['text'] = t('!link_text !facetapi_deactivate_widget !facetapi_accessible_markup', $replacements);
  $variables['options']['html'] = TRUE;
  return theme('link', $variables);
}

/**
 * Implements hook_preprocess_region().
 */
function color_glass_preprocess_region(&$variables) {
  global $theme;

  $region = $variables['region'];
  $classes = &$variables['classes_array'];

  // Content region.
  if ($region === 'content') {
    // @todo is this actually used properly?
    $variables['theme_hook_suggestions'][] = 'region__no_wrapper';
  }
  // Help region.
  elseif ($region === 'help' && !empty($variables['content'])) {
    $variables['content'] = _bootstrap_icon('question-sign') . $variables['content'];
    $classes[] = 'alert';
    $classes[] = 'alert-info';
    $classes[] = 'messages';
    $classes[] = 'info';
  }

  // Support for "well" classes in regions.
  static $wells;
  if (!isset($wells)) {
    foreach (system_region_list($theme) as $name => $title) {
      $wells[$name] = _bootstrap_setting('region_well-' . $name);
    }
  }
  if (!empty($wells[$region])) {
    $classes[] = $wells[$region];
  }
}

/**
 * Implements hook_preprocess_button().
 */
function color_glass_preprocess_button(&$vars) {
  $element = &$vars['element'];

  // Set the element's attributes.
  element_set_attributes($element, array('id', 'name', 'value', 'type'));

  // Add the base Bootstrap button class.
  $element['#attributes']['class'][] = 'btn';

  // Add button size, if necessary.
  if ($size = _bootstrap_setting('button_size')) {
    $element['#attributes']['class'][] = $size;
  }

  // Colorize button.
  _bootstrap_colorize_button($element);

  // Add in the button type class.
  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];

  // Ensure that all classes are unique, no need for duplicates.
  $element['#attributes']['class'] = array_unique($element['#attributes']['class']);
}


/**
 * Overrides theme_button().
 */
function color_glass_button($variables) {
  $element = $variables['element'];

  // Allow button text to be appear hidden.
  // @see https://www.drupal.org/node/2327437
  $text = !empty($element['#hide_text']) ? '<span class="element-invisible">' . $element['#value'] . '</span>' : $element['#value'];

  // Add icons before or after the value.
  // @see https://www.drupal.org/node/2219965
  if (!empty($element['#icon'])) {
    if ($element['#icon_position'] === 'before') {
      $text = $element['#icon'] . ' ' . $text;
    }
    elseif ($element['#icon_position'] === 'after') {
      $text .= ' ' . $element['#icon'];
    }
  }

  // This line break adds inherent margin between multiple buttons.
  return '<button' . drupal_attributes($element['#attributes']) . '>' . $text . "</button>\n";
}

/**
 * Implements hook_preprocess_links().
 */
function color_glass_preprocess_links(&$variables) {
  if (isset($variables['attributes']) && isset($variables['attributes']['class'])) {
    $string = is_string($variables['attributes']['class']);
    if ($string) {
      $variables['attributes']['class'] = explode(' ', $variables['attributes']['class']);
    }

    if ($key = array_search('inline', $variables['attributes']['class'])) {
      $variables['attributes']['class'][$key] = 'list-inline';
    }

    if ($string) {
      $variables['attributes']['class'] = implode(' ', $variables['attributes']['class']);
    }
  }
}

/**
 * Overrides theme_menu_link().
 */
function color_glass_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    // Prevent dropdown functions from being added to management menu so it
    // does not affect the navbar module.
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = drupal_render($element['#below']);
    }
    elseif ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] >= 1)) {
      // Create depth class.
      $menu_depth = 'menu-depth-' . $element['#original_link']['depth'];
      // Add our own wrapper.
      unset($element['#below']['#theme_wrappers']);
      $sub_menu = '<ul class="dropdown-menu">' . drupal_render($element['#below']) . '</ul>';
      // Generate as standard dropdown.
      $element['#title'] .= ' <span class="caret"></span>';
      $element['#attributes']['class'][] = 'dropdown';
      // Add depth to menu.
      $element['#attributes']['class'][] = $menu_depth;
      $element['#localized_options']['html'] = TRUE;

      // Set dropdown trigger element to # to prevent inadvertent page loading
      // when a submenu link is clicked.
      $element['#localized_options']['attributes']['data-target'] = '#';
      $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
    }
  }
  // On primary navigation menu, class 'active' is not set on active menu item.
  // @see https://drupal.org/node/1896674
  if (($element['#href'] == $_GET['q'] || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']))) {
    $element['#attributes']['class'][] = 'active';
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Overrides theme_menu_link() for book module.
 */
function color_glass_menu_link__book_toc(array $variables) {
  $element = $variables['element'];
  $sub_menu = drupal_render($element['#below']);
  $element['#attributes']['role'] = 'presentation';
  $link = TRUE;
  if ($element['#title'] && $element['#href'] === FALSE) {
    $element['#attributes']['class'][] = 'dropdown-header';
    $link = FALSE;
  }
  elseif ($element['#title'] === FALSE && $element['#href'] === FALSE) {
    $element['#attributes']['class'][] = 'divider';
    $link = FALSE;
  }
  elseif (($element['#href'] == $_GET['q'] || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']))) {
    $element['#attributes']['class'][] = 'active';
  }
  if ($link) {
    $element['#title'] = l($element['#title'], $element['#href'], $element['#localized_options']);
  }
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $element['#title'] . $sub_menu . "</li>\n";
}


/**
 * Implements hook_menu_local_tasks_alter().
 */
function color_glass_menu_local_tasks_alter(&$data, &$router_item, $root_path) {
  if (!empty($data['actions']['output'])) {
    $items = array();
    foreach ($data['actions']['output'] as $item) {
      $items[] = array(
        'data' => $item,
      );
    }
    $data['actions']['output'] = array(
      '#theme' => 'item_list__action_links',
      '#items' => $items,
      '#attributes' => array(
        'class' => array('action-links'),
      ),
    );
  }
}
/**
 * Overrides theme_menu_local_task().
 */
function color_glass_menu_local_task($variables) {
  $link = $variables['element']['#link'];
  $link_text = $link['title'];
  $attributes = array();

  if (!empty($variables['element']['#active'])) {
    // Add text to indicate active tab for non-visual users.
    $active = '<span class="element-invisible">' . t('(active tab)') . '</span>';

    // If the link does not contain HTML already, check_plain() it now.
    // After we set 'html'=TRUE the link will not be sanitized by l().
    if (empty($link['localized_options']['html'])) {
      $link['title'] = check_plain($link['title']);
    }
    $link['localized_options']['html'] = TRUE;
    $link_text = t('!local-task-title!active', array('!local-task-title' => $link['title'], '!active' => $active));

    $attributes['class'][] = 'active';
  }

  return '<li' . drupal_attributes($attributes) . '>' . l($link_text, $link['href'], $link['localized_options']) . "</li>\n";
}

/**
 * Overrides theme_menu_local_tasks().
 */
function color_glass_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs--primary nav nav-tabs">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }

  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs--secondary pagination pagination-sm">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

/**
 * Overrides theme_fieldset().
 */
function color_glass_fieldset($variables) {
  return theme('bootstrap_panel', $variables);
}

/**
 * Implements hook_js_callback_filter_xss_alter().
 */
function color_glass_js_callback_filter_xss_alter(array &$allowed_tags = array()) {
  $allowed_tags[] = 'button';
}


/**
 * Implements hook_process_html_tag().
 */
function color_glass_process_html_tag(&$variables) {
  // Reference the element and tag name for easier coding below.
  $element = &$variables['element'];
  $tag = $element['#tag'];
  if ($tag === 'style' || $tag === 'script') {
    // Remove default "type" attribute. Leave others unaffected as it may be
    // needed and used for other purposes.
    // @see http://stackoverflow.com/a/5265361/1226717
    // @see https://drupal.org/node/2201779
    $types = array(
      // @see http://www.w3.org/TR/html5/document-metadata.html#attr-style-type
      'style' => 'text/css',
      // @see http://www.w3.org/TR/html5/scripting-1.html#attr-script-type
      'script' => 'text/javascript',
    );
    if (!empty($element['#attributes']['type']) && $element['#attributes']['type'] === $types[$tag]) {
      unset($element['#attributes']['type']);
    }

    // Remove CDATA comments. CDATA is only required for DOCTYPES that are XML
    // based, HTML5 is not.
    $cdata_prefix = array(
      'style' => "\n<!--/*--><![CDATA[/*><!--*/\n",
      'script' => "\n<!--//--><![CDATA[//><!--\n",
    );
    $cdata_suffix = array(
      'style' => "\n/*]]>*/-->\n",
      'script' => "\n//--><!]]>\n",
    );
    if (
      !empty($element['#value_prefix']) && $element['#value_prefix'] === $cdata_prefix[$tag] &&
      !empty($element['#value_suffix']) && $element['#value_suffix'] === $cdata_suffix[$tag]
    ) {
      unset($element['#value_prefix'], $element['#value_suffix']);
    }

    // Remove the "media=all" attribute, leave others unaffected.
    if (isset($element['#attributes']['media']) && $element['#attributes']['media'] === 'all') {
      unset($element['#attributes']['media']);
    }
  }
}


/**
 * Implements hook_element_info_alter().
 */
function color_glass_element_info_alter(&$info) {
  global $theme_key;

  $cid = "theme_registry:color_glass:element_info";
  $cached = array();
  if (($cache = cache_get($cid)) && !empty($cache->data)) {
    $cached = $cache->data;
  }

  $themes = _bootstrap_get_base_themes($theme_key, TRUE);
  foreach ($themes as $theme) {
    if (!isset($cached[$theme])) {
      $cached[$theme] = array();
      foreach (array_keys($info) as $type) {
        $element = array();

        // Ensure elements that have a base type with the #input set match.
        if (isset($info[$type]['#base_type']) && isset($info[$type][$info[$type]['#base_type']]['#input'])) {
          $element['#input'] = $info[$info[$type]['#base_type']]['#input'];
        }

        // Replace fieldset theme implementations with bootstrap_panel.
        if (!empty($info[$type]['#theme']) && $info[$type]['#theme'] === 'fieldset') {
          $element['#bootstrap_replace']['#theme'] = 'bootstrap_panel';
        }
        if (!empty($info[$type]['#theme_wrappers']) && array_search('fieldset', $info[$type]['#theme_wrappers']) !== FALSE) {
          $element['#bootstrap_replace']['#theme_wrappers']['fieldset'] = 'bootstrap_panel';
        }

        // Setup a default "icon" variable. This allows #icon to be passed
        // to every template and theme function.
        // @see https://drupal.org/node/2219965
        $element['#icon'] = NULL;
        $element['#icon_position'] = 'before';

        $properties = array(
          '#process' => array(
            'form_process',
            'form_process_' . $type,
          ),
          '#pre_render' => array(
            'pre_render',
            'pre_render_' . $type,
          ),
        );
        foreach ($properties as $property => $callbacks) {
          foreach ($callbacks as $callback) {
            $function = $theme . '_' . $callback;
            if (function_exists($function)) {
              // Replace direct core function correlation.
              if (!empty($info[$type][$property]) && array_search($callback, $info[$type][$property]) !== FALSE) {
                $element['#bootstrap_replace'][$property][$callback] = $function;
              }
              // Check for a "form_" prefix instead (for #pre_render).
              elseif (!empty($info[$type][$property]) && array_search('form_' . $callback, $info[$type][$property]) !== FALSE) {
                $element['#bootstrap_replace'][$property]['form_' . $callback] = $function;
              }
              // Otherwise, append the function.
              else {
                $element[$property][] = $function;
              }
            }
          }
        }
        $cached[$theme][$type] = $element;
      }

      // Cache the element information.
      cache_set($cid, $cached);
    }

    // Merge in each theme's cached element info.
    $info = _bootstrap_element_info_array_merge($info, $cached[$theme]);
  }
}

/**
 * Implements hook_form_alter().
 */
function color_glass_form_alter(array &$form, array &$form_state = array(), $form_id = NULL) {
  if ($form_id) {
    switch ($form_id) {
      case 'system_theme_settings':
        // Create vertical tabs for global settings (provided by core or other
        // contrib modules).
        if (!isset($form['global'])) {
          $form['global'] = array(
            '#type' => 'vertical_tabs',
            '#weight' => -9,
          );
          if (!empty($form_state['build_info']['args'][0])) {
            $form['global']['#prefix'] = '<h2><small>' . t('Override Global Settings') . '</small></h2>';
          }
        }

        // Iterate over all child elements and check to see if they should be
        // moved in the global vertical tabs.
        $global_children = element_children($form);
        foreach ($global_children as $child) {
          if (isset($form[$child]['#type']) && $form[$child]['#type'] === 'fieldset' && !isset($form[$child]['#group'])) {
            $form[$child]['#group'] = 'global';
          }
        }
        break;

      case 'search_form':
        // Add a clearfix class so the results don't overflow onto the form.
        $form['#attributes']['class'][] = 'clearfix';

        // Remove container-inline from the container classes.
        $form['basic']['#attributes']['class'] = array();

        // Hide the default button from display.
        $form['basic']['submit']['#attributes']['class'][] = 'element-invisible';

        // Implement a theme wrapper to add a submit button containing a search
        // icon directly after the input element.
        $form['basic']['keys']['#theme_wrappers'] = array('bootstrap_search_form_wrapper');
        $form['basic']['keys']['#title'] = '';
        $form['basic']['keys']['#attributes']['placeholder'] = t('Search');
        break;

      case 'search_block_form':
        $form['#attributes']['class'][] = 'form-search';

        $form['search_block_form']['#title'] = '';
        $form['search_block_form']['#attributes']['placeholder'] = t('Search');

        // Hide the default button from display and implement a theme wrapper
        // to add a submit button containing a search icon directly after the
        // input element.
        $form['actions']['submit']['#attributes']['class'][] = 'element-invisible';
        $form['search_block_form']['#theme_wrappers'] = array('bootstrap_search_form_wrapper');

        // Apply a clearfix so the results don't overflow onto the form.
        $form['#attributes']['class'][] = 'content-search';
        break;

      case 'image_style_form':
        $form['effects']['new']['new']['#input_group_button'] = TRUE;
        break;

      case 'path_admin_filter_form':
        $form['basic']['filter']['#input_group_button'] = TRUE;
        break;
    }

  }
}

/**
 * Implements theme_preprocess_bootstrap_modal().
 *
 * @todo: Replace with "bootstrap_effect_fade" theme setting.
 */
function color_glass_preprocess_bootstrap_modal(&$variables) {
  if (empty($variables['attributes']['id'])) {
    $variables['attributes']['id'] = drupal_html_id(strip_tags($variables['heading']));
  }
  $variables['attributes']['class'][] = 'modal';
  $variables['attributes']['class'][] = 'fade';
  $variables['attributes']['tabindex'] = -1;
  $variables['attributes']['role'] = 'dialog';
  $variables['attributes']['aria-hidden'] = 'true';

  $variables['heading'] = $variables['html_heading'] ? $variables['heading'] : check_plain($variables['heading']);
}

/**
 * Implements hook_preprocess_bootstrap_panel().
 */
function color_glass_preprocess_bootstrap_panel(&$variables) {
  $element = &$variables['element'];
  $attributes = !empty($element['#attributes']) ? $element['#attributes'] : array();
  $attributes['class'][] = 'panel';
  $attributes['class'][] = 'panel-default';
  // states.js requires form-wrapper on fieldset to work properly.
  $attributes['class'][] = 'form-wrapper';
  $variables['collapsible'] = FALSE;
  if (isset($element['#collapsible'])) {
    $variables['collapsible'] = $element['#collapsible'];
  }
  $variables['collapsed'] = FALSE;
  if (isset($element['#collapsed'])) {
    $variables['collapsed'] = $element['#collapsed'];
  }
  // Force grouped fieldsets to not be collapsible (for vertical tabs).
  if (!empty($element['#group'])) {
    $variables['collapsible'] = FALSE;
    $variables['collapsed'] = FALSE;
  }
  if (!isset($element['#id']) && $variables['collapsible']) {
    $element['#id'] = drupal_html_id('bootstrap-panel');
  }
  $variables['target'] = NULL;
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
    $variables['target'] = '#' . $element['#id'] . ' > .collapse';
  }
  $variables['content'] = $element['#children'];
  if (isset($element['#value'])) {
    $variables['content'] .= $element['#value'];
  }

  // Iterate over optional variables.
  $keys = array(
    'description',
    'prefix',
    'suffix',
    'title',
  );
  foreach ($keys as $key) {
    $variables[$key] = !empty($element["#$key"]) ? $element["#$key"] : FALSE;
  }
  $variables['attributes'] = $attributes;
}

/**
 * Implements hook_process_bootstrap_panel().
 */
function color_glass_process_bootstrap_panel(&$variables) {
  $variables['attributes'] = drupal_attributes($variables['attributes']);
}

/**
 * Implements theme_process_bootstrap_modal().
 */
function color_glass_process_bootstrap_modal(&$variables) {
  $variables['attributes'] = drupal_attributes($variables['attributes']);
  $variables['body'] = render($variables['body']);
  $variables['footer'] = render($variables['footer']);
}


/**
 * Theme function implementation for bootstrap_search_form_wrapper.
 */
function color_glass_bootstrap_search_form_wrapper($variables) {
  $output = '<div class="input-group">';
  $output .= $variables['element']['#children'];
  $output .= '<span class="input-group-btn">';
  $output .= '<button type="submit" class="btn btn-primary">' . _bootstrap_icon('search', t('Search')) . '</button>';
  $output .= '</span>';
  $output .= '</div>';
  return $output;
}

/**
 * Implements hook_icon_bundle_list_alter().
 */
function color_glass_icon_bundle_list_alter(&$build, $bundle) {
  if (_bootstrap_setting('tooltip_enabled')) {
    foreach ($build as &$icon) {
      $icon['#attributes']['data-toggle'] = 'tooltip';
      $icon['#attributes']['data-placement'] = 'bottom';
    }
  }
}

/**
 * Implements hook_icon_providers().
 */
function color_glass_icon_providers() {
  $providers['bootstrap'] = array(
    'title' => t('Bootstrap'),
    'url' => 'http://getbootstrap.com/components/#glyphicons',
  );
  return $providers;
}

/**
 * Implements hook_icon_bundles().
 */
function color_glass_icon_bundles() {
  $bundles = array();
  if (_bootstrap_glyphicons_supported()) {
    $bundles['bootstrap'] = array(
      'render' => 'sprite',
      'provider' => 'bootstrap',
      'title' => t('Bootstrap'),
      'version' => t('Icons by Glyphicons'),
      'variations' => array(
        'icon-white' => t('White'),
      ),
      'settings' => array(
        'tag' => 'span',
      ),
      'icons' => _bootstrap_glyphicons(),
    );
  }
  return $bundles;
}

/**
 * Implements hook_pre_render_HOOK().
 */
function color_glass_pre_render_fieldset($element) {
  // Fieldsets may be rendered outside of a Form API context.
  if (!empty($element['#bootstrap_ignore_pre_render']) || !isset($element['#parents']) || !isset($element['#groups'])) {
    return $element;
  }
  // Inject group member elements belonging to this group.
  $parents = implode('][', $element['#parents']);
  $children = element_children($element['#groups'][$parents]);
  if (!empty($children)) {
    if (empty($element['#default_tab'])) {
      $children_keys = array_values($children);
      $element['#default_tab'] = $element['#groups'][$parents][array_shift($children_keys)]['#id'];
    }
    foreach ($children as $key) {
      // Break references and indicate that the element should be rendered as
      // group member.
      $child = (array) $element['#groups'][$parents][$key];
      $child['#attributes']['id'] = $child['#id'];
      $child['#group_fieldset'] = TRUE;
      // Inject the element as new child element.
      $element[] = $child;

      $sort = TRUE;
    }
    // Re-sort the element's children if we injected group member elements.
    if (isset($sort)) {
      $element['#sorted'] = FALSE;
    }
  }

  if (isset($element['#group'])) {
    $group = $element['#group'];
    // If this element belongs to a group, but the group-holding element does
    // not exist, we need to render it (at its original location).
    if (!isset($element['#groups'][$group]['#group_exists'])) {
      // Intentionally empty to clarify the flow; we simply return $element.
    }
    // If we injected this element into the group, then we want to render it.
    elseif (!empty($element['#group_fieldset'])) {
      // Intentionally empty to clarify the flow; we simply return $element.
    }
    // Otherwise, this element belongs to a group and the group exists, so we do
    // not render it.
    elseif (element_children($element['#groups'][$group])) {
      $element['#printed'] = TRUE;
    }
  }

  return $element;
}

/**
 * Implements hook_pre_render().
 */
function color_glass_pre_render($element) {
  if (!empty($element['#bootstrap_ignore_pre_render'])) {
    return $element;
  }

  // Only add the "form-control" class for specific element input types.
  $types = array(
    // Core.
    'password',
    'password_confirm',
    'machine_name',
    'select',
    'textarea',
    'textfield',

    // Elements module (HTML5).
    'date',
    'datefield',
    'email',
    'emailfield',
    'number',
    'numberfield',
    'range',
    'rangefield',
    'search',
    'searchfield',
    'tel',
    'telfield',
    'url',
    'urlfield',

    // Webform module.
    'webform_email',
    'webform_number',
  );

  // Add necessary classes for specific types.
  if ($type = !empty($element['#type']) ? $element['#type'] : FALSE) {
    if (in_array($type, $types) || ($type === 'file' && empty($element['#managed_file']))) {
      $element['#attributes']['class'][] = 'form-control';
    }
    if ($type === 'machine_name') {
      $element['#wrapper_attributes']['class'][] = 'form-inline';
    }
  }

  // Add smart descriptions to the element, if necessary.
  _bootstrap_element_smart_description($element);

  // Return the modified element.
  return $element;
}

/**
 * Implements hook_form_process().
 */
function color_glass_form_process($element, &$form_state, &$form) {
  if (!empty($element['#bootstrap_ignore_process'])) {
    return $element;
  }

  if (!empty($element['#attributes']['class']) && is_array($element['#attributes']['class'])) {
    $key = array_search('container-inline', $element['#attributes']['class']);
    if ($key !== FALSE) {
      $element['#attributes']['class'][$key] = 'form-inline';
    }

    if (in_array('form-wrapper', $element['#attributes']['class'])) {
      $element['#attributes']['class'][] = 'form-group';
    }
  }

  // Automatically inject the nearest button found after this element if
  // #input_group_button exists.
  if (!empty($element['#input_group_button'])) {
    // Obtain the parent array to limit search.
    $array_parents = array();
    if (!empty($element['#array_parents'])) {
      $array_parents += $element['#array_parents'];
      // Remove the current element from the array.
      array_pop($array_parents);
    }

    // If element is nested, return the referenced parent from the form.
    if (!empty($array_parents)) {
      $parent = &drupal_array_get_nested_value($form, $array_parents);
    }
    // Otherwise return the complete form.
    else {
      $parent = &$form;
    }

    // Ignore buttons before we find the element in the form.
    $found_current_element = FALSE;
    foreach (element_children($parent) as $child) {
      if ($parent[$child] === $element) {
        $found_current_element = TRUE;
        continue;
      }

      if ($found_current_element && _bootstrap_is_button($parent[$child])) {
        _bootstrap_iconize_button($parent[$child]);
        $element['#field_suffix'] = drupal_render($parent[$child]);
        break;
      }
    }
  }

  return $element;
}

/**
 * Implements hook_form_process_HOOK().
 */
function color_glass_form_process_actions($element, &$form_state, &$form) {
  $element['#attributes']['class'][] = 'form-actions';

  if (!empty($element['#bootstrap_ignore_process'])) {
    return $element;
  }

  foreach (element_children($element) as $child) {
    _bootstrap_iconize_button($element[$child]);
  }
  return $element;
}

/**
 * Implements hook_form_process_HOOK().
 */
function color_glass_form_process_text_format($element, &$form_state, &$form) {
  if (!empty($element['#bootstrap_ignore_process'])) {
    return $element;
  }

  // Provide smart description on the value text area.
  _bootstrap_element_smart_description($element, $element['value']);

  // Allow the elements inside to be displayed inline.
  $element['format']['#attributes']['class'][] = 'form-inline';

  // Remove the cluttering guidelines; they can be viewed on a separate page.
  $element['format']['guidelines']['#access'] = FALSE;

  // Hide the select label.
  $element['format']['format']['#title_display'] = 'none';

  // Make the select element smaller using a Bootstrap class.
  $element['format']['format']['#attributes']['class'][] = 'input-sm';

  // Support the Bootstrap Select plugin if it is used.
  $element['format']['format']['#attributes']['data-style'] = 'btn-sm btn-default';

  return $element;
}

/**
 * Implements hook_theme_registry_alter().
 */
function color_glass_theme_registry_alter(&$registry) {
  // Retrieve the active theme names.
  $themes = _bootstrap_get_base_themes(NULL, TRUE);

  // Return the theme registry unaltered if it is not Bootstrap based.
  if (!in_array('color_glass', $themes)) {
    return;
  }

  // Inject the "footer" variable default in the existing "table" hook.
  // @see https://drupal.org/node/806982
  // @todo Make this discoverable in some way instead of a manual injection.
  $registry['table']['variables']['footer'] = NULL;

  // Process registered hooks in the theme registry.
  _bootstrap_process_theme_registry($registry, $themes);

  // Process registered hooks in the theme registry to add necessary theme hook
  // suggestion phased function invocations. This must be run after separately
  // and after all includes have been loaded.
  _bootstrap_process_theme_registry_suggestions($registry, $themes);

  // Post-process theme registry. This happens after all altering has occurred.
  foreach ($registry as $hook => $info) {
    // Ensure uniqueness.
    if (!empty($registry[$hook]['includes'])) {
      $registry[$hook]['includes'] = array_unique($info['includes']);
    }
    if (!empty($registry[$hook]['preprocess functions'])) {
      $registry[$hook]['preprocess functions'] = array_unique($info['preprocess functions']);
    }
    if (!empty($registry[$hook]['process functions'])) {
      $registry[$hook]['process functions'] = array_unique($info['process functions']);
    }

    // Ensure "theme path" is set.
    if (!isset($registry[$hook]['theme path'])) {
      $registry[$hook]['theme path'] = $GLOBALS['theme_path'];
    }
  }
}

/**
 * Implements hook_preprocess_admin_menu_links().
 */
function color_glass_preprocess_admin_menu_links(&$variables) {
  $elements = &$variables['elements'];
  foreach (element_children($elements) as $child) {
    $elements[$child]['#bootstrap_ignore_pre_render'] = TRUE;
    $elements[$child]['#bootstrap_ignore_process'] = TRUE;
  }
}

/**
 * Implements hook_preprocess_book_navigation().
 */
function color_glass_process_book_navigation(&$variables) {
  $variables['tree'] = _bootstrap_book_children($variables['book_link']);
}

/**
 * Implements hook_preprocess_bootstrap_carousel().
 */
function color_glass_preprocess_bootstrap_carousel(&$variables) {
  $variables['attributes']['class'][] = 'carousel';
  $variables['attributes']['class'][] = 'slide';
  $variables['attributes']['data-ride'] = 'carousel';
  if (empty($variables['attributes']['id'])) {
    $variables['attributes']['id'] = drupal_html_id('carousel');
  }
  $default_data_attributes = array(
    'interval' => 5000,
    'pause' => TRUE,
    'wrap' => TRUE,
  );
  foreach ($default_data_attributes as $name => $value) {
    if ($variables[$name] !== $value) {
      // Convert PHP booleans to the JSON equivalent, otherwise they'll be
      // interpreted as integers when they're parsed.
      if (is_bool($variables[$name])) {
        $variables[$name] = $variables[$name] ? 'true' : 'false';
      }
      $variables['attributes']['data-' . $name] = $variables[$name];
    }
  }
}

/**
 * Implements hook_process_bootstrap_carousel().
 */
function color_glass_process_bootstrap_carousel(&$variables) {
  $variables['target'] = '#' . $variables['attributes']['id'];
  $variables['attributes'] = drupal_attributes($variables['attributes']);

  // Ensure the item arrays are constructed properly for the template.
  foreach ($variables['items'] as $delta => $item) {
    // Convert items that are string into the appropriate array structure.
    if (is_string($item)) {
      $variables['items'][$delta] = array(
        'image' => $item,
      );
    }
    // Populate defaults.
    $variables['items'][$delta] += array(
      'title' => NULL,
      'description' => NULL,
      'url' => NULL,
    );
  }
}

/**
 * Renders the Bootstrap dropdown.
 */
function color_glass_bootstrap_dropdown($variables) {
  return drupal_render($variables['dropdown']);
}


/**
 * Implements hook_preprocess_bootstrap_dropdown().
 */
function color_glass_preprocess_bootstrap_dropdown(&$variables) {
  $element = &$variables['element'];

  // Provide defaults.
  $element += array(
    '#wrapper_attributes' => NULL,
    '#attributes' => NULL,
    '#alignment' => NULL,
    '#toggle' => NULL,
    '#items' => NULL,
  );

  // Dropdown vertical alignment.
  $element['#wrapper_attributes']['class'][] = 'dropdown';
  if ($element['#alignment'] === 'up' || (is_array($element['#alignment']) && in_array('up', $element['#alignment']))) {
    $element['#wrapper_attributes']['class'][] = 'dropup';
  }

  if (isset($element['#toggle'])) {
    if (is_string($element['#toggle'])) {
      $element['#toggle'] = array(
        '#theme' => 'link__bootstrap_dropdown__toggle',
        '#text' => $element['#toggle'],
        '#path' => '#',
        '#options' => array(
          'attributes' => array(),
          'html' => TRUE,
          'external' => TRUE,
        ),
      );
    }
    if (isset($element['#toggle']['#options']['attributes'])) {
      $element['#toggle']['#options']['attributes']['class'][] = 'dropdown-toggle';
      $element['#toggle']['#options']['attributes']['data-toggle'] = 'dropdown';
    }
    else {
      $element['#toggle']['#attributes']['class'][] = 'dropdown-toggle';
      $element['#toggle']['#attributes']['data-toggle'] = 'dropdown';
    }
  }

  // Menu items.
  $element['#attributes']['role'] = 'menu';
  $element['#attributes']['class'][] = 'dropdown-menu';
  if ($element['#alignment'] === 'right' || (is_array($element['#alignment']) && in_array('right', $element['#alignment']))) {
    $element['#attributes']['class'][] = 'dropdown-menu-right';
  }
}

/**
 * Implements hook_process_bootstrap_dropdown().
 */
function color_glass_process_bootstrap_dropdown(&$variables) {
  $element = &$variables['element'];

  $items = array();
  foreach ($element['#items'] as $data) {
    $item_classes = array();

    // Dividers.
    if (empty($data)) {
      $data = '';
      $item_classes[] = 'divider';
    }
    // Headers (must be a string).
    elseif (is_array($data) && (!empty($data['header']) || !empty($data['#header']))) {
      $item_classes[] = 'dropdown-header';
    }
    // Disabled.
    elseif (is_array($data) && (!empty($data['disabled']) || !empty($data['#disabled']))) {
      $item_classes[] = 'disabled';
    }

    // Construct item_list item.
    $item = array(
      'data' => render($data),
      'role' => 'presentation',
    );
    if (!empty($item_classes)) {
      $item['class'] = $item_classes;
    }
    $items[] = $item;
  }

  // Create the dropdown.
  $variables['dropdown'] = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => $element['#wrapper_attributes'],
    'toggle' => $element['#toggle'],
    'items' => array(
      '#theme' => 'item_list__bootstrap_dropdown',
      '#items' => $items,
      '#attributes' => $element['#attributes'],
    ),
  );
}

/**
 * Returns HTML for an image field widget.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: A render element representing the image field widget.
 *
 * @ingroup themeable
 */
function color_glass_image_widget(array $variables) {
  $element = $variables['element'];
  $output = '';
  $output .= '<div class="image-widget form-managed-file clearfix">';

  if (isset($element['preview'])) {
    $output .= '<div class="image-preview">';
    $output .= drupal_render($element['preview']);
    $output .= '</div>';
  }

  $output .= '<div class="image-widget-data">';
  if ($element['fid']['#value'] != 0) {
    $element['filename']['#markup'] = '<div class="form-group">' . $element['filename']['#markup'] . ' <span class="file-size badge">' . format_size($element['#file']->filesize) . '</span></div>';
  }
  else {
    $element['upload']['#prefix'] = '<div class="input-group">';
    $element['upload_button']['#prefix'] = '<span class="input-group-btn">';
    $element['upload_button']['#suffix'] = '</span></div>';
  }

  $output .= drupal_render_children($element);
  $output .= '</div>';
  $output .= '</div>';

  return $output;
}

/**
 * Implements hook_preprocess_webform_element().
 */
function color_glass_preprocess_webform_element(&$variables) {
  $element = $variables['element'];
  $wrapper_attributes = array();
  if (isset($element['#wrapper_attributes'])) {
    $wrapper_attributes = $element['#wrapper_attributes'];
  }

  // See http://getbootstrap.com/css/#forms-controls.
  if (isset($element['#type'])) {
    if ($element['#type'] === 'radio') {
      $wrapper_attributes['class'][] = 'radio';
    }
    elseif ($element['#type'] === 'checkbox') {
      $wrapper_attributes['class'][] = 'checkbox';
    }
    elseif ($element['#type'] !== 'hidden') {
      $wrapper_attributes['class'][] = 'form-group';
    }
  }

  $variables['element']['#wrapper_attributes'] = $wrapper_attributes;
}

/**
 * Overrides theme_mark().
 */
function color_glass_mark($variables) {
  global $user;
  if ($user->uid) {
    if ($variables['type'] == MARK_NEW) {
      return ' <span class="marker label label-primary">' . t('new') . '</span>';
    }
    elseif ($variables['type'] == MARK_UPDATED) {
      return ' <span class="marker label label-info">' . t('updated') . '</span>';
    }
  }
}

/**
 * Implements hook_preprocess_node().
 */
function color_glass_preprocess_node(&$variables) {
  // Update the links with wells.
  $variables['content']['links']['#attributes']['class'][] = 'well';
  $variables['content']['links']['#attributes']['class'][] = 'well-sm';
}
