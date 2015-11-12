<?php

/**
 * Batch operation to process storages managed by stream_wrapper.
 */
class StorageApiPopulateBatchOperationFilesStreamWrapper extends StorageApiPopulateBatchOperationBase implements StorageApiPopulateBatchOperationInterface {

  /**
   * @{inheritdoc}
   */
  function __construct(array &$context, $step, $stream_wrapper) {
    $this->stream_wrapper = $stream_wrapper;
    parent::__construct($context, $step);
  }

  /**
   * @{inheritdoc}
   */
  function getProgressMessage() {
    return t('Moving files to @wrapper://: @current of @total.', array(
      '@wrapper' => $this->stream_wrapper,
      '@current' => $this->context['sandbox']['current'],
      '@total'   => $this->context['sandbox']['total'],
    ));
  }

  /**
   * @{inheritdoc}
   */
  function count() {
    return db_select('file_managed', 'f')->countQuery()->execute()->fetchField();
  }

  /**
   * @{inheritdoc}
   */
  function process($current, $total) {
    $result = db_select('file_managed', 'f')
      ->fields('f', array('fid', 'uri'))
      ->orderBy('f.fid', 'ASC')
      ->range($current, $this->step)
      ->execute();
    while ($row = $result->fetchObject()) {
      // TODO: files may be in any other stream wrapper!
      if (strpos($row->uri, 'public://') === 0) {
        $file = file_load($row->fid);
        if ($file) {
          $uri = $file->uri;
          $this->context['results']['processed'][] = $uri;

          $scheme = file_uri_scheme($uri);
          $target = file_uri_target($uri);
          $storage_uri = preg_replace('/^' . $scheme . '/', $this->stream_wrapper, $uri);
          $options = array(
            'source_uri' => $uri,
            'filename' => $target,
          );
          try {
            // Add to Storage API.
            $storage = storage_stream_wrapper_selector($this->stream_wrapper)->storageAdd($options);
            db_insert('storage_stream_wrapper')
              ->fields(array(
                'storage_id' => $storage->storage_id,
                'uri' => $storage_uri,
              ))
              ->execute();

            // Update File URI.
            $file->uri = $storage_uri;
            $file = file_save($file);
          }
          catch (StorageException $e) {
            $this->context['results']['failed'][] = $uri;
            $message = 'Failed adding file @fid to storage api with exception @exception.';
            $variables = array(
              '@fid' => $file->fid,
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

