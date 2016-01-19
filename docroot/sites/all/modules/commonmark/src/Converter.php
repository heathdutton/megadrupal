<?php
/**
 * @file
 * Contains Drupal\CommonMark\CommonMark.
 */

namespace Drupal\CommonMark;

use League\CommonMark\DocParser;
use League\CommonMark\ElementRendererInterface;
use League\CommonMark\HtmlRenderer;

/**
 * Class Converter.
 *
 * @package Drupal\CommonMark
 */
class Converter extends \League\CommonMark\Converter {

  /**
   * The document parser instance.
   *
   * @var \League\CommonMark\DocParser
   */
  protected $docParser;

  /**
   * The current CommonMark Environment instance.
   *
   * @var \League\CommonMark\Environment
   */
  protected $environment;


  /**
   * The html renderer instance.
   *
   * @var \League\CommonMark\ElementRendererInterface
   */
  protected $htmlRenderer;

  /**
   * Creates a new Drupal\CommonMark\CommonMark instance.
   *
   * @param DocParser $parser
   *   The document parser instance.
   * @param ElementRendererInterface $renderer
   *   The html renderer instance.
   */
  public function __construct(DocParser $parser = NULL, ElementRendererInterface $renderer = NULL, $filter = NULL) {
    if (!isset($parser)) {
      $parser = new DocParser($this->getEnvironment($filter));
    }
    if (!isset($renderer)) {
      $renderer = new HtmlRenderer($this->getEnvironment($filter));
    }
    parent::__construct($parser, $renderer);
  }

  /**
   * Retrieves current CommonMark environment, creating it if necessary.
   *
   * @return \League\CommonMark\Environment
   *   The CommonMark Environment instance.
   */
  protected function getEnvironment($filter = NULL) {
    // Create the CommonMark environment.
    if (!isset($this->environment)) {
      $this->environment = Environment::createCommonMarkEnvironment($filter);
    }
    return $this->environment;
  }

}
