<?php
/**
 * @file
 * Context Plugin for access to premium content, to allow CCK blocks.
 * Denotes recent submittal of reg form or single-button form.
 */

/**
* Setup the condition, based from node (for the form option).
*/
class premium_access_condition extends context_condition {
  function condition_values() {
    // Some value save is required to attach the condition. Must be a way around
    // it when checking context_condition_path.inc, but not worth the trouble.
    return array(1 => t('Confirm user access via submission (just check it).'));
  }

  // Adding a value option so we can check for positive or negative results.
  function options_form($context) {
    $values = $this->fetch_from_context($context, 'options');
    $form = array();
    $form["submitted_value"] = array(
      '#description' => t('User submitted for this node'),
      '#type' => 'radios',
      '#options'=> array(0=>'No', 1=>'Yes'),
      '#default_value' => isset($values["submitted_value"]) ? $values["submitted_value"] : FALSE,
    );
    return $form;
  }
  function options_form_sumbit($values) {
  }

  function execute($node) {
    foreach ($this->get_contexts() as $context) {
      // Check the node form option.
      $option = $this->fetch_from_context($context, 'options');
      $submitted = _premium_access_getflag($node->nid);
      if ($submitted == $option['submitted_value']) {
        $this->condition_met($context);
      }
    }
  }
}
?>
