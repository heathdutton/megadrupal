<?php
/**
 * @file
 */

class override_css_ui extends ctools_export_ui {
  /**
   * Build a row based on the item.
   *
   * By default all of the rows are placed into a table by the render
   * method, so this is building up a row suitable for theme('table').
   * This doesn't have to be true if you override both.
   */
  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    $name = $item->{$this->plugin['export']['key']};
    $schema = ctools_export_get_schema($this->plugin['schema']);

    // Note: $item->{$schema['export']['export type string']} should have already been set up by export.inc so
    // we can use it safely.
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$name] = empty($item->disabled) . $name;
        break;
      case 'title':
        $this->sorts[$name] = $item->{$this->plugin['export']['admin_title']};
        break;
      case 'name':
        $this->sorts[$name] = $name;
        break;
      case 'storage':
        $this->sorts[$name] = $item->{$schema['export']['export type string']} . $name;
        break;
    }

    $this->rows[$name]['data'] = array();
    $this->rows[$name]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');

    if ( variable_get('override_css_bundle_enabled', FALSE) ) {
      $this->rows[$name]['data'][] = array('data' => check_plain($item->bundle), 'class' => array('ctools-export-ui-bundle'));
    }

    // If we have an admin title, make it the first row.
    if (!empty($this->plugin['export']['admin_title'])) {
      $this->rows[$name]['data'][] = array('data' => check_plain($item->{$this->plugin['export']['admin_title']}), 'class' => array('ctools-export-ui-title'));
    }
    $this->rows[$name]['data'][] = array('data' => check_plain($name), 'class' => array('ctools-export-ui-name'));
    $this->rows[$name]['data'][] = array('data' => check_plain($item->selectors), 'class' => array('ctools-export-ui-selectors'));
    $this->rows[$name]['data'][] = array('data' => check_plain(implode(', ', array_filter($item->properties))), 'class' => array('ctools-export-ui-properties'));

    $operations['values'] = array('title' => t('Set values of properties'), 'href' => 'admin/appearance/override_css/list/'. $name .'/values');
    $operations['reset'] = array('title' => t('Reset values'), 'href' => 'admin/appearance/override_css/list/'. $name .'/reset');

    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));
    $this->rows[$name]['data'][] = array('data' => $ops, 'class' => array('ctools-export-ui-operations'));

    // Add an automatic mouseover of the description if one exists.
    if (!empty($this->plugin['export']['admin_description'])) {
      $this->rows[$name]['title'] = $item->{$this->plugin['export']['admin_description']};
    }
  }

  /**
   * Provide the table header.
   *
   * If you've added columns via list_build_row() but are still using a
   * table, override this method to set up the table header.
   */
  function list_table_header() {
    $header = array();

    if ( variable_get('override_css_bundle_enabled', FALSE) ) {
      $header[] = array('data' => t('Bundle'), 'class' => array('ctools-export-ui-bundle'));
    }

    if (!empty($this->plugin['export']['admin_title'])) {
      $header[] = array('data' => t('Title'), 'class' => array('ctools-export-ui-title'));
    }

    $header[] = array('data' => t('Name'), 'class' => array('ctools-export-ui-name'));
    $header[] = array('data' => t('Selectors'), 'class' => array('ctools-export-ui-selectors'));
    $header[] = array('data' => t('Properties'), 'class' => array('ctools-export-ui-properties'));
    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));

    return $header;
  }

  /**
   * Callback to enable a page.
   */
  function enable_page($js, $input, $item) {
    drupal_flush_all_caches();
    return $this->set_item_state(FALSE, $js, $input, $item);
  }

  /**
   * Callback to disable a page.
   */
  function disable_page($js, $input, $item) {
    drupal_flush_all_caches();
    return $this->set_item_state(TRUE, $js, $input, $item);
  }
}
