<?php

/**
 * @file
 * Use date fields attached to entities as time frame.
 *
 * This provides a handler for utilizing date fields attached to entities as
 * time frame for group content. Date field configuration like granularity and
 * "collect end date" are utilized.
 *
 * For this implementation to be meaningfull, modules have to call
 * setFieldInstance() and setEntityId() at some point.
 */

/**
 * Extension of OgTimeframeHandlerInterface for date field as source.
 */
interface OgTimeframeDateFieldHandlerInterface extends OgTimeframeHandlerInterface {

  /**
   * Set the source field to retrieve dates from.
   *
   * @param string $type
   *   Entity type of date field source.
   * @param string $bundle
   *   Bundle name of date field source.
   * @param string $field_name
   *   Date field name to use as time frame.
   */
  public function setFieldInstance($type, $bundle, $field_name);

  /**
   * Set the Entity ID where to retrieve dates from.
   *
   * @param int $id
   *   The Entity ID.
   */
  public function setEntityId($id);

}

/**
 * The main class for using date fields as source for time frame.
 */
class OgTimeframeDateFieldHandler extends OgTimeframeHandler implements OgTimeframeDateFieldHandlerInterface {

  /**
   * Entity type of date source.
   *
   * @var string
   */
  protected $dateType = '';

  /**
   * Entity bundle name of date source.
   *
   * @var string
   */
  protected $dateBundle = '';

  /**
   * Field name of date source.
   *
   * @var string
   */
  protected $dateFieldName = '';

  // What column from date field items to collect 'from' and 'to' values from.
  // Depends on date field settings.
  protected $fromColumn = '';
  protected $toColumn = 'value';

  /**
   * Implements OgTimeframeDateFieldHandlerInterface::setFieldInstance().
   */
  public function setFieldInstance($type, $bundle, $field_name) {
    $this->dateType = $type;
    $this->dateBundle = $bundle;
    $this->dateFieldName = $field_name;
    $date_field = field_info_field($field_name);
    if ($date_field['module'] != 'date') {
      throw new Exception(t("Field is not a date."));
    }
    $this->measure = og_timeframe_granularity_to_measure(date_granularity_precision($date_field['settings']['granularity']));
    if (count(date_process_values($date_field)) > 1) {
      // Date field is configured to collect end date. In which case it acts as
      // both from and to dates.
      $this->fromColumn = 'value';
      $this->toColumn = 'value2';
    }
    else {
      $this->fromColumn = '';
      $this->toColumn = 'value';
    }
  }

  /**
   * Implements OgTimeframeDateFieldHandlerInterface::setEntityId().
   */
  public function setEntityId($id) {
    $this->fromDate = $this->toDate = NULL;
    if (!$id) {
      return;
    }
    if ($this->groupId == $id and $this->groupType == $this->dateType and $this->groupBundle == $this->dateBundle) {
      // We're using the group as source for date fields. No need to load
      // anything.
      $date_entity = $this->group;
    }
    else {

      // We have no guarantee the referred entity for date source is loaded or
      // cached, so lets manually load the field content instead of using the
      // costly entity_load().
      $entity_info = entity_get_info($this->dateType);
      $entity_key = $entity_info['entity keys']['id'];

      $date_entity = new stdClass();
      $date_entity->{$entity_key} = $id;
      if (isset($entity_info['entity keys']['bundle'])) {
        $date_entity->{$entity_info['entity keys']['bundle']} = $this->dateBundle;
      }
      field_attach_load($this->dateType, array($id => $date_entity), FIELD_LOAD_CURRENT, array('field_id' => $this->dateFieldName));
    }
    $date_items = field_get_items($this->dateType, $date_entity, $this->dateFieldName);
    if (empty($date_items)) {
      return;
    }
    $date_item = array_shift($date_items);
    $date_format = date_type_format($date_item['date_type']);
    if ($this->fromColumn and !empty($date_item[$this->fromColumn])) {
      $this->fromDate = new DateObject($date_item[$this->fromColumn], $date_item['timezone'], $date_format);
    }
    if (!empty($date_item[$this->toColumn])) {
      $this->toDate = new DateObject($date_item[$this->toColumn], $date_item['timezone'], $date_format);
    }
  }

}
