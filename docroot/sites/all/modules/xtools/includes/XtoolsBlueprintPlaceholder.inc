<?php

/**
 * Represents any blueprint defined in a hook_xtools_blueprint_info() hook
 * implementation. Use this when you cannot get a blueprint through
 * xtools_blueprint_info().
 */
class XtoolsBlueprintPlaceholder implements XtoolsBlueprintInterface {

  /**
   * The blueprint's machine name as defined in hook_xtools_blueprint_info().
   *
   * @var string
   */
  public $blueprint_name = '';

  function __construct($blueprint_name) {
    $this->blueprint_name = $blueprint_name;
  }

  /**
   * Implements XtoolsBlueprint::getLabel().
   */
  function getLabel() {
    return $this->blueprint()->getLabel();
  }

  /**
   * Implements XtoolsBlueprint::validate().
   */
  function validate($data, array &$asserts, $path) {
    return $this->blueprint()->validate($data, $asserts, $path);
  }

  /**
   * Load the blueprint.
   *
   * @throws Exception
   *
   * @return XtoolsBlueprintInterface
   */
  function blueprint () {
    if ($blueprint = xtools_blueprint_info($this->blueprint_name)) {
      return $blueprint;
    }
    throw new Exception(t('Blueprint !blueprint_name could not be loaded by xtools_blueprint_info().', array(
      '!blueprint_name' => $this->blueprint_name,
    )));
  }
}