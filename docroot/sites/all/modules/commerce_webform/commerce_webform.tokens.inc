<?php

/**
 * @file
 * Token replacements for the commerce_webform module.
 */

/**
 * Implements hook_token_info().
 */
function commerce_webform_token_info() {
  return array(
    'tokens' => array(
      'commerce-order' => array(
        'commerce-webform-submission' => array(
          'name' => t('Commerce webform submission'),
          'description' => t('A detailed listing of the webform submissions which generated this order.'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_token().
 */
function commerce_webform_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'commerce-order' && !empty($data['commerce-order'])) {
    $order = $data['commerce-order'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'commerce-webform-submission' :
          $submissions = array();
          $details = '<p>';
          foreach (entity_metadata_wrapper('commerce_order', $order)->commerce_line_items->value() as $line_item) {
            if ($line_item->type == 'commerce_webform') {
              $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
              $sid = $wrapper->commerce_webform_sid->value();
              $nid = $wrapper->commerce_webform_nid->value();
              if (!isset($submissions["$nid:$sid"])) {
                module_load_include('inc', 'webform', 'includes/webform.submissions');
                $sub = webform_get_submission($nid, $sid);
                $submissions["$nid:$sid"] = array('node' => node_load($nid), 'submission' => $sub);
              }
            }
          }

          foreach ($submissions as $id => $submission) {
            $node = $submission['node'];
            $sub = $submission['submission'];
            $details .= webform_submission_render($node, $sub, NULL, 'html') . "\r\n";
          }

          $details .= "</p>\r\n";
          $replacements[$original] = $details;
          break;
      }
    }
  }

  return $replacements;
}
