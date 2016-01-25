<?php

class WebformCapturePlusMessageCatalogue implements WebformCapturePlusMessageCatalogueInterface {

  private $messages = array();
  private $locale;
  private $fallbackCatalogue;

  /**
   * Constructor.
   *
   * @param string $locale   The locale
   * @param array  $messages An array of messages classified by domain
   *
   * @api
   */
  public function __construct($locale, array $messages = array()) {
    $this->locale = $locale;
    $this->messages = $messages;
  }

  /**
   * {@inheritdoc}
   */
  public function getLocale() {
    return $this->locale;
  }

  /**
   * {@inheritdoc}
   */
  public function getDomains() {
    return array_keys($this->messages);
  }

  /**
   * {@inheritdoc}
   */
  public function all($domain = null) {
    if (null === $domain) {
      return $this->messages;
    }

    return isset($this->messages[$domain]) ? $this->messages[$domain] : array();
  }

  /**
   * {@inheritdoc}
   */
  public function set($id, $translation, $domain = 'messages') {
    $this->add(array($id => $translation), $domain);
  }

  /**
   * {@inheritdoc}
   *
   * @api
   */
  public function has($id, $domain = 'messages') {
    if (isset($this->messages[$domain][$id])) {
      return true;
    }

    if (null !== $this->fallbackCatalogue) {
      return $this->fallbackCatalogue->has($id, $domain);
    }
    return false;
  }

  /**
   * {@inheritdoc}
   */
  public function defines($id, $domain = 'messages') {
    return isset($this->messages[$domain][$id]);
  }

  /**
   * {@inheritdoc}
   */
  public function get($id, $domain = 'messages') {
    if (isset($this->messages[$domain][$id])) {
      return $this->messages[$domain][$id];
    }

    if (null !== $this->fallbackCatalogue) {
      return $this->fallbackCatalogue->get($id, $domain);
    }

    return $id;
  }

  /**
   * {@inheritdoc}
   */
  public function replace($messages, $domain = 'messages') {
    $this->messages[$domain] = array();

    $this->add($messages, $domain);
  }

  /**
   * {@inheritdoc}
   */
  public function add($messages, $domain = 'messages') {
    if (!isset($this->messages[$domain])) {
      $this->messages[$domain] = $messages;
    } else {
      $this->messages[$domain] = array_replace($this->messages[$domain], $messages);
    }
  }

  /**
   * {@inheritdoc}
   *
   * @api
   */
  public function setFallbackCatalogue(WebformCapturePlusMessageCatalogueInterface $catalogue) {
    $this->fallbackCatalogue = $catalogue;
  }

  /**
   * {@inheritdoc}
   *
   * @api
   */
  public function getFallbackCatalogue() {
    return $this->fallbackCatalogue;
  }

}

interface WebformCapturePlusMessageCatalogueInterface {

  /**
   * Gets the catalogue locale.
   *
   * @return string The locale
   *
   * @api
   */
  public function getLocale();

  /**
   * Gets the domains.
   *
   * @return array An array of domains
   *
   * @api
   */
  public function getDomains();

  /**
   * Gets the messages within a given domain.
   *
   * If $domain is null, it returns all messages.
   *
   * @param string $domain The domain name
   *
   * @return array An array of messages
   *
   * @api
   */
  public function all($domain = null);

  /**
   * Sets a message translation.
   *
   * @param string $id          The message id
   * @param string $translation The messages translation
   * @param string $domain      The domain name
   *
   * @api
   */
  public function set($id, $translation, $domain = 'messages');

  /**
   * Checks if a message has a translation.
   *
   * @param string $id     The message id
   * @param string $domain The domain name
   *
   * @return bool true if the message has a translation, false otherwise
   *
   * @api
   */
  public function has($id, $domain = 'messages');

  /**
   * Checks if a message has a translation (it does not take into account the fallback mechanism).
   *
   * @param string $id     The message id
   * @param string $domain The domain name
   *
   * @return bool true if the message has a translation, false otherwise
   *
   * @api
   */
  public function defines($id, $domain = 'messages');

  /**
   * Gets a message translation.
   *
   * @param string $id     The message id
   * @param string $domain The domain name
   *
   * @return string The message translation
   *
   * @api
   */
  public function get($id, $domain = 'messages');

  /**
   * Sets translations for a given domain.
   *
   * @param array  $messages An array of translations
   * @param string $domain   The domain name
   *
   * @api
   */
  public function replace($messages, $domain = 'messages');

  /**
   * Adds translations for a given domain.
   *
   * @param array  $messages An array of translations
   * @param string $domain   The domain name
   *
   * @api
   */
  public function add($messages, $domain = 'messages');

  /**
   * Sets fallback catalogue.
   *
   * @param WebformCapturePlusMessageCatalogueInterface $catalogue A WebformCapturePlusMessageCatalogueInterface instance
   *
   * @api
   */
  public function setFallbackCatalogue(WebformCapturePlusMessageCatalogueInterface $catalogue);

  /**
   * Gets the fallback catalogue.
   *
   * @return WebformCapturePlusMessageCatalogueInterface|null A WebformCapturePlusMessageCatalogueInterface instance or null when no fallback has been set
   *
   * @api
   */
  public function getFallbackCatalogue();
}
