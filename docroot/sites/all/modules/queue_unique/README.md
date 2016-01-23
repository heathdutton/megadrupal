# Queue Unique
Did you ever wanted a queue that only accepts unique items? This module provides a way of doing that. If you try to insert a duplicated item in the queue, the item is ignored.

```php
// $data can be anything.
$data = array('Lorem', 'ipsum');

$queue_name = 'your_queue_name';
$queue = \DrupalQueue::get($queue_name);
$queue->createQueue();
// This puts the $data array in the queue the first time.
$queue->createItem($data);
// This will insert a duplicate, and will return FALSE.
if ($queue->createItem($data) === FALSE) {
  // The item was a duplicate, respond appropriately.
}
```

## Usage
In order for your queue to use the Queue Unique you need to set a variable:

```php
$queue_name = 'your_queue_name';
variable_set('queue_class_' . $queue_name, '\\Drupal\\queue_unique\\Queue\\SystemSetQueue');
```

## Queue UI integration
To have your queues show up in the _Queue UI_, you will need to install _Queue UI_ with the following patches:

  * https://www.drupal.org/node/2428543
  * https://www.drupal.org/node/2430287

After that, just implement `hook_queue_ui_queue_name_info`.

```php
/**
 * Implements hook_queue_ui_queue_name_info().
 */
function cache_warmer_queue_info() {
  $queue_name = 'your_queue_name';
  return array(
    $queue_name => array(
      'title' => t('Cache Warmer'),
      // Batch is optional, but nice to have.
      'batch' => array(
        'operations' => array(
          array('_custom_module_batch_process', array()),
        ),
        'finished' => '_custom_module_batch_finished',
        'title' => t('Warming Caches.'),
      ),
    ),
  );
}
```

(Bonus) If you want to have Queue UI process all the items in your queue in batches, then implement:

```php
/**
 * Processes batches of items.
 *
 * This is optional, but it's nice to be able to process your queue items with a click.
 *
 * @param \DrupalQueueInterface $queue
 *   The affected queue.
 *
 * @param array $context
 *   The batch context variable.
 */
function _custom_module_batch_process($queue, &$context) {
  $batch_size = 20;
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current'] = 0;
    $context['sandbox']['max'] = $queue->numberOfItems();
  }
  for ($i = 0; $i < $batch_size && $context['sandbox']['current'] < $context['sandbox']['max']; $i++) {
    $item = $queue->claimItem();
    if (!empty($item->data)) {
      // Rebuild the cache for the item.
      $operation = $item->data;
      // Do what you need to do with your queue item.
      _custom_module_process_item($operation);
    }
    $context['sandbox']['progress']++;
    $context['sandbox']['current']++;
    // Delete the item from the queue.
    $queue->deleteItem($item);
  }
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}
```

## Info
This module differs from <a href="drupal.org/project/unique_queue">unique_queue</a> by the fact that _Queue Unique_ (this module) does not use a specific API to ensure uniqueness. You can switch your queue over to _Queue Unique_ by just setting the variable to the `\Drupal\queue_unique\Queue\SystemSetQueue` class.
