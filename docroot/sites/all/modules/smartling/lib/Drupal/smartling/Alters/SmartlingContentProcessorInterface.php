<?php

/**
 * @file
 * Interface IContentProcessorSmartlingInterface.
 */

namespace Drupal\smartling\Alters;

/*
 * Content Processor interface. It's implementations allow to change
 * translated content. For example, to
 * rewrite paths and links to respect language translated nodes. Or to
 * substitute files from media module to translated ones.
 */
interface SmartlingContentProcessorInterface {

  /**
   * Changes string content according to the other params.
   *
   * @param string $item
   *   Item string.
   * @param mixed $context
   *   Context array.
   * @param string $lang
   *   Locale in drupal format (ru, en).
   * @param string $field_name
   *   Field name.
   * @param object $entity
   *   Entity object.
   */
  public function process(&$item, $context, $lang, $field_name, $entity);
}
