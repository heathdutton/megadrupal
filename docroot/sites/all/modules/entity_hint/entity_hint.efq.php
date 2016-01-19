<?php

use Zend\Code\Generator\ClassGenerator;
use Zend\Code\Generator\DocBlock\Tag\GenericTag;
use Zend\Code\Generator\DocBlockGenerator;
use Zend\Code\Generator\DocBlock\Tag\MethodTag as Tag;
use Zend\Code\Generator\MethodGenerator;
use Zend\Code\Generator\PropertyGenerator;

function entity_hint_efq($type, $bundle = NULL, $namespace = NULL) {
  if (NULL === $bundle) {
    return (new EntityTypeEFQHintGenerator($type, $namespace))->generate();
  }

  return (new EntityBundleEFQHintGenerator($type, $bundle))->generate();
}

/**
 * Generate base entity type EFQ on top of EntityHintEFQ.
 */
final class EntityTypeEFQHintGenerator {
  private $type;
  private $namespace;
  private $wrapper;

  public function __construct($type, $namspace) {
    $this->type = $type;
    $this->namespace = $namspace;
    $this->wrapper = entity_metadata_wrapper($type);
  }

  protected function getClassName($input, $suffix = 'EntityHintEFQ') {
    $name = ucwords(str_replace('_', ' ', $input)) . $suffix;
    return str_replace(' ', '', $name);
  }

  public function generate() {
    $className = $this->getClassName($this->type, 'EntityHintEFQ');
    $class = new ClassGenerator($className);

    $doc = new DocBlockGenerator();
    foreach ($this->wrapper->getPropertyInfo() as $name => $info) {
      $method = 'where__property__' . $name;
      $doc->setTag(new GenericTag('method', $className . ' ' . $method . '()'));
    }

    foreach ($this->wrapper->getPropertyInfo() as $name => $info) {
      $method = 'sort__property__' . $name;
      $doc->setTag(new GenericTag('method', $className . ' ' . $method . '($direction = "ASC")'));
    }

    return $class
      ->setExtendedClass('\atdrupal\efq_wrapper\EntityHintEFQ')
      ->setNamespaceName($this->namespace)
      ->setDocBlock($doc)
      ->setAbstract(TRUE)
      ->addProperty('type', $this->type, PropertyGenerator::FLAG_PROTECTED)
      ->generate();
  }
}

class EntityBundleEFQHintGenerator {
  private $type;
  private $bundle;
  private $wrapper;

  public function __construct($type, $bundle) {
    $this->type = $type;
    $this->bundle = $bundle;
    $this->entity_info = entity_get_info($type);
    $this->wrapper = entity_metadata_wrapper($type);
    if ($bundleKey = $this->entity_info['bundle keys']['bundle']) {
      $this->wrapper = entity_metadata_wrapper($type, entity_create($type, [$bundleKey => $bundle]));
    }
  }

  protected function getClassName($input, $suffix = 'EntityHintEFQ') {
    $name = ucwords(str_replace('_', ' ', $input)) . $suffix;
    return str_replace(' ', '', $name);
  }

  public function generate() {
    $className = $this->getClassName($this->type . ' ' . $this->bundle);
    $parentClass = $this->getClassName($this->type);
    $class = new ClassGenerator($className);

    $class
      ->setIndentation(2)
      ->setExtendedClass($parentClass)
      ->addProperty('bundle', $this->bundle, PropertyGenerator::FLAG_PROTECTED)
      ->addMethodFromGenerator($this->getConstructor());

    $doc = new DocBlockGenerator();
    $doc->setWordWrap(FALSE);

    foreach (field_info_instances($this->type, $this->bundle) as $name => $info) {
      foreach (array_keys(field_info_field($name)['columns']) as $column) {
        $method = 'where__field__' . $name . '__' . $column;
        $doc->setTag(new GenericTag('method', $className . ' ' . $method . '($value, $op = NULL)'));
      }
    }

    foreach (field_info_instances($this->type, $this->bundle) as $name => $info) {
      foreach (array_keys(field_info_field($name)['columns']) as $column) {
        $method = 'sort__field__' . $name . '__' . $column;
        $doc->setTag(new GenericTag('method', $className . ' ' . $method . '($direction = "ASC")'));
      }
    }

    return $class->setDocBlock($doc)->generate();
  }

  private function getConstructor() {
    $method = new MethodGenerator();

    $body = 'parent::__construct($this->type, $this->bundle);';

    return $method
      ->setName('__construct')
      ->setBody($body)
      ->setVisibility('public');
  }
}
