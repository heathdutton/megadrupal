<?php

/**
 * @file Theme functions of the CAT UI module.
 *
 * @author Oleksiy Kalinichenko <aexchecker@gmail.com>
 */

/**
 * Theme form with CAT item values.
 *
 * @param array $variables
 *          Drupal theme variables array.
 */
function theme_cat_ui_add_form($variables) {
  return _cat_render_table($variables);
}

/**
 * Theme table with CAT item path.
 *
 * @param array $variables
 *          Drupal theme variables array.
 */
function theme_cat_ui_edit_form($variables) {
  return _cat_render_table($variables);
}

/**
 * Theme table with CAT items.
 *
 * @param array $variables
 *          Drupal theme variables array.
 *
 * @return string Formated HTML table
 */
function _cat_render_table($variables) {
  $form = $variables['form'];
  $output = '';

  if (!empty($form['cat_tabs'])) {
    // REMOVE TABS
    $rows = array();
    foreach (element_children($form['cat_remove']) as $children_key) {
      $children = $form['cat_remove'][$children_key];
      if (isset($children['path']) && isset($children['delete'])) {
        $children['weight']['#attributes']['class'][] = 'path-weight';
        $rows[] = array(
          'data' => array(
            array(
              'data' => $children['path'],
            ),
            array(
              'data' => $children['remove_settings'],
            ),
            array(
              'data' => $children['weight'],
            ),
            array(
              'data' => $children['delete'],
              'class' => array(
                'cat-tab-delete',
              ),
            ),
          ),
          'class' => array(
            'draggable',
          ),
        );
      }
    }

    unset($form['cat_remove']);

    if ($rows) {
      $rows[] = array(
        'data' => array(
          array(
            'data' => render($form['cat_remove_more']),
            'colspan' => 4,
          ),
        ),
      );

      $output .= theme('table', array(
        'header' => array(
          t('Hide existing path'),
          t('Settings'),
          t('Weight'),
          array(
            'data' => t('Delete'),
            'class' => array(
              'cat-path-delete',
            ),
          ),
        ),
        'rows' => $rows,
        'attributes' => array(
          'class' => array(
            'cat-path-table',
          ),
          'id' => 'cat-remove-tabs-table',
        ),
      ));

      drupal_add_tabledrag('cat-remove-tabs-table', 'order', 'sibling', 'path-weight');
    }
    else {
      $rows[] = array(
        'data' => array(
          array(
            'data' => render($form['cat_remove_more']),
            'colspan' => 2,
          ),
        ),
      );

      $output .= theme('table', array(
        'header' => array(
          t('Hide existing path'),
          array(
            'data' => t('Delete'),
            'class' => array(
              'cat-path-delete',
            ),
          ),
        ),
        'rows' => $rows,
        'attributes' => array(
          'class' => array(
            'cat-path-table'
          ),
          'id' => 'cat-remove-tabs-table',
        ),
      ));
    }

    // NEW TABS
    $rows = array();
    foreach (element_children($form['cat_tabs']) as $children_key) {
      $children = $form['cat_tabs'][$children_key];
      if (isset($children['title']) && isset($children['path']) && isset($children['delete'])) {
        $children['weight']['#attributes']['class'][] = 'path-weight';
        $rows[] = array(
          'data' => array(
            array(
              'data' => render($children['title'])
                      . render($children['reusing']),
              'class' => array(
                'cat-tab-title',
              ),
            ),
            array(
              'data' => render($children['path'])
                      . render($children['path_query'])
                      . render($children['path_destination']),
              'class' => array(
                'cat-tab-path',
              ),
            ),
            array(
              'data' => render($children['css_id'])
                      . render($children['css_class'])
                      . render($children['active']),
              'class' => array(
                'cat-tab-css',
              ),
            ),
            array(
              'data' => $children['weight'],
            ),
            array(
              'data' => $children['delete'],
              'class' => array(
                'cat-tab-delete',
              ),
            ),
          ),
          'class' => array(
            'draggable',
          )
        );
      }
    }

    unset($form['cat_tabs']);

    if ($rows) {
      $rows[] = array(
        'data' => array(
          array(
            'data' => render($form['cat_tabs_more'])
                    . render($form['cat_tabs_reusable']),
            'colspan' => 5,
          ),
        ),
      );

      $output .= theme('table', array(
        'header' => array(
          t('Local tab name'),
          t('Path'),
          array(
            'data' => t('CSS'),
            'class' => array(
              'cat-tab-css',
            ),
          ),
          t('Weight'),
          array(
            'data' => t('Delete'),
            'class' => array(
              'cat-path-delete',
            ),
          ),
        ),
        'rows' => $rows,
        'attributes' => array(
          'class' => array(
            'cat-path-table',
          ),
          'id' => 'cat-tabs-table',
        ),
      ));

      drupal_add_tabledrag('cat-tabs-table', 'order', 'sibling', 'path-weight');
    }
    else {
      $rows[] = array(
        'data' => array(
          array(
            'data' => render($form['cat_tabs_more'])
                    . render($form['cat_tabs_reusable']),
            'colspan' => 2,
          ),
        ),
      );

      $output .= theme('table', array(
        'header' => array(
          t('Local tab name'),
          array(
            'data' => t('Delete'),
            'class' => array(
              'cat-path-delete',
            ),
          ),
        ),
        'rows' => $rows,
        'attributes' => array(
          'class' => array(
            'cat-path-table'
          ),
          'id' => 'cat-tabs-table',
        ),
      ));
    }

    $rows = array();
    foreach (element_children($form['cat_actions']) as $children_key) {
      $children = $form['cat_actions'][$children_key];
      if (isset($children['title']) && isset($children['path']) && isset($children['delete'])) {
        $children['weight']['#attributes']['class'][] = 'path-weight';
        $rows[] = array(
          'data' => array(
            array(
              'data' => $children['title'],
            ),
            array(
              'data' => render($children['path'])
                      . render($children['path_query'])
                      . render($children['path_destination']),
              'class' => array(
                'cat-tab-path',
              ),
            ),
            array(
              'data' => render($children['css_id'])
                      . render($children['css_class'])
                      . render($children['active']),
              'class' => array(
                'cat-tab-css',
              ),
            ),
            array(
              'data' => $children['weight']
            ),
            array(
              'data' => $children['delete'],
              'class' => array(
                'cat-tab-delete',
              ),
            ),
          ),
          'class' => array(
            'draggable',
          ),
        );
      }
    }

    unset($form['cat_actions']);

    if ($rows) {
      $rows[] = array(
        'data' => array(
          array(
            'data' => render($form['cat_actions_more']),
            'colspan' => 5,
          ),
        ),
      );

      $output .= theme('table', array(
        'header' => array(
          t('Local action name'),
          t('Path'),
          array(
            'data' => t('CSS'),
            'class' => array(
              'cat-tab-css',
            ),
          ),
          t('Weight'),
          array(
            'data' => t('Delete'),
            'class' => array(
              'cat-path-delete',
            ),
          ),
        ),
        'rows' => $rows,
        'attributes' => array(
          'class' => array(
            'cat-path-table',
          ),
          'id' => 'cat-actions-table',
        ),
      ));

      drupal_add_tabledrag('cat-actions-table', 'order', 'sibling', 'path-weight');
    }
    else {
      $rows[] = array(
        'data' => array(
          array(
            'data' => render($form['cat_actions_more']),
            'colspan' => 2,
          ),
        ),
      );

      $output .= theme('table', array(
        'header' => array(
          t('Local action name'),
          array(
            'data' => t('Delete'),
            'class' => array(
              'cat-path-delete',
            ),
          ),
        ),
        'rows' => $rows,
        'attributes' => array(
          'class' => array(
            'cat-path-table',
          ),
          'id' => 'cat-actions-table',
        ),
      ));
    }

    // Other form parts
    $output .= !empty($form['token_tree']) ? render($form['token_tree']) : '';
    $output .= !empty($form['custom']) ? render($form['custom']) : '';
    $output .= render($form['actions']);

    return drupal_render_children($form) . $output;
  }
}

