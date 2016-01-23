<?php
/**
 * @file
 * A plugin automatically declaring the set of paths of the Classified Ads
 * module as context path conditions
 *
 * @copyright (c) 2010 Ouest Systemes Informatiques (OSInet)
 *
 * @author Frederic G. MARAND <fgm@osinet.fr>
 *
 * @license General Public License version 2 or later
 *
 * Original code, not derived from the ed_classified module.
 */

/**
 * Merge Classified Ads paths within a Context path condition.
 */
class classified_context_condition_path extends context_condition_path {

  protected function get_classified_paths($assoc = FALSE) {
    $ret = array(
      'admin/config/content/classified',
      'admin/config/content/classified/*',
      'classified',
      'classified/*',
      'user/*/classified',
      'user/*/classified/*',
    );

    if ($assoc) {
      $ret = drupal_map_assoc($ret);
    }
    return $ret;
  }

  /**
   * Condition form.
   *
   * @return array
   *   A form array for the condition.
   */
  function condition_form($context) {
    $form = array();
    $form['1'] = parent::condition_form($context);
    $all_paths = explode("\n", $form['1']['#default_value']);
    $classified_paths = $this->get_classified_paths();
    sort($classified_paths);
    $common_paths = array_values(array_intersect($all_paths, $classified_paths));
    sort($common_paths);
    $form['2'] = array(
      '#title' => t('Auto Classified path set'),
      '#description' => t('Helper: merge all paths belonging to the Classified Ads application into the above list.'),
      '#type' => 'checkbox',
      '#default_value' => $common_paths == $classified_paths,
    );
    return $form;
  }

  /**
   * Condition form submit handler.
   *
   * Merge the Classified Ads paths into the manually entered paths list.
   *
   * @return array
   *   An array of paths on which the condition will hold.
   */
  function condition_form_submit($values) {
    $auto_path = array_pop($values);
    $values = reset($values);
    $paths = parent::condition_form_submit($values);
    if ($auto_path) {
      $paths = array_merge($paths, $this->get_classified_paths(TRUE));
      sort($paths);
    }
    // Coder limitation: unlike usual FAPI, Context submit handlers return data.
    return $paths;
  }
}
