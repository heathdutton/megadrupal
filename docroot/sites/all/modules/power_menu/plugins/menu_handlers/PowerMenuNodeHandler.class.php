<?php

include_once 'PowerMenuHandler.interface.php';

/**
 * Implementation of the interface PowerMenuHandlerInterface.
 */
class PowerMenuNodeHandler implements PowerMenuHandlerInterface {

  /**
   * @see PowerMenuHandlerInterface::configurationForm()
   */
  public function configurationForm() {
    $form = array();
    return $form;
  }

  /**
   * @see PowerMenuHandlerInterface::configurationFormSubmit()
   */
  public function configurationFormSubmit(array $form, array &$form_state) {

  }

  /**
   * @see PowerMenuHandlerInterface::configurationFormValidate()
   */
  public function configurationFormValidate(array &$elements, array &$form_state, $form_id = NULL) {

  }

  /**
   * @see PowerMenuHandlerInterface::getMenuPathToActivate()
   */
  public function getMenuPathToActivate($entity, $type, array $router_item, $alias) {
    $path = NULL;
    $bundles = variable_get('power_menu_node_bundles', array());
    $info = entity_get_info($type);

    // Get the bundle name from the entity. Does no bundle exists use type
    $bundle = isset($entity->$info['entity keys']['bundle']) ? $entity->$info['entity keys']['bundle'] : $type;
    $key = $type . '|' . $bundle;

    // Is a menu link defined for given bundle key return it.
    if (!empty($bundles[$key])) {
      $path = $bundles[$key]['link_path'];
    }

    return $path;
  }

  /**
   * @see PowerMenuHandlerInterface::menuFormAlter()
   */
  public function menuFormAlter(&$menu_item_form, &$form_state) {
    $form = array();

    $bundles = power_menu_get_entities_and_bundles();
    $bundles_used = variable_get('power_menu_node_bundles', array());
    $bundles_selected = array();

    foreach ($bundles as $key => $value) {
      // Is this bundel used by a menu item. When not leave it as an option.
      if (!empty($bundles_used[$key])) {
        // Is it used from this menu item. Leave it as selected option
        if ($bundles_used[$key]['mlid'] == $menu_item_form['mlid']['#value']) {
          $bundles_selected[] = $key;
        }
        // Is it used from an other menu item, remove it as an option
        else {
          unset($bundles[$key]);
        }
      }
    }

    $form['power_menu_node_bundles'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $bundles,
      '#default_value' => $bundles_selected,
      '#description' => t('Select the entity/bundle on which this menu item should be marked as active. The entity/bundels that are not available for selection are already being used for an other menu item.'),
    );
    return $form;
  }

  /**
   * @see PowerMenuHandlerInterface::menuFormValidate()
   */
  public function menuFormValidate(array &$elements, array &$form_state, $form_id = NULL) {

  }

  /**
   * @see PowerMenuHandlerInterface::menuFormSubmit()
   */
  public function menuFormSubmit(array $form, array &$form_state) {

    $bundles = variable_get('power_menu_node_bundles', array());

    // Remove all bundles with this mlid
    foreach ($bundles as $key => $value) {
      if ($value['mlid'] == $form_state['values']['mlid']) {
        unset($bundles[$key]);
      }
    }

    if (isset($form_state['values']['power_menu_node_bundles'])) {
      // Add bundels for this menu item
      foreach ($form_state['values']['power_menu_node_bundles'] as $value) {
        $bundles[$value] = array(
          'mlid' => $form_state['values']['mlid'],
          'link_path' => $form_state['values']['link_path'],
          'menu_name' => $form_state['values']['menu_name'],
          'router_path' => $form_state['values']['router_path'],
        );
      }
    }

    variable_set('power_menu_node_bundles', $bundles);
  }

  /**
   * @see PowerMenuHandlerInterface::menuLinkDelete()
   */
  public function menuLinkDelete(array $link) {

  }
}
