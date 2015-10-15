<?php

/**
 * Admin UI controller class for entities using operations.
 *
 * This is optional, but tweaks Entity API's default admin UI to make it work
 * better with operations.
 */
class EntityOperationsDefaultAdminUIController extends EntityDefaultUIController {

  /**
   * The column numbers for the operation links set by the parent class.
   *
   * Available to subclasses that wish to add further operations to table rows.
   */
  protected $operation_column_numbers;

  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    // Sigh... have to get this all over again.
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';

    $operations = entity_operations_get_operation_info($this->entityType);

    // TODO: check for handlers rather than paths, since these can be custom.
    // However, the handler classes could be subclasses! And also we have two
    // classes of our own to check for 'add' and 'edit'.
    if (isset($operations['add'])) {
      // Change the admin UI 'add' menu item to go to the operation handler, so
      // it becomes effectively just a clone of the menu item for 'ENTITY/add'.

      // We have to repeat a fair bit of work from
      // EntityOperationsDefaultUIController to get the menu item for the
      // 'add' operation.
      $add_operation_handler = entity_operations_get_entity_operation_handler($this->entityType, 'add');
      // Though we give a different loader position to the operation to use for
      // its menu item.
      $operation_loader_position = count(explode('/', $this->path));
      $add_menu_item = $add_operation_handler->menu_item('add', $operations['add'], $operation_loader_position);

      // Change the page and access callbacks to those from the operation menu
      // item, while leaving it at the current path. Thus we get a clone of the
      // 'add' menu item which works correctly as a local task.
      foreach (array(
        'page callback',
        'page arguments',
        'access callback',
        'access arguments',
      ) as $menu_item_key) {
        $items[$this->path . '/add'][$menu_item_key] = $add_menu_item[$menu_item_key];
      }
    }

    if (isset($operations['edit'])) {
      unset($items[$this->path . '/manage/' . $wildcard]);
      unset($items[$this->path . '/manage/' . $wildcard . '/edit']);
    }

    // Delete uses a generic path which is also used by 'revert', so leave that.

    return $items;
  }

  /**
   * Generates the table headers for the overview table.
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    // Before we hand over to the parent, create extra columns we might need.

    // Add a type column if this entity has more than one bundle.
    if (count($this->entityInfo['bundles']) > 1) {
      $additional_header['type'] = t('Type');
    }

    $header = parent::overviewTableHeaders($conditions, $rows, $additional_header);

    return $header;
  }

  /**
   * Generates the row for the passed entity and may be overridden in order to
   * customize the rows.
   *
   * @param $additional_cols
   *   Additional columns to be added after the entity label column.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    // Before we hand over to the parent, create extra columns we might need.

    // Add a type column if this entity has more than one bundle.
    if (count($this->entityInfo['bundles']) > 1) {
      list(, , $bundle) = entity_extract_ids($this->entityType, $entity);
      $additional_cols['type'] = $this->entityInfo['bundles'][$bundle]['label'];
    }

    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);

    // Figure out which columns have the 'edit' and 'delete' links so we can
    // doctor them if needed.
    if (!isset($this->operation_column_numbers)) {
      foreach ($row as $index => $cell) {
        if ($index == 0) {
          // It's the entity label, which is not a string, so skip it.
          continue;
        }

        // Find the 'edit' operation link in the row cells. Look for the
        // terminating '"' as well so we don't match a longer URL.
        $entity_api_edit_path = $this->path . '/manage/' . $id;
        if (strpos($cell, $entity_api_edit_path . '"') !== FALSE) {
          $this->operation_column_numbers['edit'] = $index;
        }
        // Find the 'delete' operation link in the row cells. This has a
        // destination query on the URL, so match for that.
        $entity_api_delete_path = $this->path . '/manage/' . $id . '/delete';
        if (strpos($cell, $entity_api_delete_path . '?') !== FALSE) {
          $this->operation_column_numbers['delete'] = $index;
        }
      }
    }

    $operations = entity_operations_get_operation_info($this->entityType);

    $uri = $entity->uri();

    // TODO: check for handlers rather than paths, since these can be custom.
    if (isset($operations['edit'])) {
      // TODO: check for access -- though note that Entity API doesn't here.
      $row[$this->operation_column_numbers['edit']] = l('edit', $uri['path'] . '/edit');
    }
    if (isset($operations['delete'])) {
      $row[$this->operation_column_numbers['delete']] = l('delete', $uri['path'] . '/delete', array(
        'query' => drupal_get_destination(),
      ));
    }

    return $row;
  }

}
