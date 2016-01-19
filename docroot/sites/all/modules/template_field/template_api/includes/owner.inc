<?php

interface TemplateApiOwnerTypePlugin {

  /**
   * Permits access to an action and item
   *
   * @abstract
   *
   * @param $op
   * @param $template Template
   *
   * @return Boolean | NULL
   *  Boolean if setting access and NULL if it has no impact
   */
  public function access($op, Template $template);

  /**
   * Get the values that can be an owner
   *
   * @return array
   */
  public function values();

  /**
   * Get the label of the owner
   *
   * @return string
   */
  public function label(Template $template);
}

/**
 * The abstract class for owner types
 */
abstract class TemplateApiOwnerType extends TemplateApiPlugin implements TemplateApiOwnerTypePlugin {

  /**
   * Get the values that can be an owner
   *
   * @return array
   */
  public function values() {
    return array();
  }

  /**
   * Get the label of the owner
   *
   * @return string
   */
  public function label(Template $template) {
    return $template->owner;
  }
}


class TemplateApiOwnerTypeController extends TemplateApiPluginController {

  /**
   * The plugin type
   */
  protected function pluginType() {
    return 'owner_type';
  }

  /**
   * Check access
   */
  public function checkAccess($op, Template $template) {
    return $this->plugin->access($op, $template);
  }

  public function getValues() {
    return $this->plugin->values();
  }

  /**
   * Get the label of the owner
   *
   * @param Template $template
   *
   * @return string|NULL
   */
  public function getLabel(Template $template) {
    if (empty ($template->owner_type) || empty($template->owner)) {
      return NULL;
    }
    return $this->plugin->label($template);
  }
}
