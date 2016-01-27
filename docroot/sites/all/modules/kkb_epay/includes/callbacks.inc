<?php

/**
 * @file
 * Menu callbacks.
 *
 * Copyright (C) 2012, Victor Nikulshin
 *
 * This file is part of KKB Epay.
 *
 * KKB Epay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * KKB Epay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KKB Epay.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Menu page callback. User landing page, payment was successful.
 */
function kkb_epay_callback_back_success($id) {
  $page = module_invoke_all('kkb_epay_success_page', $id);
  if (empty($page)) {
    $page['message'] = array(
      '#prefix' => '<p><strong>',
      '#markup' => t('Thank you, your payment was successfully accepted.'),
      '#suffix' => '</strong></p>',
    );
    $page['return'] = array(
      '#prefix' => '<p>',
      '#markup' => l(t('Back to front page.'), '<front>'),
      '#suffix' => '</p>',
    );
  }
  return $page;
}

/**
 * Menu page callback. User landing page, payment failed.
 */
function kkb_epay_callback_back_failure($id) {
  $page = module_invoke_all('kkb_epay_failure_page', $id);
  if (empty($page)) {
    $page['message'] = array(
      '#prefix' => '<p><strong>',
      '#markup' => t('Unfortunately your payment was not completed.'),
      '#suffix' => '</strong></p>',
    );
    $page['return'] = array(
      '#prefix' => '<p>',
      '#markup' => l(t('Back to front page.'), '<front>'),
      '#suffix' => '</p>',
    );
  }
  return $page;
}

/**
 * Menu page callback. Handles success response from the processing center.
 */
function kkb_epay_callback_post_success() {
  module_load_include('inc', 'kkb_epay', 'includes/billing');
  if (empty($_POST['response'])) {
    echo '0';
    drupal_exit();
  }
  $document = substr($_POST['response'], 0, 5000);

  $record = (object) array(
    'posted_timestamp' => REQUEST_TIME,
    'remote_ip' => ip_address(),
    'document' => $document
  );
  drupal_write_record('kkb_epay_raw_success_posts', $record);

  if (kkb_epay_bank_response_document_validate($document)) {
    $notification = kkb_epay_bank_response_document_parse($document);

    // TODO
    // Use 'kkb_epay_validated_documents' table to add protection from replay
    // attacks.

    $record = (object) array(
      'order_id' => $notification->getOrderId(),
      'timestamp' => $notification->getTimestamp(),
      'amount' => $notification->getAmount(),
      'reference' => $notification->getReference(),
      'approval_code' => $notification->getApprovalCode(),
    );
    drupal_write_record('kkb_epay_validated_documents', $record);

    module_invoke_all('kkb_epay_raw_success_response', $document);
    module_invoke_all('kkb_epay_payment', $notification);
  }
  echo '0';
}

/**
 * Menu page callback. Handles failure response from the processing center.
 *
 * @todo
 *   Callback must be implemented once the processing center actually starts to
 *   send failure responses. As for now it is completely unused.
 */
function kkb_epay_callback_post_failure() {
  module_load_include('inc', 'kkb_epay', 'includes/billing');
  echo '0';
}
