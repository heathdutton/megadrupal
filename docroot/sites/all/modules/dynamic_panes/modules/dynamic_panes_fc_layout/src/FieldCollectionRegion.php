<?php

/**
 * @file
 * Contains class for Field Collection region class.
 */

namespace Drupal\dynamic_panes_fc_layout;

use Drupal\dynamic_panes\Region;

/**
 * Class for Field Collection region class.
 */
class FieldCollectionRegion extends Region {

  /**
   * @var \EntityDrupalWrapper
   */
  protected $data;

  /**
   * @var FieldCollectionLayout
   */
  protected $layout;

  /**
   * Overrides Region::__construct().
   */
  public function __construct($data, $layout) {
    $this->data = $data;
    $this->layout = $layout;
    $this->blocks = array();
    $this->addBlockLinks = array();

    // Init empty region.
    if (is_null($this->data)) {
      $this->initEmptyRegion();
    }

    $this->initBlocks();
    $this->initAdminLinks();
  }

  /**
   * Overrides Region::sortForm().
   */
  public function sortForm(&$form) {
    parent::sortForm($form);

    $layout_type = $this->layout->getLayoutType();

    if (!isset($form[$layout_type])) {
      $form[$layout_type] = array(
        '#tree' => TRUE,
      );
    }

    foreach ($this->blocks as $level => $blocks) {
      $sort = array();
      foreach ($blocks as $block) {
        $sort[] = $block->getID();
      }

      $form[$layout_type][$this->getRegionID()][$level] = array(
        '#type' => 'hidden',
        '#default_value' => implode(',', $sort),
      );
    }
  }

  /**
   * Overrides Region::sortFormSubmit().
   */
  public static function sortFormSubmit($form, &$form_state) {
    $values = $form_state['values'][DYNAMIC_PANES_FC_LAYOUT_ENTITY_TYPE_NAME];

    foreach ($values as $fc_item_id => $levels) {
      if ($fc_region_wrapper = entity_metadata_wrapper('field_collection_item', $fc_item_id)) {
        if (isset($fc_region_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS_BLOCKS})) {
          foreach ($fc_region_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS_BLOCKS} as $fc_block_wrapper) {
            if (isset($fc_block_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_LEVEL})) {
              $level = $fc_block_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_LEVEL}->value();
              $level = is_null($level) ? 'all' : $level;

              if (isset($values[$fc_item_id][$level])) {
                $fc_block_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_BLOCK}->set(explode(',', $values[$fc_item_id][$level]));
                $fc_block_wrapper->save();
              }
            }
          }
        }

      }
    }
  }

  /**
   * Implements Region::initAdminLinks().
   */
  protected function initAdminLinks() {
    $this->adminLinks['layout-add-block'] = array();

    $link = array(
      'title' => t('Add block'),
      'query' => array(drupal_get_destination()),
      'attributes' => array('title' => t('Add new block')),
    );

    $levels = array_replace(array('all' => 'all'), _dynamic_panes_get_config_levels());
    if (isset($this->data->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS_BLOCKS})) {
      foreach ($this->data->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS_BLOCKS} as $fc_block_wrapper) {
        if (isset($fc_block_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_LEVEL})) {
          $level = $fc_block_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_LEVEL}->value();
          $level = is_null($level) ? 'all' : $level;

          $this->adminLinks['layout-add-block'][$level] = $link + array(
            'href' => $this->getAddBlockUrl($fc_block_wrapper->getIdentifier(), $level),
          );
        }
      }
    }

    foreach (array_diff_key($levels, $this->adminLinks['layout-add-block']) as $level) {
      $this->adminLinks['layout-add-block'][$level] = $link + array(
        'href' => $this->getAddBlockUrl('new', $level),
      );
    }
  }

  /**
   * Implements Region::initBlocks().
   */
  protected function initBlocks() {
    if (isset($this->data->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS_BLOCKS})) {
      foreach ($this->data->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS_BLOCKS} as $fc_block_wrapper) {
        $field_level_exist = isset($fc_block_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_LEVEL});
        $field_block_exist = isset($fc_block_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_BLOCK});
        if ($field_level_exist && $field_block_exist) {
          $level = $fc_block_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_LEVEL}->value();
          $level = is_null($level) ? 'all' : $level;

          foreach ($fc_block_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_BLOCK}->value() as $delta => $block) {
            $this->addBlock($fc_block_wrapper->getIdentifier(), $level, $delta, $block);
          }
        }
      }
    }
  }

  /**
   * Implements Region::getRegionType().
   */
  public function getRegionType() {
    return 'field_collection_item';
  }

  /**
   * Implements Region::getRegionID().
   */
  public function getRegionID() {
    $region_id = $this->data->getIdentifier();
    return !empty($region_id) ? $region_id : 'new';
  }

  /**
   * Implements Region::getRegionName().
   */
  public function getRegionName() {
    if ($this->data->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS_REGION}) {
      return $this->data->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS_REGION}->value();
    }

    return FALSE;
  }

  /**
   * Implements Region::getFormElementName().
   */
  public function getFormElementName($level) {
    $name_array = array(
      $this->layout->getLayoutType(),
      $this->getRegionID(),
      $level,
    );

    $name = array_shift($name_array);
    $name .= '[' . implode('][', $name_array) . ']';

    return $name;
  }

  /**
   * Init empty region.
   */
  protected function initEmptyRegion() {
    $layout_type = $this->layout->getLayoutType();
    $layout_wrapper = $this->layout->getData();

    $region_type = $this->getRegionType();
    $region_name = $this->layout->getRegionName();

    $region = entity_create($region_type, array('field_name' => DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS));
    $region->setHostEntity($layout_type, $layout_wrapper->value(), LANGUAGE_NONE, FALSE);

    $this->data = entity_metadata_wrapper($region_type, $region);
    $this->data->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS_REGION}->set($region_name);
  }

  /**
   * Add block to $this->blocks.
   *
   * @param int $block_wrapper_id
   *   The field_collection_items id used as level wrapper of this block.
   * @param int $level
   *   The level wrapper of this block.
   * @param int $delta
   *   The block delta.
   * @param object $block
   *   The block object.
   */
  protected function addBlock($block_wrapper_id, $level, $delta, $block) {
    $context = array(
      'block_wrapper_id' => $block_wrapper_id,
      'delta' => $delta,
    );

    if ($block = $this->wrapBlock($block, $context)) {
      $this->blocks[$level][$delta] = $block;
    }
  }

  /**
   * Get url for add block to this region.
   *
   * @param int $id
   *   The ID of this region.
   * @param int|string $level
   *   The level of region.
   *
   * @return string
   *   The url of link.
   */
  protected function getAddBlockUrl($id, $level) {
    $path = array(
      'dynamic-panes-fc-layout',
      $this->layout->getLayoutID(),
      $this->getRegionID(),
      $this->getRegionName(),
      $id,
      $level,
      'add',
    );

    return implode('/', $path);
  }
}
