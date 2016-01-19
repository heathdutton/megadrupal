<?php

/**
 * @file
 * Feedback entity and controller classes.
 */

class FeedbackRController extends EntityAPIController {

  /**
   * Add properties as pseudo-fields.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $user = user_load($entity->uid);
    $status_options = array(
    '0' => t('New'),
    '1' => t('Processed'),
    );
    $content['created By'] = array('#markup' => t('<b>Submitted by: </b><a href="!link">!user</a><br>', array('!user' => $user->name, '!link' => 'user/'.$user->uid)));
    $content['created on'] = array('#markup' => t('<b>Date of submission:</b> !created<br>', array('!created' => format_date($entity->timestamp))));
    $content['status'] = array('#markup' => t('<b>Status:</b> !status<br>', array('!status' => $status_options[$entity->status])));
    $content['location'] = array('#markup' => t('<b>Location:</b> <a href="!location">!location</a><br>', array('!location' => $entity->url)));
    $content['message'] = array('#markup' => t('<b>Problem Description:</b><br> !message<br>', array('!message' => check_plain($entity->message))));
    $content['useragent'] = array('#markup' => t('<b>Useragent:</b><br> !useragent<br>', array('!useragent' => $entity->useragent)));
    $content['screenshot'] = array('#markup' => t('<b>Screenshot (click on image to see in full size):</b> <br><a href="!link"><img style="width: 100%; height: 50%;" title="Click to see full size image" src="!link"></a>', array('!link' => $entity->screenshoturl)));
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}

class FeedbackRTypeController extends EntityAPIControllerExportable {
}



/**
 * FeedbackR class.
 */
class FeedbackR extends Entity {

  public function __construct($values = array()) {
    if (isset($values['type']) && is_object($values['type'])) {
      $values['type'] = $values['type']->name;
    }
    parent::__construct($values, 'feedback_reloaded');
  }

  protected function defaultLabel() {
    $username = isset($this->uid) ? user_load($this->uid)->name : 'username';
    $type_label = feedback_reloaded_get_types($this->type)->getTranslation('label');
    if(!empty($this->title))
      return $this->title;
    else
      return t('@type Feedback by @username', array('@type' => $type_label, '@username' => $username));
  }

  protected function defaultUri() {
    return array('path' => 'feedback/' . $this->identifier());
  }
}

/**
 * FeedbackRType class.
 */
class FeedbackRType extends Entity {
  public $name;
  public $label;
  public $weight = 0;

  public function __construct($values = array()) {
    parent::__construct($values, 'feedback_reloaded_type');
  }

  /**
   * Returns whether the feedback type is locked and thus may not be deleted or renamed.
   *
   * Feedbacl types provided in code are automatically treated as locked, as well
   * as any fixed subscription type.
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }

}
