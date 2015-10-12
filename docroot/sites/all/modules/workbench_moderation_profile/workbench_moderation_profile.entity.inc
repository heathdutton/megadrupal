<?php

/**
 * @file
 * Classes for the Profiles.
 */

/**
 * The class used for workbench_moderation_profile entities
 */
class WorkbenchModerationProfile extends Entity {
  /**
   * Construct the entity.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'workbench_moderation_profile');
  }

  /**
   * Return a default object URI.
   */
  protected function defaultUri() {
    return array('path' => 'workbench_moderation_profile/' . $this->wmpid);
  }
}

/**
 * The Controller for WorkbenchModerationProfile entities
 */
class WorkbenchModerationProfileController extends EntityAPIControllerExportable {

  /**
   * Export workbench moderation profile data.
   */
  public function export($entity, $prefix = '') {
    if (!empty($entity->field_transitions[LANGUAGE_NONE]) && is_array($entity->field_transitions[LANGUAGE_NONE])) {
      foreach ($entity->field_transitions[LANGUAGE_NONE] as $key => $target) {
        $trans = workbench_moderation_transition_load($target['target_id']);
        $entity->field_transitions[LANGUAGE_NONE][$key] = $trans->from_name . ":" . $trans->to_name;
      }
    }
    return parent::export($entity, $prefix);
  }

  /**
   * Import workbench moderation profile data.
   */
  public function import($data) {
    $profile = drupal_json_decode($data);
    $field_trans = array();
    if (!empty($profile['field_transitions'][LANGUAGE_NONE]) && is_array($profile['field_transitions'][LANGUAGE_NONE])) {
      foreach ($profile['field_transitions'][LANGUAGE_NONE] as $key => $transition) {

        // Look up ID by from_name and to_name.
        list($from_name, $to_name) = explode(":", $transition);

        $trans_id = db_select('workbench_moderation_transitions', 'wmt')
          ->fields('wmt', array('id'))
          ->condition('wmt.from_name', $from_name, '=')
          ->condition('wmt.to_name', $to_name, '=')
          ->execute()->fetchField();

        // Use id as an entity reference should.
        if ($trans_id) {
          $field_trans[LANGUAGE_NONE][$key]['target_id'] = $trans_id;
        }
        else {
          drupal_set_message(t('The transition %transition did not exist, and was not imported.', array('%transition' => $transition)), 'error');
        }
      }
    }
    $profile['field_transitions'] = $field_trans;

    if (is_array($profile)) {
      return $this->create($profile);
    }
    return FALSE;
  }

  /**
   * Construct the entity.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Create a workbench_moderation_profile.
   *
   * @param array $values
   *   Array of values to include in object.
   *
   * @return obj
   *   A workbench_moderation_profile object w/ all default fields initialized.
   */
  public function create(array $values = array()) {
    /* We first set up the values that are specific to our
     * workbench_moderation_profile schema but then also go through the
     * EntityAPIController function.
     */
    // Add values that are specific to our WorkbenchModerationProfile.
    $values += array(
      'wmpid' => '',
      'label' => '',
      'profile' => '',
    );

    $workbench_moderation_profile = parent::create($values);
    return $workbench_moderation_profile;
  }
}

/**
 * UI controller.
 */
class WorkbenchModerationProfileUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['title'] = t('Profiles');
    $items[$this->path]['description'] = 'Manage profiles, including fields.';
    $items[$this->path]['access callback'] = 'workbench_moderation_profile_access';
    $items[$this->path]['access arguments'] = array('view', 'administer workbench_moderation_profiles');
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    $items[$this->path]['weight'] = 5;
    return $items;
  }

  /**
   * Override for overviewForm function.
   */
  public function overviewForm($form, &$form_state) {
    $form = parent::overviewForm($form, $form_state);
    // Adjust some language.
    $form['table']['#header'][0] = t("Moderation Profile");
    $form['table']['#empty'] = t("No workflow profiles have been defined.");
    return $form;
  }
}
