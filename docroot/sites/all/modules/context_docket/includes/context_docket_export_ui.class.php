<?php

/**
 * @file
 * Contains class for context docket UI.
 */

include_once drupal_get_path('module', 'context_ui') . '/export_ui/context_export_ui.class.php';

/**
 * Context docket export UI class.
 */
class context_docket_export_ui extends context_export_ui {
  /**
   * {@inheritdoc}
   */
  function list_css() {
    parent::list_css();
    $path = drupal_get_path('module', 'context_docket');
    drupal_add_css($path . '/css/context_docket.css');
    drupal_add_js($path . '/js/context_docket.js');
  }

  /**
   * {@inheritdoc}
   */
  function list_render(&$form_state) {
    $tabs = array(
      _context_docket_tabs_link(t('All'), 'all', count($this->rows)),
      _context_docket_tabs_link(t('No tags'), CONTEXT_DOCKET_NO_TAGS, _context_docket_get_count($this->rows, CONTEXT_DOCKET_NO_TAGS)),
    );

    $all_tags = array();
    foreach ($form_state['object']->items as $item) {
      if (!empty($item->tag)) {
        $all_tags[] = $item->tag;
        if (array_key_exists($item->tag . ':' . $item->name, $this->rows)) {
          $tabs[$item->tag] = _context_docket_tabs_link($item->tag, $item->tag, _context_docket_get_count($this->rows, $item->tag));
        }
      }
    }

    $table = array(
      'header' => $this->list_table_header(),
      'rows' => $this->rows,
      'empty' => $this->plugin['strings']['message']['no items'],
    );

    return theme('context_docket',
      array(
        'tabs' => array('#markup' => theme('item_list', array('items' => $tabs))),
        'table' => array('#markup' => theme('table', $table)),
      )
    );
  }

  /**
   * {@inheritdoc}
   */
  function list_table_header() {
    $header = array();
    $header[] = array('data' => t('Name'), 'class' => array('ctools-export-ui-name'));
    $header[] = array('data' => t('Description'), 'class' => array('ctools-export-ui-description'));
    $header[] = array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));

    return $header;
  }

  /**
   * {@inheritdoc}
   */
  function list_build_row($item, &$form_state, $operations) {
    $name = $item->name;
    $tag = !empty($item->tag) ? $item->tag : CONTEXT_DOCKET_NO_TAGS;
    $key = $tag . ':' . $name;

    // Build row for each context item.
    $this->rows[]['data'] = array();
    $this->rows[$key]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');
    $this->rows[$key]['data-tag'] = $tag;

    $name_markup = check_plain($name);
    if ($tag != CONTEXT_DOCKET_NO_TAGS) {
      $name_markup .= "<div class='description'>" . t('Tag') . ': ' . check_plain($tag) . "</div>";
    }

    $this->rows[$key]['data'][] = array(
      'data' => $name_markup,
      'class' => array('ctools-export-ui-name'),
    );

    $this->rows[$key]['data'][] = array(
      'data' => "<div class='description'>" . check_plain($item->description) . "</div>",
      'class' => array('ctools-export-ui-description'),
    );

    $this->rows[$key]['data'][] = array(
      'data' => check_plain($item->type),
      'class' => array('ctools-export-ui-storage'),
    );

    $this->rows[$key]['data'][] = array(
      'data' => theme('links', array(
        'links' => $operations,
        'attributes' => array('class' => array('links inline')),
      )),
      'class' => array('ctools-export-ui-operations'),
    );

    $this->sorts[$key] = $tag . $name;
  }
}
