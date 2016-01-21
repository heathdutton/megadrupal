<?php

/**
 * @file
 * Interface IContentParserSmartlingInterface.
 */

namespace Drupal\smartling\Alters;

/**
 * Parses the translated content for the needed parts (like link addresses etc).
 *
 * And passes it to the relevant processors.
 */
interface SmartlingContentParserInterface {

  /**
   * Array of objects that implement IContentProcessorSmartlingInterface.
   *
   * @param array $processors
   *   Processors.
   */
  public function __construct(array $processors);

  /**
   * Parses string content after the translation is made.
   *
   * @param string $content
   *   Content strint.
   * @param string $lang
   *   Locale in drupal format (ru, en).
   * @param string $field_name
   *   Field name.
   * @param object $entity
   *   Entity object.
   */
  public function parse($content, $lang, $field_name, $entity);
}
