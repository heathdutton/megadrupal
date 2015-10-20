<?php

/**
 * @file
 * Smartling content base parser.
 */

namespace Drupal\smartling\Alters;

use Drupal\smartling\Alters\SmartlingContentParserInterface;

/**
 * Abstract class SmartlingContentBaseParser.
 */
abstract class SmartlingContentBaseParser implements SmartlingContentParserInterface {
  protected $regexp = '';
  protected $processors;
  protected $fieldName;
  protected $entity;

  /**
   * Init.
   *
   * @param array $processors
   *   Processors.
   */
  public function __construct(array $processors) {
    $this->processors = $processors;
  }


  /**
   * Determines additional context based on the content.
   *
   * @param array $matches
   *   Array of strings that matched the regexp.
   *
   * @return array
   *   Return content.
   */
  protected abstract function getContext(array $matches);


  /**
   * Processes each item that was found by a regexp in a parse method.
   *
   * @param array $match
   *   Match.
   *
   * @return string
   *   Return executor.
   */
  protected function processorExecutor(array $match) {
    $context = $this->getContext($match);

    foreach ($this->processors as $processor) {
      $processor->process($match, $context, $this->lang, $this->fieldName, $this->entity);
    }

    return $match[1];
  }

  /**
   * Parses the translated content.
   *
   * Parses the translated content and
   * passes items that were found to a processor.
   *
   * @param string $content
   *   Content.
   * @param string $lang
   *   Language.
   * @param string $field_name
   *   Field name.
   * @param object $entity
   *   Entity object.
   *
   * @return string
   *   Return content.
   */
  public function parse($content, $lang, $field_name, $entity) {
    $this->fieldName = $field_name;
    $this->entity = $entity;
    $this->lang = $lang;

    $content = preg_replace_callback($this->regexp, array(
      $this,
      'processorExecutor',
    ), $content);

    return $content;
  }
}
