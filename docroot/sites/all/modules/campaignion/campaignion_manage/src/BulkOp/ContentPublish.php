<?php

namespace Drupal\campaignion_manage\BulkOp;

class ContentPublish {
  public function __construct() {
  }
  public function title() { return t('Publish'); }
  public function helpText() {
    return t('Publishing your content will make it visible to the users of your site. Usually this includes all visitors of your site, but this depends on your permission settings.');
  }
  public function formElement(&$element, &$form_state) {
    $element['warn']['#type'] = 'container';
    $element['warn']['#attributes']['class'][] = 'warn';
    $message = <<<STR
You're about to publish !count pages. Publishing them will make them visible
for almost all visitors of your website. Are you sure?
STR;
    $element['warn']['message']['#markup'] = t($message, array('!count' => '<span class="bulkop-count"></span>'));
  }
  public function apply($nids, $values) {
    $nodes = node_load_multiple($nids);
    foreach ($nodes as $node) {
      if (!$node->status) {
        node_publish_action($node);
        node_save($node);
      }
    }
  }
}
