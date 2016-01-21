<?php
/**
 * @file
 * Provides default classes for UI actions on data sets.
 */

/**
 * The interface for action UI controllers.
 *
 * @see party_attached_entity_action_form()
 */
interface PartyDataSetActionInterface {

  /**
   * Get the page title for the action form.
   *
   * @todo: consider moving this to the data set action definition, where the
   * local action label is already defined.
   *
   * @param $party
   *   A loaded party object.
   * @param $data_set
   *   A loaded data set.
   *   We don't actually need this loaded, but we need a menu loader to convert
   *   the path-style string to the machine name, and a menu loader that doesn't
   *   load would be weird too.
   * @param $eid
   *   The id of the entity, if relevant to this action.
   *
   * @return
   *  A localized string.
   */
  public function get_page_title($name, $var);

  /**
   * Provides the action form for adding a new entity.
   *
   * @param $party
   *   A loaded party object.
   * @param $data_set
   *   A loaded data set.
   *   We don't actually need this loaded, but we need a menu loader to convert
   *   the path-style string to the machine name, and a menu loader that doesn't
   *   load would be weird too.
   * @param $eid
   *   The id of the entity, if relevant to this action.
   *
   * @return
   *  A FormAPI array.
   */
  function action_form($form, &$form_state, $party, $data_set, $eid = NULL);

  /**
   * Form validation for the action form.
   */
  function action_form_validate($form, &$form_state);

  /**
   * Form submission for the action form.
   */
  function action_form_submit($form, &$form_state);

}

/**
 * The 'add' action: attach a new entity.
 *
 * @todo: not yet in use. See todo in party_menu().
 */
class PartyDefaultDataSetUIAdd implements PartyDataSetActionInterface {

  /**
   * The page title for the action form.
   */
  function get_page_title($party, $data_set, $eid = NULL) {
    return t("Add new @data-set-label to @party-label", array(
      '@data-set-label' => $data_set['label'],
      '@party-label' => $party->label,
    ));
  }

  /**
   * Provides the action form for adding a new entity.
   *
   * @param $party
   *   A loaded party object.
   * @param $data_set
   *   A loaded data set.
   *   We don't actually need this loaded, but we need a menu loader to convert
   *   the path-style string to the machine name, and a menu loader that doesn't
   *   load would be weird too.
   * @param $eid
   *   The id of the entity, if relevant to this action.
   */
  function action_form($form, &$form_state, $party, $data_set, $eid = NULL) {
    $form = array();

    $form_state['#attached_entity'] = $party
      ->getDataSetController($data_set['set_name'])
      ->createEntity();

    field_attach_form($data_set['entity type'], $form_state['#attached_entity'], $form, $form_state);

    $form['actions'] = array(
      '#type' => 'actions',
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Create new @label', array(
          '@label' => $data_set['label'],
        )),
      ),
    );

    return $form;
  }

  function action_form_validate($form, &$form_state) {
    // Get the original form parameters.
    list ($party, $data_set, $action, $eid) = $form_state['build_info']['args'];

    field_attach_form_validate($data_set['entity type'], $form_state['#attached_entity'], $form, $form_state);
  }

  function action_form_submit($form, &$form_state) {
    // Get the original form parameters.
    list ($party, $data_set, $action, $eid) = $form_state['build_info']['args'];

    field_attach_submit($data_set['entity type'], $form_state['#attached_entity'], $form, $form_state);

    // Attach the entity and save the entity and controller.
    $party->getDataSetController($data_set['set_name'])
      ->attachEntity($form_state['#attached_entity'])
      ->save(TRUE);
  }

}

/**
 * The 'attach' action: attach an existing entity.
 */
class PartyDefaultDataSetUIAttach implements PartyDataSetActionInterface {

  /**
   * The page title for the action form.
   */
  function get_page_title($party, $data_set, $eid = NULL) {
    return t("Attach existing @data-set-label to @party-label", array(
      '@data-set-label' => $data_set['label'],
      '@party-label' => $party->label,
    ));
  }

  /**
   * Provides the action form for attaching an existing entity.
   *
   * @param $party
   *   A loaded party object.
   * @param $data_set
   *   A loaded data set.
   *   We don't actually need this loaded, but we need a menu loader to convert
   *   the path-style string to the machine name, and a menu loader that doesn't
   *   load would be weird too.
   * @param $eid
   *   The id of the entity, if relevant to this action.
   */
  function action_form($form, &$form_state, $party, $data_set, $eid = NULL) {
    $form = array();

    drupal_set_message(t("This doesn't work for general entities yet!"), 'error');

    return $form;
  }

  function action_form_validate($form, &$form_state) {

  }

  function action_form_submit($form, &$form_state) {

  }

}