/**
 * Theming function for cat_ui_cat_list() form.
 *
 * @param array $variables
 *          Drupal theme variables array.
 *
 * @return string Formated HTML table and form
 */
function theme_cat_ui_cat_list($variables) {
  $output = '';
  $form = $variables['form'];

  $header = array();
  $rows = array();

  $path_properties = array(
    'Title' => 'title',
    'Machine name' => 'name',
    'Category' => 'category',
    'Path' => 'path',
    'Actions' => 'actions',
    'Weight' => 'weight',
  );

  foreach ($path_properties as $path_property_title => $path_property) {
    $header[] = array(
      'data' => t($path_property_title),
      'class' => array(
        "cat-ui-table-list-{$path_property}",
      ),
    );
  }

  foreach (element_children($form) as $path_id) {
    if (isset($form[$path_id]['weight'])) {
      $row = array();

      foreach ($path_properties as $path_property) {
        $row[] = array(
          'data' => render($form[$path_id][$path_property]),
          'class' => array(
            "cat-ui-table-list-{$path_property}",
          ),
        );
      }

      $class = $form[$path_id]['disabled']['#value'] == TRUE ? 'disabled' : 'enabled';

      $rows[] = array(
        'data' => $row,
        'class' => array(
          'draggable',
          $class,
        ),
      );
    }
  }

  if (empty($rows)) {
    $nodata_string = t('There are no created %item yet.', array(
      '%item' => t('CAT items'),
    ));
    $nodata_string .= ' ' . t('You can <a href="@url">create new %item</a>.', array(
      '@url' => url('admin/structure/cat/create'),
      '%item' => t('CAT item'),
    ));
    $rows = array(
      array(
        'data' => array(
          array(
            'data' => $nodata_string,
            'colspan' => 6,
          ),
        ),
      ),
    );
  }

  if (!empty($form['#js_category'])) {
    $output .= '<div id="cat-ui-table-list-ajax">';
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(
        'id' => 'cat-ui-table-list',
      ),
    ));
    $output .= '</div>';

    $nop = drupal_render_children($form);
  }
  else {
    $output .= render($form['top row']);
    $output .= render($form['bottom row']);

    $output .= '<div id="cat-ui-table-list-ajax">';
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(
        'id' => 'cat-ui-table-list',
      ),
    ));
    $output .= '</div>';

    drupal_add_tabledrag('cat-ui-table-list', 'order', 'sibling', 'cat-ui-table-weight');

    $output .= drupal_render_children($form);
  }

  return $output;
}

/**
 * Theme the table for argument settings form.
 *
 * @param array $variables
 *          Drupal theme variables array.
 *
 * @return string Formated HTML table
 */
function theme_cat_ui_form_step_arguments_selection_table($variables) {
  $header = array(
    t('Argument'),
    t('Position in path'),
    t('Context assigned'),
    t('Operations'),
  );

  $form = $variables['form'];
  ctools_include('modal');
  ctools_modal_add_js();

  $rows = array();
  foreach (element_children($form['argument']) as $key) {
    $row = array();

    $row[] = check_plain($form['argument'][$key]['#keyword']);
    $row[] = check_plain($form['argument'][$key]['#position']);
    $row[] = $form['argument'][$key]['#context'] . ' &nbsp; ' . drupal_render($form['argument'][$key]['change']);
    $row[] = drupal_render($form['argument'][$key]['settings']) . drupal_render($form['argument'][$key]);

    $rows[] = array(
      'data' => $row,
    );
  }

  if (!$rows) {
    $rows[] = array(
      array(
        'data' => t('The path %path has no arguments to configure.', array('%path' => $form['#cat-path'])),
        'colspan' => 4,
      ),
    );
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'cat-ui-argument-table',
    ),
  ));
}

