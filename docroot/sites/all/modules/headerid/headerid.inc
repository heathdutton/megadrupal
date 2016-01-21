<?php

/**
 * Interface for ID registry.
 */
interface IHeaderIdRegistry {
  /**
   * Add ID to registry.
   * @param $id
   */
  function add($id);

  /**
   * Checks if ID exists in the registry.
   *
   * @param string $id
   * @return bool
   */
  function exist($id);
}


/**
 * Class which servers as simple registry for Header IDs.
 */
class HeaderIdRegistry implements IHeaderIdRegistry {

  protected $registry = array();

  /**
   * @see IHeaderIdRegistry::add
   */
  public function add($id) {
    $this->registry[$id] = 1;
  }

  /**
   * @see IHeaderIdRegistry::exists
   */
  public function exist($id) {
    return isset($this->registry[$id]);
  }
}


/**
 * The class is responsible for generating unique IDs.
 */
class HeaderIdGenerator {

  protected $default_prefix = 'h';

  protected $used_prefixes = array();

  protected $registry = NULL;

  public function __construct(IHeaderIdRegistry $registry, $default_prefix = '') {
    $this->registry = $registry;
    if ($default_prefix) {
      $this->setDefaultPrefix($default_prefix);
    }
  }

  /**
   * Setter for default prefix.
   *
   * @param string $default_prefix
   * @throws InvalidArgumentException
   */
  public function setDefaultPrefix($default_prefix) {
    if ($default_prefix == '') {
      throw InvalidArgumentException('Default ID prefix should be non-empty.');
    }
    elseif (preg_match('~^\d~', $default_prefix)) {
      throw InvalidArgumentException('Default ID prefix should not begin with number.');
    }
    $this->default_prefix = $default_prefix;
  }

  /**
   * Checks if ID was used before.
   *
   * @param $id
   * @return bool
   */
  public function exist($id) {
    return $this->registry->exist($id);
  }

  /**
   * Adds ID to registry - mark as used.
   *
   * @param $id
   */
  public function add($id) {
    $this->registry->add($id);
  }

  /**
   * Generates new unique ID.
   *
   * @param string $prefix
   * @param bool $include_zero
   * @return string
   */
  public function generate($prefix = '', $include_zero = TRUE) {
    do {
      $new_id = $this->nextSequenceId($prefix);
    }
    while($this->exist($new_id));

    return $new_id;
  }


  /**
   * Returns new ID and store it in the registry.
   *
   * @param string $prefix
   * @return string
   */
  public function create($prefix = '') {
    $new_id = $this->generate($prefix);
    $this->add($new_id);
    return $new_id;
  }

  /**
   * Generates new unique ID.
   *
   * @param string $prefix
   * @param bool $include_zero
   * @return string
   */
  protected function nextSequenceId($prefix = '', $include_zero = TRUE) {
    if ($prefix == '') {
      $prefix = $this->default_prefix;
    }
    $counter = $this->increment($prefix);
    return $include_zero || $counter ? $prefix . $counter : $prefix;
  }

  /**
   * Increment ID sequence.
   *
   * @param string  $prefix
   * @return int
   */
  protected function increment($prefix) {
    if (isset($this->used_prefixes[$prefix])) {
      $this->used_prefixes[$prefix]++;
    }
    else {
      $this->used_prefixes[$prefix] = 1;
    }
    return $this->used_prefixes[$prefix];
  }
}
