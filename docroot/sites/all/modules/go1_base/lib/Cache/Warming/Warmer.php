<?php

namespace Drupal\go1_base\Cache\Warming;

/**
 * @todo  Think about sub-processes.
 *
 * Warm cached data.
 *
 * Usage
 *
 * @code
 *   go1_container('cache.warmer')
 *     ->setEventName('user_login')
 *     ->setContext(array('entity_type' => 'user', 'entity' => $account))
 *     ->warm()
 *   ;
 * @code
 */
class Warmer {
  private $tag_discover;
  private $tag_flusher;
  private $warmers;
  private $context;
  private $event_name;

  /**
   * A process can start sub-process. This flag will avoid infinitive master
   * processes.
   *
   * @var boolean
   */
  private $is_sub_process = FALSE;

  public function __construct($tag_discover, $tag_flusher) {
    $this->tag_discover = $tag_discover;
    $this->tag_flusher = $tag_flusher;

    $this->warmers = go1_container('container')->find('cache.warmer', 'service');
  }

  public function setEventName($event_name) {
    $this->event_name = $event_name;
    $this->tag_discover->setEventName($event_name);
    return $this;
  }

  public function setIsSubProcess($is_sub_process = FALSE) {
    $this->is_sub_process = $is_sub_process;
    return $this;
  }

  public function setContext($context) {
    $this->context = $context;
    return $this;
  }

  /**
   * Wrapper function to warm cached-tags & views.
   */
  public function warm() {
    $this->tag_flusher->resetTags();

    foreach ($this->tag_discover->tags() as $tag) {
      foreach ($this->warmers as $warmer) {
        if (TRUE === $warmer->validateTag($tag)) {
          if ($tag = $warmer->processTag($tag, $this->context)) {
            $this->tag_flusher->addTag($tag);
          }
        }
      }
    }

    $this->tag_flusher->flush();
  }
}
