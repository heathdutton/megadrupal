<?php

include_once 'PowerMenuHandler.interface.php';

/**
 * Implementation of the interface PowerMenuHandlerInterface.
 */
class PowerMenuPathHandler implements PowerMenuHandlerInterface {

  /**
   * @see PowerMenuHandlerInterface::configurationForm()
   */
  public function configurationForm() {
    $form = array();

    $form['note'] = array(
      '#markup' => '<p><strong>' . t('Choose the entity bundles for path recognition.') . '</strong></p><p>' . t('For which bundle should the path alias define the menu trail?
        The parent path elements are used to find a menu link with the given path. Example:
        Path for content \'forum/discussion/myquestion\', lookup for a menu link \'forum/discussion\' and activate this menu link.') . '</p>',
    );

    $form['power_menu_path']['bundles'] = array(
      '#type' => 'fieldset',
      '#title' => t('Bundles (Entity type : bundle)'),
      '#collapsible' => FALSE,
    );

    $form['power_menu_path']['bundles']['elements'] = array(
      '#type' => 'checkboxes',
      '#options' => power_menu_get_entities_and_bundles(),
      '#default_value' => variable_get('power_menu_path_bundles', array()),
    );

    $form['power_menu_path']['steps'] = array(
      '#type' => 'fieldset',
      '#title' => t('How many steps back'),
      '#collapsible' => FALSE,
    );

    $form['power_menu_path']['steps']['numbers'] = array(
      '#type' => 'select',
      '#title' => '',
      '#default_value' => variable_get('power_menu_path_number', 1),
      '#options' => array(1 => 1, 2 => 2, 3 => 3),
      '#description' => t('The number of path elements to go back to search for a valid menu link.'),
    );

    return $form;
  }

  /**
   * @see PowerMenuHandlerInterface::configurationFormSubmit()
   */
  public function configurationFormSubmit(array $form, array &$form_state) {
    $selection = array();

    foreach ($form_state['values']['elements'] as $value) {
      if (!empty($value)) {
        $selection[] = $value;
      }
    }

    variable_set('power_menu_path_bundles', $selection);

    variable_set('power_menu_path_number', $form_state['values']['numbers']);
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
    $bundles = variable_get('power_menu_path_bundles', array());
    $steps = variable_get('power_menu_path_number', 1);
    // Is no type field in the entity, use entity type as bundle name
    $bundle = empty($entity->type) ? $type : $entity->type;

    // Is path recognition enabled for this entity type and bundle
    if (in_array($type . '|' . $bundle, $bundles)) {

      for ($i = 1; $i <= $steps; $i++) {
        $alias = explode('/', $alias);
        array_pop($alias);

        // Does a parent path exists
        if (count($alias) > 0) {
          $alias = implode('/', $alias);
          // Menu link lookup for given alias
          $query = db_select('url_alias', 'ua');
          $query->leftJoin('menu_links', 'ml', 'ua.source = ml.link_path');
          $path = $query->fields('ml', array('link_path'))
              ->condition('ua.alias', $alias)
              ->execute()->fetchField();

          if ($path) {
            break;
          }
        }
        else {
          break;
        }
      }
    }

    return $path;
  }

  /**
   * @see PowerMenuHandlerInterface::menuFormAlter()
   */
  public function menuFormAlter(&$menu_item_form, &$form_state) {
    return FALSE;
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

  }

  /**
   * @see PowerMenuHandlerInterface::menuLinkDelete()
   */
  public function menuLinkDelete(array $link) {

  }
}
