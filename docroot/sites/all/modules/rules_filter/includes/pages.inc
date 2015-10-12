<?php

/**
 * @file
 * Page elements for the module.
 */

/**
 * Returns filter form elements.
 */
function _rules_filter_filter_form($is_rules_page) {
  $form = array();

  // Adds wrapper container.
  $form['filter'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'rules-filter-form',
      ),
    ),
  );

  // Adds search field.
  $form['filter']['search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#size' => 45,
    '#attributes' => array(
      'class' => array(
        'search',
      ),
      'placeholder' => t('Search'),
    ),
    '#title_display' => 'invisible',
  );

  // Adds enabled/disabled checkboxes only to Rules page.
  if ($is_rules_page) {
    $form['filter']['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => TRUE,
      '#attributes' => array(
        'class' => array(
          'rule-enabled',
        ),
        'checked' => 'checked',
      ),
    );
    $form['filter']['disabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Disabled'),
      '#default_value' => FALSE,
      '#attributes' => array(
        'class' => array(
          'rule-disabled',
        ),
      ),
    );
  }
  return $form;
}

/**
 * Returns Tabs and Rules table.
 *
 * @param bool $is_rules_page
 *   TRUE if it is Rules page.
 *
 * @return array
 *   Contains:
 *    -table: rules table.
 *    -tabs: tabs.
 */
function _rules_filter_rules_table_components($is_rules_page) {
  // Prepare conditions and options for Rules and Components pages.
  if ($is_rules_page) {
    $conditions = array('plugin' => 'reaction rule');
    $options = array('show plugin' => FALSE);
  }
  else {
    $conditions = array('plugin' => array_keys(rules_filter_array(rules_fetch_data('plugin_info'), 'component', TRUE)));
    $options = array('hide status op' => TRUE);
  }

  // Get Rules or Components table.
  $rules_table = rules_ui()->overviewTable($conditions, $options);

  // Adds class if table is empty.
  if (empty($rules_table['#rows'])) {
    $rules_table['#attributes']['class'][] = 'empty';
  }

  // Default tabs.
  $tabs = array(
    _rules_filter_tabs_link(t('All'), 'all'),
    _rules_filter_tabs_link(t('No tags'), 'no-tags'),
  );

  // Prepare tabs and add necessary classes to table rows.
  foreach ($rules_table['#rows'] as &$row) {
    // Wrap second column to span with class for search.
    $row[1] = '<span class="search-field">' . $row[1] . '</span>';

    // Check if rule enabled or disabled (only for rules page).
    if ($is_rules_page) {
      $enabled_class = (strpos($row[4], 'enable') === FALSE) ? 'enabled' : 'disabled';
    }

    // Replace operation links to dropdowns menu if ctools module exists.
    if (module_exists('ctools')) {
      $operations = array();
      $count = count($row);
      for ($ind = 3; $ind < $count; $ind++) {
        if (!empty($row[$ind])) {
          $operations[] = $row[$ind];
        }
        unset($row[$ind]);
      }
      $row[3] = _rules_filter_operations_dropdown($operations);
    }

    // Converts rows format.
    $row = array('data' => $row);

    $classes = array('all');
    if (!empty($row['data'][0]['data']['description']['tags']['tags']['#markup'])) {

      $tags = explode(', ', $row['data'][0]['data']['description']['tags']['tags']['#markup']);
      foreach ($tags as $tag) {
        // Convert Tag to class name.
        $class = preg_replace("![^a-z0-9]+!i", "-", strtolower($tag));

        // Generate hash if class is cyrillic or any problems with converting.
        if ($class == '-') {
          $class = substr(md5($class), 0, 8);
        }
        $classes[] = $class;

        // Prepare links for tabs.
        $tabs[$tag] = _rules_filter_tabs_link($tag, $class);
      }
    }
    else {
      $classes[] = 'no-tags';
    }

    // Adds enabled/disabled class if its rules page.
    if ($is_rules_page) {
      $classes[] = $enabled_class;
    }

    $row['class'] = $classes;
  }

  // Attach css and js files.
  _rules_filter_form_attach_styles($rules_table);

  return array(
    'table' => $rules_table,
    'tabs' => array('#markup' => theme('item_list', array('items' => $tabs))),
  );
}

/**
 * Attach css and js files to element.
 */
function _rules_filter_form_attach_styles(&$element) {
  $path = drupal_get_path('module', 'rules_filter');
  $element['#attached']['js'][] = $path . '/js/rules_filter.js';
  $element['#attached']['js'][] = $path . '/js/list.min.js';
  $element['#attached']['css'][] = $path . '/css/rules_filter.css';
}

/**
 * Returns link with hash url.
 *
 * @param string $title
 *   Link title.
 * @param string $fragment
 *   Link hash query.
 *
 * @return string
 *   Rendered link.
 */
function _rules_filter_tabs_link($title, $fragment) {
  return l($title, current_path(), array('fragment' => $fragment, 'attributes' => array('rel' => $fragment)));
}

/**
 * Output given links as ctools dropdown menu.
 *
 * Note: we don't use native ctools theme function
 * because it needs not rendered links as input params,
 * but in given rules table we have only already rendered links.
 *
 * @param array $links
 *   An array of links.
 *
 * @return string
 *   HTML of dropdown menu.
 */
function _rules_filter_operations_dropdown(array $links) {

  // Add needed files.
  if (count($links) > 1) {
    ctools_add_js('dropbutton');
    ctools_add_css('dropbutton');
  }
  ctools_add_css('button');

  // Provide a unique identifier for every button on the page.
  static $id = 0;
  $id++;

  $output = '<div class="ctools-dropbutton ctools-button" id="ctools-button-' . $id . '">';

  // Open link.
  $output .= '<div class="ctools-link"><a href="#" class="ctools-twisty ctools-text">' . t('open') . '</a></div>';

  // The button content.
  $output .= '<div class="ctools-content">' . theme('item_list', array('items' => $links)) . '</div>';

  // Close ctools-dropbutton.
  $output .= '</div>';

  return $output;
}
