<?php
/**
 * @file
 * Contains Drupal\CommonMark\LinkRenderer.
 */

namespace Drupal\CommonMark\Inline;

use Drupal\CommonMark\Extension;
use League\CommonMark\ElementRendererInterface;
use League\CommonMark\HtmlElement;
use League\CommonMark\Inline\Element\AbstractInline;
use League\CommonMark\Inline\Element\Link;
use League\CommonMark\Inline\Renderer\InlineRendererInterface;

/**
 * Class LinkRenderer.
 *
 * @package Drupal\CommonMark
 */
class LinkRenderer extends Extension implements InlineRendererInterface {

  /**
   * {@inheritdoc}
   */
  public function render(AbstractInline $inline, ElementRendererInterface $html_renderer) {
    if (!($inline instanceof Link)) {
      throw new \InvalidArgumentException('Incompatible inline type: ' . get_class($inline));
    }

    $attributes = [];
    foreach ($inline->getData('attributes', []) as $key => $value) {
      $attributes[$key] = $html_renderer->escape($value, TRUE);
    }

    // Retrieve the URL.
    $url = $inline->getUrl();
    $external = $this->isExternalUrl($url);
    $attributes['href'] = $html_renderer->escape($url, TRUE);

    // Make external links open in a new window.
    if ($this->getSetting('external_new_window') && $external) {
      $attributes['target'] = '_blank';
    }

    // rel="nofollow"
    $no_follow = $this->getSetting('no_follow');
    if ($no_follow === 'all' || ($external && $no_follow === 'external') || (!$external && $no_follow === 'internal')) {
      $attributes['rel'] = 'nofollow';
    }

    if (isset($inline->data['title'])) {
      $attributes['title'] = $html_renderer->escape($inline->data['title'], TRUE);
    }

    return new HtmlElement('a', $attributes, $html_renderer->renderInlines($inline->getChildren()));
  }

  /**
   * Determines if a URL is external to current host.
   *
   * @param string $url
   *   The URL to verify.
   *
   * @return bool
   *   TRUE or FALSE
   */
  private function isExternalUrl($url) {
    $url_host = parse_url($url, PHP_URL_HOST);

    // Only process URLs that actually have a host (e.g. not fragments).
    if (!isset($url_host) || empty($url_host)) {
      return FALSE;
    }

    // The environment can be reset, this too would be reset and would re-parse
    // the hosts again. Save some time during the same environment instance.
    static $hosts;

    // Parse the whitelist of internal hosts.
    if (!isset($hosts)) {
      $hosts = preg_split("/\r\n|\n/", $this->getSetting('internal_host_whitelist'), -1, PREG_SPLIT_NO_EMPTY);

      // Ensure that the site's base url host name is always in this whitelist.
      $base_host = parse_url($GLOBALS['base_url'], PHP_URL_HOST);
      $key = array_search($base_host, $hosts);
      if ($key === FALSE) {
        $hosts[] = $base_host;
      }
    }

    // Iterate through the internal host whitelist.
    $internal = FALSE;
    foreach ($hosts as $host) {
      if ($host === $url_host) {
        $internal = TRUE;
        break;
      }
    }

    return !$internal;
  }


}
