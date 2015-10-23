<?php

/**
 * Batch operation to process storages associated to a field via core_bridge.
 */
class StorageApiPupulateBatchOperationFieldCoreBridge extends StorageApiPopulateBatchOperationBase implements StorageApiPopulateBatchOperationInterface {

  /**
   * @{inheritdoc}
   */
  function __construct(array &$context, $step, $field_name) {
    $this->field_name = $field_name;
    parent::__construct($context, $step);
  }

  /**
   * @{inheritdoc}
   */
  function getProgressMessage() {
    return t('Moving files from @field_name field: @current of @total.', array(
      '@field_name' => $this->field_name,
      '@current'    => $this->context['sandbox']['current'],
      '@total'      => $this->context['sandbox']['total'],
    ));
  }

  /**
   * @{inheritdoc}
   */
  function count() {
    $table_name = 'field_data_' . $this->field_name;
    return db_select($table_name, 'f')->countQuery()->execute()->fetchField();
  }

  /**
   * @{inheritdoc}
   */
  function process($current, $total) {
    $table_name = 'field_data_' . $this->field_name;
    $table_field_name = $this->field_name . '_fid';
    $result = db_select($table_name, 'f')
      ->fields('f', array('entity_id', 'delta', $table_field_name))
      ->orderBy('f.entity_id', 'ASC')
      ->range($current, $this->step)
      ->execute();
    while ($row = $result->fetchObject()) {
      $file = file_load($row->$table_field_name);
      if ($file) {
        $uri = $file->uri;
        $scheme = file_uri_scheme($uri);
        $target = file_uri_target($uri);

        if (!preg_match('/^storage\-/', $scheme)) { // Check file schema is not already set to Storage.
          $this->context['results']['processed'][] = $uri;

          $storage_core_bridge_uri = preg_replace('/^' . $scheme . '/', storage_core_bridge_field_scheme($this->field_name), $uri);
          $options = array(
            'source_uri' => $uri,
            'filename' => $target,
          );
          try {
            // Add to Storage API.
            $storage = storage_core_bridge_field_selector($this->field_name)->storageAdd($options);
            db_insert('storage_core_bridge')
              ->fields(array(
                'storage_id' => $storage->storage_id,
                'uri' => $storage_core_bridge_uri,
              ))
              ->execute();

            // Update File URI.
            $file->uri = $storage_core_bridge_uri;
            $file = file_save($file);
          }
          catch (StorageException $e) {
            $this->context['results']['failed'][] = $uri;
            $message = 'Failed adding file @fid from field @field_name to storage api with exception @exception.';
            $variables = array(
              '@fid' => $file->fid,
              '@field_name' => $this->field_name,
              '@exception' => $e->getMessage(),
            );
            watchdog('storage_api_populate', $message, $variables, WATCHDOG_ERROR);
          }
        }
      }

      $this->updateContext();
    }
  }
}

