<?php

/**
 * @file
 * Provides a controller for building a party overview form.
 */

/**
 * Controller for providing Party UI.
 */
class PartyUIController extends EntityDefaultUIController {
  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    // Set the main path title and description as it's our general admin hub.
    $items[$this->path]['title'] = t('Community');
    $items[$this->path]['description'] = t('Manage the community');
    $items[$this->path]['access callback'] = 'user_access';
    $items[$this->path]['access arguments'] = array('administer crm settings');

    // Remove the entity add callback for now, as we provide our own.
    // @todo: use htis one instead!
    unset($items[$this->path . '/add']);

    // Disambiguate the 'list' tab.
    $items[$this->path . '/list']['title'] = 'List parties';
    $items[$this->path . '/list']['access arguments'] = array('administer crm settings');

    return $items;
  }


  /**
   * Builds the entity overview form.
   */
  public function overviewForm($form, &$form_state) {
    // Show an overview for all entities.

    // Actions form is commented out for now as it doesn't do anything yet!
    //$form['actions'] = $this->actionsForm();
    $form['table'] = $this->overviewTable();
    $form['pager'] = array('#theme' => 'pager');
    return $form;
  }

  /**
   * Builds the actions form.
   *
   * @todo: figure out clean parameters here!
   */
  public function actionsForm() {
    // This gives us the bulk operations form element
    $form['options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Update options'),
      '#attributes' => array('class' => array('container-inline')),
    );
    $options = array();
    // Get party operations from any modules that implement hook_party_operations().
    foreach (module_invoke_all('party_operations') as $operation => $array) {
      $options[$operation] = $array['label'];
    }
    $form['options']['operation'] = array(
      '#type' => 'select',
      '#title' => t('Operation'),
      '#title_display' => 'invisible',
      '#options' => $options,
      '#default_value' => 'merge',
    );
    $options = array();
    $form['options']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
    );

    return $form;
  }

  /**
   * Generates the render array for a overview table for arbitrary entities
   * matching the given conditions.
   *
   * @param $conditions
   *   An array of conditions as needed by entity_load().

   * @return Array
   *   A renderable array.
   */
  /*
  // TODO. This is currently commented out as the conversion to a tableselect
  // causes the rows to be truncated due to this core bug: http://drupal.org/node/365554
  // Since we don't actually need the tableselect right now, simplest to
  // remove it temporarily.
  public function overviewTable($conditions = array()) {
    $render_table = parent::overviewTable($conditions);

    // Turn the table into a tableselect.
    $render = array(
      '#type' => 'tableselect',
      '#header' => $render_table['#header'],
      '#options' => $render_table['#rows'],
      '#empty' => t('No parties available.'),
    );

    return $render;
  }
  */

  /**
   * Generates the table headers for the overview table.
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $header = $additional_header;
    array_unshift($header, t('Id'), t('Label'));
    if (!empty($this->entityInfo['exportable'])) {
      $header[] = t('Status');
    }

    // Add operations with the right colspan.
    // $this->operationCount() returns 3 which is wrong: TODO: figure out why
    // it's wrong.
    $header[] = array('data' => t('Operations'), 'colspan' => 1);
    return $header;
  }

  /**
   * Generates the row for the passed entity and may be overridden in order to
   * customize the rows.
   */
  protected function overviewTableRow($conditions, $id, $party, $additional_cols = array()) {
    // Get the basic row from the parent class.
    $row = parent::overviewTableRow($conditions, $id, $party, $additional_cols);

    // Take off the label so we can rearrange things.
    $label = array_shift($row);

    array_unshift($row,
      $id,
      $label
    );

    return $row;
  }

}
