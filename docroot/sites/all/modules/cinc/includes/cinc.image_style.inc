<?php

/**
 * Creates image styles in code.
 *
 * @code
 * $my_image_style = CINC::init('ImageStyle')->set('name', 'my_image_style');
 * @endcode
 */
class CINC_ImageStyle extends CINC implements CINC_CRUD {

  public static $dependencies = array('image');
  public $config_machine_name = 'name';
  public $cinc_yaml_prefix = 'image.style';

  function __construct() {

    $this->config_filters['name'] = 'set_name';
    $this->config_filters['effects'] = 'set_effects';

    $this->config = array(
      'name' => '',
      'label' => '',
      'effects' => array(),
    );

  }

  /**
   * Auto-adds label when name is set, and cleans name.
   */
  public function set_name(&$name) {
    $name = CINC::clean_machine_name($name);
    if (empty($this->config['label'])) {
      $this->set('label', CINC::display_name_from_machine_name($name));
    }
    return TRUE;
  }

  /**
   * Converts effects array into effects objects.
   */
  public function set_effects(&$effects) {

    $this->create();

    foreach ($effects as $effect) {

      $effect_cinc = CINC::init('ImageEffect');

      if (isset($effect['name'])) {
        $effect_cinc->machine_name($effect['name']);
        unset($effect['name']);
      }
      elseif (isset($effect['id'])) {
        $effect_cinc->machine_name($effect['id']);
        unset($effect['id']);
      }

      foreach ($effect as $property => $value) {
        $effect_cinc->set($property, $value);
      }

      $this->add_effect($effect_cinc);
      $effect_cinc->create();

    }

    return FALSE;
  }

  /**
   * Saves image style to the database.
   */
  public function create() {
    $this->config = image_style_save($this->config);
    return $this;
  }

  /**
   * Tests if content type exists.
   */
  public function exists() {
    $image_style = image_style_load($this->config['name']);
    if ($image_style) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Loads image style from the database.
   */
  public function read() {
    $image_style = image_style_load($this->config['name']);
    if ($image_style) {
      $this->config = $image_style;
    }
    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {

    $list = array();

    $styles = image_styles();

    foreach ($styles as $machine_name => $style) {
      $list[$machine_name] = array(
        'display' => $machine_name,
      );
    }

    return $list;

  }

  /**
   * Saves image style to the database.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes image style from the database.
   */
  public function delete() {
    if (empty($this->config['isid'])) {
      $this->read();
    }
    image_style_delete($this->config, $replacement);
    return $this;
  }

  public function add_effect($effect) {
    $effect->add_to_style($this);
    $this->config['effects'][$effect->get('ieid')] = $effect->config;
    return $this;
  }

}

/**
 * Creates image effects in code.
 *
 * @code
 * $my_image_effect = CINC::init('ImageEffect')->set('name', 'my_image_effect');
 * @endcode
 */
class CINC_ImageEffect extends CINC implements CINC_CRUD {

  public $config_machine_name = 'name';

  function __construct() {

    $this->config_filters['name'] = 'set_name';

    $this->config = array(
      'name' => '',
      'weight' => 1,
      'data' => array()
    );

  }

  /**
   * Cleans name.
   */
  public function set_name(&$name) {
    $name = CINC::clean_machine_name($name);
    return TRUE;
  }

  /**
   * Saves image style to the database.
   */
  public function create() {
    $this->config = image_effect_save($this->config);
    return $this;
  }

  /**
   * Tests if content type exists.
   */
  public function exists() {
    $image_effect = image_effect_load($this->config['ieid'], $this->config['name']);
    if ($image_effect) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Loads image style from the database.
   */
  public function read() {
    $image_effect = image_effect_load($this->config['ieid'], $this->config['name']);
    if ($image_effect) {
      $this->config = $image_effect;
    }
    return $this;
  }

  /**
   * Saves image style to the database.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes image style from the database.
   */
  public function delete() {
    image_effect_delete($this->config);
    return $this;
  }

  public function add_to_style($style) {
    $this->config['isid'] = $style->config['isid'];
    return $this;
  }

}
