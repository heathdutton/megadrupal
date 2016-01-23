<?php
/**
 * @file
 * Drush file with custom commands.
 */

/**
 * Implements hook_drush_command().
 */
function max_image_size_drush_command() {

  $items['max-image-size-queue'] = array(
    'description' => dt('Add items to the image resize queue.'),
    'aliases' => array('misq'),
    'options' => array(
      'reset' => dt('Reset the queue and process table.'),
    ),
    'examples' => array(
      'drush misq' => dt('Queue items to process.'),
      'drush misq --reset' => dt('Reset the process table and queue.'),
    ),
  );

  $items['max-image-size-process'] = array(
    'description' => dt('Process items in the resize queue and table.'),
    'aliases' => array('misp'),
    'options' => array(
      'items' => dt('The number of images to process.'),
    ),
    'examples' => array(
      'drush misp' => dt('Process a batch of items.'),
      'drush misp --items=all' => dt('Process all available items.'),
      'drush misp --items=250' => dt('Processes a batch of 250 items.'),
    ),
  );

  return $items;
}

/**
 * Drush command implementation for dealing with image resizing.
 */
function drush_max_image_size_queue() {

  $reset = drush_get_option('reset', false);
  if ($reset) {

    $queue = DrupalQueue::get('max_image_size');
    $queue->deleteQueue();
    db_delete('max_image_size')->execute();

    drush_log(dt('Cleared queue and process table.'), 'ok');

    return true;
  }

  $result = max_image_size_queue_unprocessed_images();
  if ($result) {

    drush_log(dt('@count files in queue.', array('@count' => $result)), 'ok');
  }
  else {

    drush_log(dt('No new files.'), 'ok');
  }

  return true;
}

/**
 * Drush command implementation for dealing with image resizing.
 */
function drush_max_image_size_process() {

  $queue = DrupalQueue::get('max_image_size');
  $queue->createQueue();
  if (0 == $queue->numberOfItems()) {

    drush_log(dt('There are no items to process.'), 'ok');
    return true;
  }

  $limit = drush_get_option('items', variable_get('search_cron_limit', 200));
  if ('all' == $limit) {

    $limit = $queue->numberOfItems();
  }
  else if ($limit > $queue->numberOfItems()) {

    $limit = $queue->numberOfItems();
  }

  for ($i = 0; $i < $limit; $i++) {

    $item = $queue->claimItem();
    if (empty($item->data) || !is_numeric($item->data)) {

      drush_log(dt('Queue item @id did not contain a valid file id (@fid).',
        array(
          '@id' => $item->item_id,
          '@fid' => $item->data,
        )), 'error');

      continue;
    }

    drush_log(dt('Processing file id @fid (@index / @total @percent%).',
              array(
                '@fid' => $item->data,
                '@index' => $i,
                '@total' => $limit,
                '@percent' => round($i / $limit, 2) * 100,
              )), 'ok');
    max_image_size_resize_callback($item->data);
    $queue->deleteItem($item);
  }

  drush_log(dt('Processed @count items', array('@count' => $limit)), 'ok');

  return true;
}
