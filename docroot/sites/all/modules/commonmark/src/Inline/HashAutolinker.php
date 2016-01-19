<?php
/**
 * @file
 * Contains \Drupal\CommonMark\Inline\HashAutolinker.
 */

namespace Drupal\CommonMark\Inline;

use Drupal\CommonMark\Extension;
use League\CommonMark\ContextInterface;
use League\CommonMark\Inline\Element\Link;
use League\CommonMark\Inline\Parser\InlineParserInterface;
use League\CommonMark\InlineParserContext;

/**
 * Class HashAutolinker.
 *
 * @package Drupal\CommonMark\Inline
 */
class HashAutolinker extends Extension implements InlineParserInterface {

  /**
   * {@inheritdoc}
   */
  public function getCharacters() {
    return ['#'];
  }

  /**
   * {@inheritdoc}
   */
  public function getName() {
    $reflection = new \ReflectionClass($this);
    return $reflection->getShortName();
  }

  /**
   * {@inheritdoc}
   */
  public function parse(ContextInterface $context, InlineParserContext $inline_context) {
    $cursor = $inline_context->getCursor();

    // The # symbol must not have any other characters immediately prior.
    $previous_char = $cursor->peek(-1);
    if ($previous_char !== NULL && $previous_char !== ' ' && $previous_char !== '[') {
      // peek() doesn't modify the cursor, so no need to restore state first.
      return FALSE;
    }

    // Save the cursor state in case we need to rewind and bail.
    $previous_state = $cursor->saveState();

    // Advance past the # symbol to keep parsing simpler.
    $cursor->advance();

    // Parse the handle.
    $text = $cursor->match('/^[^\s\]]+/');
    $url = FALSE;
    $title = FALSE;

    $type = $this->getSetting('type');
    if ($type === 'node' && is_numeric($text) && ($node = node_load($text))) {
      $url = url("node/$node->nid", ['absolute' => TRUE]);
      if ($this->getSetting('node_title') && !empty($node->title)) {
        $text = $node->title;
      }
      else {
        $text = "#$text";
      }
    }
    elseif ($type === 'url' && ($url = $this->getSetting('url')) && strpos($url, '[text]') !== FALSE) {
      $url = str_replace('[text]', $text, $url);
      if ($this->getSetting('url_title') && ($title = $this->getUrlTitle($url))) {
        $text = $title;
        $title = FALSE;
      }
    }
    else {
      $text = FALSE;
    }

    // Regex failed to match; this isn't a valid @ handle.
    if (empty($text) || empty($url)) {
      $cursor->restoreState($previous_state);
      return FALSE;
    }

    $inline_context->getInlines()->add(new Link($url, $text, $title));

    return TRUE;
  }

  /**
   * Retrieves a URL page title.
   *
   * @param string $url
   *   The URL to retrieve the title from.
   *
   * @return string|FALSE
   *   The URL title or FALSE if it could not be retrieved.
   */
  protected function getUrlTitle($url) {
    $request = drupal_http_request($url);
    if (!isset($request->error) && isset($request->data) && preg_match("/<title>(.*)<\\/title>/siU", $request->data, $matches)) {
      return check_plain(trim(preg_replace('/\s+/', ' ', $matches[1])));
    }
    return FALSE;
  }

}
