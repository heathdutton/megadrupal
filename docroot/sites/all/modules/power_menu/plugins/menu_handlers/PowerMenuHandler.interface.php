<?php

/**
 * Interface representing a power menu handler.
 */
interface PowerMenuHandlerInterface {

  /**
   * Display a form for configuring this handler.
   *
   * @return array
   *   A form array for configuring this handler, or FALSE if no configuration
   *   is necessary.
   */
  public function configurationForm();

  /**
   * Validation callback for the form returned by configurationForm().
   *
   * @param array &$elements
   *   An associative array containing the structure of the form.
   * @param array &$form_state
   *   A keyed array containing the current state of the form.
   * @param array $form_id
   *   A unique string identifying the form.
   */
  public function configurationFormValidate(array &$elements, array &$form_state, $form_id = NULL);

  /**
   * Submit callback for the form returned by configurationForm().
   *
   * This method should both return the new options and set them internally.
   *
   * @param array $form
   *   The form returned by configurationForm().
   * @param array $form_state
   *   The complete form state.
   *
   * @return array
   *   The new options array for this callback.
   */
  public function configurationFormSubmit(array $form, array &$form_state);

  /**
   * Get the menu path which sould activated.
   *
   * This method is used when a handler plugin should modify the menu router item information.
   *
   * @param $entity
   *     The eintity object
   * @param $type
   *     The eintity type
   * @param array $item
   *     The router item array to alter
   * @param array $alias
   *     The alias or system path for the given entity
   *
   * @return
   *   The path to activate or NULL when no path is found.
   */
  public function getMenuPathToActivate($entity, $type, array $router_item, $alias);

  /**
   * Drupal menu item form alter.
   *
   * Alter the existing form or return a forms array which is added to the menu item form grouped by the handler.
   *
   * @param array &$menu_item_form
   *   An associative array containing the structure of the form.
   * @param array &$form_state
   *   A keyed array containing the current state of the form.
   *
   * @return
   *   Return a form array, otherwise FALSE.
   */
  public function menuFormAlter(&$menu_item_form, &$form_state);

  /**
   * Validation callback for the form returned by form menu_edit_item.
   *
   * @param array &$form
   *   An associative array containing the structure of the form.
   * @param array &$form_state
   *   A keyed array containing the current state of the form.
   * @param array $form_id
   *   A unique string identifying the form.
   */
  public function menuFormValidate(array &$form, array &$form_state, $form_id = NULL);

  /**
   * Submit callback for the form returned by form menu_edit_item.
   *
   * This method should both return the new options and set them internally.
   *
   * @param array $form
   *   The form returned by configurationForm().
   * @param array $form_state
   *   The complete form state.
   */
  public function menuFormSubmit(array $form, array &$form_state);

  /**
   * Callback for the hook_menu_link_delete().
   *
   * This method should cleanup menu link related data from a menu handler.
   *
   * @param array $link
   *   The complete menu link array.
   */
  public function menuLinkDelete(array $link);

}
