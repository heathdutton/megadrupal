<?php

namespace Drupal\campaignion_action;

abstract class TypeBase implements TypeInterface {
  /**
   * Content-type
   */
  protected $type;
  /**
   * Parameters
   */
  protected $parameters;

  public function __construct($type, array $parameters = array()) {
    $this->type = $type;
    $this->parameters = $parameters;
  }

  public function defaultTemplateNid() {
    return NULL;
  }

  public function actionFromNode($node) {
    return new ActionBase($this, $node);
  }

  public static function isAction($type) {
    $action_types = self::types();
    return isset($action_types[$type]);
  }

  public static function typesInfo() {
    $types_info = \module_invoke_all('campaignion_action_info');
    foreach ($types_info as $type => &$info) {
      $info += array('parameters' => array());
    }
    return $types_info;
  }

  public static function types() {
    $types = drupal_static(__CLASS__);
    if ($types === NULL) {
      $types = array();
      $types_info = static::typesInfo();
      foreach ($types_info as $type => $info) {
        $class = $info['class'];
        $types[$type] = new $class($type, $info['parameters']);
      }
    }
    return $types;
  }

  public static function thankYouPageTypes() {
    $tyTypes = array();
    foreach (static::typesInfo() as $type => $info) {
      $p = &$info['parameters'];
      if (isset($p['thank_you_page'])) {
        $tyTypes[$p['thank_you_page']['type']][$p['thank_you_page']['reference']] = TRUE;
      }
    }
    return $tyTypes;
  }

  public static function referenceFieldsByType($type) {
    $types = static::thankYouPageTypes();
    if (isset($types[$type])) {
      return array_keys($types[$type]);
    }
    return FALSE;
  }

  public static function fromContentType($type) {
    $action_types = self::types();
    if (isset($action_types[$type])) {
      return $action_types[$type];
    }
  }

  /**
   * {@inheritdoc}
   */
  public function isDonation() {
    return FALSE;
  }
}
