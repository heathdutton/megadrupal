<?php

/**
 * @file
 * Example hook functions for hooks provided by the core regcode module.
 */

/**
 * Called when a registration code is used.
 */
function hook_regcode_used($code, $account) {
  if (is_object($code)) {
    drupal_set_message(t('Thanks %name, the code %code was used.', array(
      '%name' => $account->name,
      '%code' => $code['code'])));
  }
}


/**
 * Called when validating registration code use.
 */
function hook_regcode_validate($edit, $account) {
  if (empty($edit['loves_bacon'])) {
    form_set_error('regcode_code', t("This just isn't working out between us."));
  }
}


/**
 * Change the regcode data that is being loaded.
 *
 * @param array $code
 *   An array of reg codes, indexed by the registration code ID.
 *   Each element of the array is a regcode stdClass object. You can change the
 *   properties of individual reg codes.
 *
 *   For reasons of backward compatibility and historical reasons, this hook is
 *   called from two places.
 *
 *   Entity API module loads the data directly from the database into stdClass
 *   objects array, and invokes hook_regcode_load().
 *
 *   In older versions of this module, there was hook already to alter
 *   individual regcode values. Since the arguments of the hook implementations
 *   are not consistent, reg code module now calls the hook_regcode_load() hook
 *   as an array. Note that adding or removing regcodes will work only if the
 *   caller expects multiple objects.
 *
 *   If you need to replace and regcode that is being loaded, the array key
 *   should not be changed.
 *
 *   Note that tags are not loaded when the hook is invoked by the entity API
 *   module.
 */
function hook_regcode_load(array $code) {
  foreach ($code as $rid => $regcode) {
    /*
     * Do not change $rid or $regcode->rid.
     */

    // Feel free to change other properties.
    $regcode->group = 'foo';
  }
}


/**
 * Called to gather field information.
 */
function hook_regcode_fields() {
  return array(
    'group' => array(
      'description' => t('The group this code should be assigned to.'),
      'title'       => t('Group'),
    ),
  );
}


/**
 * Called when a new registration code is created.
 */
function hook_regcode_presave($code, &$terms = array()) {
  $term_string = implode(', ', $terms);
  drupal_set_message(
    t('The code: "%code" was created with the terms: "%terms!"', array(
        '%code' => $code,
        '%terms' => $term_string,
      )
    )
  );
}
