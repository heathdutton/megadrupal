<?php

use \Drupal\campaignion_newsletters\NewsletterList;
use \Drupal\campaignion_newsletters\QueueItem;
use \Drupal\campaignion_newsletters\ApiError;
use \Drupal\campaignion_newsletters\ApiPersistentError;

/**
 * Send items from the cron queue.
 */
function campaignion_newsletters_send_queue() {
  $lists = NewsletterList::listAll();
  $batchSize = variable_get('campaignion_newsletters_batch_size', 50);
  $items = QueueItem::claimOldest($batchSize);

  foreach ($items as $item) {
    $list = $lists[$item->list_id];
    $provider = $list->provider();

    try {
      if ($item->action & QueueItem::SUBSCRIBE) {
        $provider->subscribe($list, $item->email, $item->data, (bool) ($item->action & QueueItem::OPTIN));
      }
      elseif ($item->action == QueueItem::UNSUBSCRIBE) {
        $provider->unsubscribe($list, $item->email);
      }
      $item->delete();
    }
    catch (ApiPersistentError $e) {
      $e->log();
      // There is no point to items with persistent errors in the queue.
      $item->delete();
    }
    catch (ApiError $e) {
      $e->log();
    }
  }
}

