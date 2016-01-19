<?php
/**
 * @file
 * Subs entity and controller classes.
 */

/**
 * The class used for subscription entities.
 */
class Subs extends Entity {

  /**
   * The subscription id.
   *
   * @var integer
   */
  public $sid;

  /**
   * The name of the subscription type.
   *
   * @var string
   */
  public $type;

  /**
   * The user id of the applicant.
   *
   * @var integer
   */
  public $uid;

  /**
   * The Unix timestamp when the subscription was created.
   *
   * @var integer
   */
  public $created;

  /**
   * The Unix timestamp when the subscription was most recently saved.
   *
   * @var integer
   */
  public $changed;

  /**
   * The Unix timestamp when the subscription started.
   *
   * @var integer
   */
  public $date_start;

  /**
   * The Unix timestamp when the subscription will end.
   *
   * @var integer
   */
  public $date_end;

  /**
   * The subscription status.
   *
   * @var boolean
   */
  public $status;

  public function __construct($values = array()) {
    if (isset($values['type'])) {
      if ($values['type'] instanceof SubsType) {
        $values['type'] = $values['type']->name;
      }
      else {
        $values['type'] = $values['type'];
      }
    }

    parent::__construct($values, 'subs');
  }

  /**
   * Returns the full url() for the subscription.
   */
  public function url() {
    $uri = $this->uri();
    return url($uri['path'], $uri);
  }

  /**
   * Returns the drupal path to this subscription.
   */
  public function path() {
    $uri = $this->uri();
    return $uri['path'];
  }

  protected function defaultUri() {
    return array(
      'path' => 'subscription/' . $this->sid,
    );
  }

  protected function defaultLabel() {
    $username = isset($this->uid) ? user_load($this->uid)->name : 'username';
    $type_label = subs_get_types($this->type)->getTranslation('label');
    return t('@type subscription by @username', array('@type' => $type_label, '@username' => $username));
  }

  public function save() {
    // Care about setting created and changed values. But do not automatically
    // set a created values for already existing subscriptions.
    if (empty($this->created)) {
      $this->created = REQUEST_TIME;
    }
    $this->changed = REQUEST_TIME;

    // Deal with user IDs properly.
    if (!is_numeric($this->uid)) {
      if ($account = user_load_by_name($this->uid)) {
        $this->uid = $account->uid;
      }
    }

    if ($return = parent::save()) {
      // Update or set the static cache from subs_load_by_user().
      $cache = &drupal_static('subs_load_by_user', array());
      $cache[$this->uid][] = $this->sid;
      return $return;
    }

    return FALSE;
  }

}

/**
 * Use a separate class for subscription types so we can specify some defaults
 * modules may alter.
 */
class SubsType extends Entity {

  public $name;
  public $label;
  public $weight = 0;

  public function __construct($values = array()) {
    parent::__construct($values, 'subs_type');
  }

  /**
   * Returns whether the subscription type is locked and thus may not be deleted or renamed.
   *
   * Subscription types provided in code are automatically treated as locked, as well
   * as any fixed subscription type.
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }

}

/**
 * The controller class for subscription entities.
 */
class SubsController extends EntityAPIController {

  /**
   * Add properties as pseudo-fields.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    // @TODO allow different formatters for date_start, date_end and uid.
    // Make User property themed like a default field.
    $user = user_load($entity->uid);
    $content['uid'] = array(
      '#theme' => 'field',
      '#title' => t('User'),
      '#access' => TRUE,
      '#label_display' => variable_get('subs_uid_label', 'inline'),
      '#view_mode' => $view_mode,
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'subs',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->uid)),
      '#formatter' => 'text_default',
      0 => array('#markup' => l($user->name, 'user/' . $entity->uid)),
    );

    // Make Start Date property themed like a default field.
    $date_start = empty($entity->date_start) ? t('None') : format_date($entity->date_start, 'subs');
    $content['date_start'] = array(
      '#theme' => 'field',
      '#title' => t('Start Date'),
      '#access' => TRUE,
      '#label_display' => variable_get('subs_date_start_label', 'inline'),
      '#view_mode' => $view_mode,
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'subs',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->date_start)),
      '#formatter' => 'text_default',
      0 => array('#markup' => $date_start),
    );

    // Make End Date property themed like a default field.
    $date_end = empty($entity->date_end) ? t('None') : format_date($entity->date_end, 'subs');
    $content['date_end'] = array(
      '#theme' => 'field',
      '#title' => t('End Date'),
      '#access' => TRUE,
      '#label_display' => variable_get('subs_date_end_label', 'inline'),
      '#view_mode' => $view_mode,
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'subs',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->date_end)),
      '#formatter' => 'text_default',
      0 => array('#markup' => $date_end),
    );

    // Make Status property themed like a default field.
    $status_options = subs_status_options_list();
    $content['status'] = array(
      '#theme' => 'field',
      '#title' => t('Status'),
      '#access' => TRUE,
      '#label_display' => variable_get('subs_status_label', 'inline'),
      '#view_mode' => $view_mode,
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'subs',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->status)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($status_options[$entity->status])),
    );

    // Make Type property themed like a default field.
    $content['type'] = array(
      '#theme' => 'field',
      '#title' => t('Type'),
      '#access' => TRUE,
      '#label_display' => variable_get('subs_type_label', 'inline'),
      '#view_mode' => $view_mode,
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'subs',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->type)),
      '#formatter' => 'text_default',
      0 => array('#markup' => subs_get_types($entity->type)->getTranslation('label')),
    );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }

}

/**
 * The controller class for subscription type entities.
 */
class SubsTypeController extends EntityAPIControllerExportable {

  /**
   * Implements EntityAPIControllerInterface.
   * This is overriden to allow for our custom delete logic, which cancels
   * and deletes all subscriptions of the deleted type and triggers a menu
   * rebuild.
   *
   * @param $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    foreach ($ids as $type) {
      // Find subscriptions of this type.
      foreach (subs_find($type) as $subscription) {
        // If subscription is in force, cancel it.
        if (subs_in_force($subscription)) {
          subs_set_cancelled($subscription);
        }

        // Delete the subscription.
        subs_delete($subscription);
      }
    }

    // Do not directly issue menu rebuilds here to avoid potentially multiple
    // rebuilds. Instead, let menu_get_item() issue the rebuild on the next page.
    variable_set('menu_rebuild_needed', TRUE);

    // Let the parent class take it from here.
    parent::delete($ids, $transaction);
  }

}
