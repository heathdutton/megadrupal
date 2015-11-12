<?php
/**
 * Base class for displaying context conditions
 */
class ContextListConditionDisplay {

  /**
   * The condition key that is being displayed
   * @var string
   */
  public $condition = NULL;

  /**
   * The settings for this condition
   * @var array
   */
  public $settings = NULL;

  /**
   * The constructor for this class
   * @param string $condition The condition identifier
   * @param array  $settings  [description]
   */
  function __construct($condition = NULL, $settings = array()) {
    $this->condition = $condition;
    $this->settings = $settings;
  }

  /**
   * Display the condition
   * @return string
   */
  function display() {
    return $this->condition;
  }
}

/**
 * A condition display class for displaying conditions that have a
 * values key in the settings.
 *
 * This class is used by a couple of different conditions.
 */
class ContextListConditionDisplayValues extends ContextListConditionDisplay {
  function display() {
    if (isset($this->settings['values']) && is_array($this->settings['values'])){
      $values = $this->settings['values'];
      $values = array_map('htmlentities', $values);
      return $this->condition . ': ' . implode(', ', $values);
    }

    parent::display();
  }
}

/**
 * Condition display class for defaultcontent module
 */
class ContextListConditionDisplay_defaultcontent extends ContextListConditionDisplayValues {
}

/**
 * Condition display class for path conditions
 */
class ContextListConditionDisplay_path extends ContextListConditionDisplayValues {
}

/**
 * The base class for outputting context reactions
 */
class ContextListReactionDisplay {
  /**
   * The reaction key for this reaction
   * @var string
   */
  public $reaction = NULL;

  /**
   * The settings array for this reaction
   * @var array
   */
  public $settings = NULL;

  /**
   * The constructor for this reaction display
   * @param string $reaction The context's reaction
   * @param array  $settings The context reaction settings
   */
  function __construct($reaction = NULL, $settings = array()) {
    $this->reaction = $reaction;
    $this->settings = $settings;
  }

  /**
   * Display the context reaction
   * @return string
   */
  function display() {
    $d = $this->reaction . ": ";
    if (is_array($this->settings)) {
      $d .= implode(', ', array_map('htmlentities', $this->settings));
    }
    else {
      $d .= htmlentities($this->settings);
    }
    return $d;
  }
}

/**
 * The reaction display class for blocks
 */
class ContextListReactionDisplay_block extends ContextListReactionDisplay {
  function display() {
    $reaction_details = array();

    foreach ($this->settings['blocks'] as $block => $details) {
      $block_link = $details['module'] . ': ' .
        l($block, 'admin/structure/block/manage/' . $details['module'] . '/' . $details['delta'] . '/configure');

      switch ($details['module']){
        case 'menu':
          $block_link .= ' ('. l('Edit', 'admin/structure/menu/manage/'. $details['delta']) .')';
          break;
        case 'views':
          $view = str_replace('-', '/', $details['delta']);
          $block_link .= ' ('. l('Edit', 'admin/structure/views/view/' . $view) .')';
          break;
      }

      array_push(
        $reaction_details,
        $block_link
      );
    }
    $list = array(
      '#theme' => 'item_list',
      '#items' => $reaction_details,
    );
    $rendered_list = '<p>Blocks:</p>' . drupal_render($list);
    return $rendered_list;
  }
}

/**
 * Implements hook_context_list_register_condition_display().
 */
function context_list_context_list_register_condition_display() {
  return array(
    'all' => 'ContextListConditionDisplay',
    'defaultcontent' => 'ContextListConditionDisplay_defaultcontent',
    'path' => 'ContextListConditionDisplay_path',
  );
}

/**
 * Implements hook_context_list_register_reaction_display().
 */
function context_list_context_list_register_reaction_display() {
  return array(
    'all' => 'ContextListReactionDisplay',
    'block' => 'ContextListReactionDisplay_block',
  );
}