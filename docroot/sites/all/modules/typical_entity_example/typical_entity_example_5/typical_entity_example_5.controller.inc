<?php

/**
 * @file
 * Describe entity and controller classes.
 */

/**
 * Typical Entity 5 class.
 */
class TypicalEntityExample5 extends Entity {
  /**
   * Define the label of the entity.
   */
  protected function defaultLabel() {
    if (!isset($this->title)) {
      return '';
    }
    return $this->title;
  }

  /**
   * Specify the default uri, which is picked up by uri() by default.
   */
  protected function defaultUri() {
    return array('path' => 'typical_entity_example_5/' . $this->identifier());
  }
}

/**
 * Typical Entity 5 controller class.
 */
class TypicalEntityExample5Controller extends EntityAPIController {
  /**
   * Build a structured array representing the entity's content.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('typical_entity_example_5', $entity);

    // Make "title" and "description" properties themed like default fields.
    $content['title'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' => t('Title'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_title',
      '#field_type' => 'text',
      '#entity_type' => 'typical_entity_example_5',
      '#bundle' => $wrapper->type->value(),
      '#items' => array(array('value' => $wrapper->title->value())),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($wrapper->title->value())),
    );
    $content['description'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' => t('Description'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'typical_entity_example_5',
      '#bundle' => $wrapper->type->value(),
      '#items' => array(array('value' => $wrapper->description->value())),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($wrapper->description->value())),
    );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}
