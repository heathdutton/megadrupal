<?php
/**
 * @file
 * Context integration for Flags applied to taxonomy term pages.
 */

/**
 * Taxonomy Term Page Active Flag condition.
 *
 * @see ContextFlagBase
 * @see context_condition
 */
class ContextFlagTaxonomyPageActive extends ContextFlagBase {
  /**
   * Construct this context_condition and apply settings specific to it.
   */
  public function __construct() {
    $registry = context_flag_context_registry();
    $key = 'flag_taxonomy_page_active';
    $this->setPropertyValue('contextSettingName', $key);
    $this->setPropertyValue('plugin', $registry['conditions'][$key]['plugin']);
    $this->setPropertyValue('title', $registry['conditions'][$key]['title']);
    $this->setPropertyValue('description', $registry['conditions'][$key]['description']);
    $this->setPropertyValue('flagType', 'taxonomy_term');
  }
  /**
   * Logic for determining if this context_condition should be triggered.
   */
  public function contextFlagConditionCheck() {
    if ($this->condition_used()) {
      $obj_page = menu_get_object('taxonomy_term', 2);
      if (!empty($obj_page->tid)) {
        $flags = $this->getFlagsForType();
        $arr_flags = array();
        foreach ($flags as $flag) {
          $obj_flag = flag_get_flag($flag->name);
          $arr_flags[$flag->name] = $obj_flag->is_flagged($obj_page->tid);
        }
        foreach ($this->get_contexts() as $context) {
          $options = $this->fetch_from_context($context, 'options');
          $require_all = isset($options[$this->contextSettingName]) ? $options[$this->contextSettingName] : FALSE;
          $arr_trigger_flags = $context->conditions[$this->contextSettingName]['values'];
          if (!$require_all) {
            foreach ($arr_trigger_flags as $flag_name) {
              if (isset($arr_flags[$flag_name]) && $arr_flags[$flag_name]) {
                $this->condition_met($context);
              }
            }
          }
          else {
            $is_all_active = TRUE;
            foreach ($arr_trigger_flags as $delta => $flag_name) {
              if (!$arr_flags[$delta]) {
                $is_all_active = FALSE;
              }
            }
            if ($is_all_active) {
              $this->condition_met($context);
            }
          }
        }
      }
    }
  }

}
