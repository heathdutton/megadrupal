<?php

/**
 * @file
 * Contains class for views docket UI.
 */

include_once drupal_get_path('module', 'views') . '/plugins/export_ui/views_ui.class.php';

/**
 * Views docket export UI class.
 */
class views_docket_export_ui extends views_ui {
  /**
   * {@inheritdoc}
   */
  function list_css() {
    parent::list_css();
    $path = drupal_get_path('module', 'views_docket');
    drupal_add_css($path . '/css/views_docket.css');
    drupal_add_js($path . '/js/views_docket.js');
  }
  /**
   * {@inheritdoc}
   */
  function list_build_row($view, &$form_state, $operations) {
    parent::list_build_row($view, $form_state, $operations);
    if (!empty($view->tag)) {
      $tags = explode(',', $view->tag);
      foreach ($tags as $tag) {
        $tag_class = str_replace(' ', '-', trim($tag));
        $this->rows[$view->name]['class'][] = 'views-docket--' . $tag_class;
        $this->rows[$view->name]['tags'][] = trim($tag);
      }
    }
    else {
      $this->rows[$view->name]['class'][] = 'views-docket--' . VIEWS_DOCKET_NO_TAGS;
      $this->rows[$view->name]['tags'][] = VIEWS_DOCKET_NO_TAGS;
    }
  }

  /**
   * {@inheritdoc}
   */
  function list_render(&$form_state) {
    $tabs = array(
      'all' => _views_docket_tabs_link(t('All'), 'all', count($this->rows)),
      VIEWS_DOCKET_NO_TAGS => _views_docket_tabs_link(t('No tags'), VIEWS_DOCKET_NO_TAGS, _views_docket_get_count($this->rows, VIEWS_DOCKET_NO_TAGS)),
      'default' => _views_docket_tabs_link(t('Default'), 'default', _views_docket_get_count($this->rows, 'default')),
    );

    foreach ($this->rows as $row) {
      if (!empty($row['tags'])) {
        foreach ($row['tags'] as $tag) {
          if (!array_key_exists($tag, $tabs)) {
            $tabs[$tag] = _views_docket_tabs_link($tag, $tag, _views_docket_get_count($this->rows, $tag));
          }
        }
      }
    }

    views_include('admin');
    views_ui_add_admin_css();
    if (empty($_REQUEST['js'])) {
      views_ui_check_advanced_help();
    }
    drupal_add_library('system', 'jquery.bbq');
    views_add_js('views-list');

    $this->active = $form_state['values']['order'];
    $this->order = $form_state['values']['sort'];

    $query = tablesort_get_query_parameters();

    $header = array(
      $this->tablesort_link(t('View name'), 'name', 'views-ui-name'),
      array('data' => t('Description'), 'class' => array('views-ui-description')),
      $this->tablesort_link(t('Tag'), 'tag', 'views-ui-tag'),
      $this->tablesort_link(t('Path'), 'path', 'views-ui-path'),
      array('data' => t('Operations'), 'class' => array('views-ui-operations')),
    );

    $table = array(
      'header' => $header,
      'rows' => $this->rows,
      'empty' => t('No views match the search criteria.'),
      'attributes' => array('id' => 'ctools-export-ui-list-items'),
    );

    return theme('views_docket',
      array(
        'tabs' => array('#markup' => theme('item_list', array('items' => $tabs))),
        'table' => array('#markup' => theme('table', $table)),
      )
    );
  }
}
