<?php

/**
 * This class overrides methods in sites/all/modules/ctools/plugins/export_ui/ctools_export_ui.class.php
 * Changes have been made to change the machine name column to a more human friendly title.
 * Also adding a description column to the listing of term queues
 **/
class term_queue_export_ui extends ctools_export_ui {

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
    $title = $item->title;
    $description = $item->description;
    $edit_terms = 'admin/structure/term_queue/'.$item->machine_name;
    $schema = ctools_export_get_schema($this->plugin['schema']);

    // Note: $item->{$schema['export']['export type string']} should have already been set up by export.inc so
    // we can use it safely.
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$name] = empty($item->disabled) . $title;
        break;
      case 'title':
        $this->sorts[$name] = $title;
        break;
      case 'description':
        $this->sorts[$name] = $description;
        break;
      case 'storage':
        $this->sorts[$name] = $item->{$schema['export']['export type string']} . $name;
        break;
    }

    $this->rows[$name]['data'] = array();
    $this->rows[$name]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');

    // If we have an admin title, make it the first row.
    if (!empty($this->plugin['export']['admin_title'])) {
      $this->rows[$name]['data'][] = array('data' => check_plain($item->{$this->plugin['export']['admin_title']}), 'class' => array('ctools-export-ui-title'));
    }
    //Changing 'name' to title
    $this->rows[$name]['data'][] = array('data' => check_plain($title), 'class' => array('ctools-export-ui-description'));
    //Additional description column
    $this->rows[$name]['data'][] = array('data' => check_plain($description), 'class' => array('ctools-export-ui-description'));
    $this->rows[$name]['data'][] = array('data' => check_plain($item->{$schema['export']['export type string']}), 'class' => array('ctools-export-ui-storage'));

    $this->rows[$name]['data'][] = array('data' => l(t('Edit Terms'), $edit_terms, array('class' => array('ctools-export-ui-edit-terms'))));

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

    $header[] = array('data' => t('Title'), 'class' => array('ctools-export-ui-title'));
    //Additional description header. Ed Crompton.
    $header[] = array('data' => t('Description'), 'class' => array('ctools-export-ui-description'));
    $header[] = array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'), 'colspan' => 2);
    //$header[] = array('data' => t())
    return $header;
  }
  
  
  /**
   * Provide a list of sort options.
   *
   * Override this if you wish to provide more or change how these work.
   * The actual handling of the sorting will happen in build_row().
   */
  function list_sort_options() {

    $options = array(
      'disabled' => t('Enabled, title'),
      'title' => t('Title'),
      'description' => t('Description'),
      'storage' => t('Storage'),
    );
    
    return $options;
  }
  
  
  /**
   * Provide a list of fields to test against for the default "search" widget.
   *
   * This widget will search against whatever fields are configured here. By
   * default it will attempt to search against the name, title and description fields.
   */
  function list_search_fields() {
    $fields = array (
	'title',
	'description'
    );
    return $fields;
  }
  
  
  
  /**
   * Submit the filter/sort form.
   *
   * This submit handler is actually responsible for building up all of the
   * rows that will later be rendered, since it is doing the filtering and
   * sorting.
   *
   * For the most part, you should not need to override this method, as the
   * fiddly bits call through to other functions.
   */
  function list_form_submit(&$form, &$form_state) {
    // Filter and re-sort the pages. 
    $plugin = $this->plugin;
    
    $schema = ctools_export_get_schema($this->plugin['schema']);

    $prefix = ctools_export_ui_plugin_base_path($plugin);

    foreach ($this->items as $name => $item) {
      // Call through to the filter and see if we're going to render this
      // row. If it returns TRUE, then this row is filtered out.
      if ($this->list_filter($form_state, $item)) {
        continue;
      }

      // Note: Creating this list seems a little clumsy, but can't think of
      // better ways to do this.
      $allowed_operations = drupal_map_assoc(array_keys($plugin['allowed operations']));
      
      //Cannot find the code that returns the list of possible operations, so I'm going to add extras here, which is not neat.
      //$allowed_operations['edit terms'] = 'edit terms';
      
      $not_allowed_operations = array('import');

      if ($item->{$schema['export']['export type string']} == t('Normal')) {
        $not_allowed_operations[] = 'revert';
      }
      elseif ($item->{$schema['export']['export type string']} == t('Overridden')) {
        $not_allowed_operations[] = 'delete';
      }
      else {
        $not_allowed_operations[] = 'revert';
        $not_allowed_operations[] = 'delete';
      }

      //$not_allowed_operations[] = empty($item->disabled) ? 'enable' : 'disable';
      
      //Forcing 'disable' and 'enable' to be disabled always. This should be done in the install schema with 'can disable' => FALSE, but that would
      //mean reinstalling the module and I don't think it works properly.
      $not_allowed_operations[] = 'disable';
      $not_allowed_operations[] = 'enable';


      foreach ($not_allowed_operations as $op) {
        // Remove the operations that are not allowed for the specific
        // exportable.
        unset($allowed_operations[$op]);
      }

      $operations = array();

      foreach ($allowed_operations as $op) {
        $operations[$op] = array(
          'title' => $plugin['allowed operations'][$op]['title'],
          'href' => ctools_export_ui_plugin_menu_path($plugin, $op, $name),
        );
        if (!empty($plugin['allowed operations'][$op]['ajax'])) {
          $operations[$op]['attributes'] = array('class' => array('use-ajax'));
        }
        if (!empty($plugin['allowed operations'][$op]['token'])) {
          $operations[$op]['query'] = array('token' => drupal_get_token($op));
        }
      }

      $this->list_build_row($item, $form_state, $operations);
    }

    // Now actually sort
    if ($form_state['values']['sort'] == 'desc') {
      arsort($this->sorts);
    }
    else {
      asort($this->sorts);
    }

    // Nuke the original.
    $rows = $this->rows;
    $this->rows = array();
    // And restore.
    foreach ($this->sorts as $name => $title) {
      $this->rows[$name] = $rows[$name];
    }
  }
  
  
}
