<?php

/**
 * @file
 * FeedsUserProcessor class.
 */

/**
 * Feeds processor plugin. Create users from feed items.
 */
class FeedsCommercePricelistItemProcessor extends FeedsProcessor {

  /**
   * Define entity type.
   */
  public function entityType() {
    return 'commerce_pricelist_item';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Commerce pricelist_items');
    return $info;
  }

  /**
   * Creates a new pricelist_item and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $list_id = $source->config['FeedsCommercePricelistItemProcessor']['list_id'];
    $pricelist_item = commerce_pricelist_item_new($list_id);
    $pricelist_item->log = 'Created by FeedsCommercePricelistItemProcessor';
    return $pricelist_item;
  }

  /**
   * Loads an existing pricelist_item.
   */
  protected function entityLoad(FeedsSource $source, $pricelist_item_id) {
    $pricelist_item = commerce_pricelist_item_load($pricelist_item_id);
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $pricelist_item->log = 'Updated by FeedsCommercePricelistItemProcessor';
    }
    else {
      $pricelist_item->log = 'Replaced by FeedsCommercePricelistItemProcessor';
    }
    return $pricelist_item;
  }

  /**
   * Save a user account.
   */
  protected function entitySave($entity) {
    commerce_pricelist_item_save($entity);
  }

  /**
   * Delete multiple user accounts.
   */
  protected function entityDeleteMultiple($ids) {
    commerce_pricelist_item_delete_multiple($ids);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'is_active' => TRUE,
      'is_attached' => TRUE,
      'list_id' => NULL,
      'convert_price' => FALSE,
      'convert_dates' => FALSE,
    ) + parent::configDefaults();
  }

  public function sourceForm($source_config) {
    return array('list_id' => NULL);
  }

  public function sourceDefaults() {
    return array(
      'list_id' => NULL,
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);

    $form['is_active'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#description' => t('Select whether pricelist_items should be imported active or not.'),
      '#options' => array(0 => t('Inactive'), 1 => t('Active')),
      '#default_value' => $this->config['is_active'],
    );

    $form['is_attached'] = array(
      '#type' => 'radios',
      '#title' => t('Attach to pricelist form'),
      '#description' => t('Should this importer be attached to th pricelist form.'),
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => $this->config['is_attached'],
    );

    $form['convert_price'] = array(
      '#type' => 'radios',
      '#title' => t('Convert price to minor currency on import'),
      '#description' => t('Should the price be multiplied by 100 or rounded to nearest integer?'),
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => $this->config['convert_price'],
    );

    $form['convert_dates'] = array(
      '#type' => 'radios',
      '#title' => t('Convert dates to Unix timestamp'),
      '#description' => t('Should we attempt to convert the dates?'),
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => $this->config['convert_dates'],
    );

    return $form;
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();
    $targets += array(
      'sku' => array(
        'name' => t('Product SKU'),
        'description' => t('The product identifier.'),
      ),
      'valid_from' => array(
        'name' => t('Valid from'),
        'description' => t('Unix timestamp'),
      ),
      'valid_to' => array(
        'name' => t('Valid to'),
        'description' => t('Unix timestamp'),
      ),
      'price_amount' => array(
        'name' => t('Price amount'),
        'description' => t('In minor currency (€1,00 = 100)'),
      ),
      'currency_code' => array(
        'name' => t('Currency code'),
        'description' => t('USD'),
      ),
      'quantity' => array(
        'name' => t('Quantity'),
        'description' => t('Lower quantity limit for this unit price'),
      ),
      'pricelist_id' => array(
        'name' => t('Price list id'),
        'description' => t('Price list ID'),
      ),
      'pricelist_title' => array(
        'name' => t('Price list title'),
        'description' => t('Name of the price list, first match'),
        'callback' => 'commerce_pricelist_feeds_set_target',
      ),
    );

    // UUID module support plus support for mappings set up before beta version.
    if (module_exists('uuid')) {
      $targets += array(
        'pricelist_uuid' => array(
          'name' => t('Price list uuid'),
          'description' => t('Generated Unique ID'),
          'callback' => 'commerce_pricelist_feeds_set_target',
        ));
    }

    // Let other modules expose mapping targets.
    self::loadMappers();
    $entity_type = $this->entityType();
    $bundle = $this->bundle();
    drupal_alter('feeds_processor_targets', $targets, $entity_type, $bundle);

    return $targets;
  }

  /**
   * @param FeedsSource $source
   * @param FeedsParserResult $result
   * @param null $target_item
   * @return array|null
   */
  protected function map(FeedsSource $source, FeedsParserResult $result, $target_item = NULL) {
    $target_item = parent::map($source, $result, $target_item);
    if ($source->importer->processor->config['convert_price'] == 1) {
      // Convert price to minor currency
      $target_item->price_amount = $target_item->price_amount * 100;
    }
    if ($source->importer->processor->config['convert_dates'] == 1) {
      // convert date strings to unix timestamp
      $target_item->valid_from = strtotime($target_item->valid_from);
      $target_item->valid_to = strtotime($target_item->valid_to);
    }
    return $target_item;
  }

}
