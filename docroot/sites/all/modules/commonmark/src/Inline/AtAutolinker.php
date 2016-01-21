<?php
/**
 * @file
 * Contains \Drupal\CommonMark\Inline\AtAutolinker.
 */

namespace Drupal\CommonMark\Inline;

use Drupal\CommonMark\Extension;
use League\CommonMark\ContextInterface;
use League\CommonMark\Inline\Element\Link;
use League\CommonMark\Inline\Parser\InlineParserInterface;
use League\CommonMark\InlineParserContext;

/**
 * Class AtAutolinker.
 *
 * @package Drupal\CommonMark\Inline
 */
class AtAutolinker extends Extension implements InlineParserInterface {

  /**
   * {@inheritdoc}
   */
  public function getCharacters() {
    return ['@'];
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

    // The @ symbol must not have any other characters immediately prior.
    $previous_char = $cursor->peek(-1);
    if ($previous_char !== NULL && $previous_char !== ' ') {
      // peek() doesn't modify the cursor, so no need to restore state first.
      return FALSE;
    }

    // Save the cursor state in case we need to rewind and bail.
    $previous_state = $cursor->saveState();

    // Advance past the @ symbol to keep parsing simpler.
    $cursor->advance();

    // Parse the handle.
    $text = $cursor->match('/^[^\s]+/');
    $url = '';
    $title = '';

    $type = $this->getSetting('type');
    if ($type === 'user') {
      $user = user_load_by_name($text);
      if ((!$user || !$user->uid) && is_numeric($text)) {
        $user = user_load((int) $text);
      }
      if ($user && $user->uid) {
        $url = url("user/$user->uid", ['absolute' => TRUE]);
        $title = t('View user profile.');
        if ($this->getSetting('format_username')) {
          $text = format_username($user);
        }
      }
      else {
        $text = FALSE;
      }
    }
    elseif ($type === 'url' && ($url = $this->getSetting('url')) && strpos($url, '[text]') !== FALSE) {
      $url = str_replace('[text]', $text, $url);
    }
    else {
      $text = FALSE;
    }

    // Regex failed to match; this isn't a valid @ handle.
    if (empty($text) || empty($url)) {
      $cursor->restoreState($previous_state);
      return FALSE;
    }

    $inline_context->getInlines()->add(new Link($url, '@' . $text, $title));

    return TRUE;
  }

}
