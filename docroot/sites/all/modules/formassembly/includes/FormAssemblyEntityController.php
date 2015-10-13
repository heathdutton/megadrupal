<?php

/**
 * @file
 * Provides a controller building upon the Entity API but with
 * some additional display and search.
 *
 * Longer file description goes here
 * Author: Shawn P. Duncan
 * Date: 7/23/14
 * Time: 4:18 PM
 */
class FormAssemblyEntityController extends EntityAPIController {

  /**
   * Extends the parent method to add FormAssembly markup.
   *
   * @param FormAssemblyEntity $entity
   *   The fa_form entity being rendered
   *
   * @param string $view_mode
   *   The view mode being displayed
   *
   * @param null $langcode
   *   optional
   *
   * @param array $content
   *   optional
   *
   * @return array
   *   A Drupal render array
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $client_id = variable_get('formassembly_oauth_cid', '');
    $client_secret = variable_get('formassembly_oauth_secret', '');
    $request = new FormAssemblyRequest($client_id, $client_secret);
    $token = $request->getToken();
    if ($token) {
      if (empty($_GET['tfa_next'])){
        $markup = $request->getFormMarkup($entity);
      }
      else {
        $markup = $request->getNextForm($_GET['tfa_next']);
      }
      $content['fa_markup'] = array(
        '#type' => 'markup',
        '#markup' => $markup,
      );
      drupal_page_is_cacheable(FALSE);
    }
    else {
      watchdog('formassembly', 'Could not get markup for form with faid: @faid.', array('@faid' => $entity->faid), WATCHDOG_ERROR);
    }
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }


  /**
   * Selects entities to load by property.
   *
   * Adapted from a patch to
   * EntityStorageControllerInterface::loadByProperties()
   * Patch posted at https://www.drupal.org/files/1184272-77.patch
   *
   * @param array $values
   *   An array of property values.
   *
   *   The array key is the property name and array value is the value of that
   *   property to be matched. The database operator will set to '='
   *   for single values and to 'IN' for an array of values.
   */
  public function loadByProperties($values = array()) {
    // Build a query to fetch the entity IDs.
    $entity_query = new EntityFieldQuery();
    $entity_query->entityCondition('entity_type', 'fa_form');
    $this->buildPropertyQuery($entity_query, $values);
    $result = $entity_query->execute();
    $entities = array();
    if (!empty($result['fa_form'])) {
      $entities = $this->load(array_keys($result['fa_form']));
    }
    return $entities;
  }

  /**
   * Builds an entity query.
   *
   * Adpated from patch for EntityStorageControllerInterface::loadByProperties()
   * posted at https://www.drupal.org/files/1184272-77.patch
   *
   * @param 'Drupal\entity\EntityFieldQuery' $entity_query
   *   EntityFieldQuery instance.
   * @param array $values
   *   An associative array of properties of the entity, where the keys are the
   *   property names and the values are the values those properties must have.
   */
  protected function buildPropertyQuery(EntityFieldQuery $entity_query, array $values) {
    foreach ($values as $name => $value) {
      $entity_query->propertyCondition($name, $value);
    }
  }


  /**
   * Returns the entity id and value of a property for all fa_form entities.
   *
   * @param string $property
   *   The name of the entity property.
   *
   * @return DatabaseStatementInterface|null
   *   If the property exists the query results are returned.
   */
  public function loadPropertySet($property) {
    $info = entity_get_property_info('fa_form');
    if (array_key_exists($property, $info['properties'])) {

      $query = db_select('formassembly', 'fa');
      $query->fields('fa', array('eid', $property));
      $stored = $query->execute();
      return $stored;
    }
    else {
      watchdog('FormAssembly', 'Failed to load Property Set. @property: Not a valid property of fa_form', 'error', array('@property' => $property), WATCHDOG_ERROR);
      return NULL;
    }
  }
}
