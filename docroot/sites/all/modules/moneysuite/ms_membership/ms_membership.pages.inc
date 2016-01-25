 <?php
/**
 * @file
 * MS Membership Pages functions
 */

/**
 * Page callback: List available memberships
 */
function ms_membership_list_memberships() {
	$build = array();
	$build['content'] = array('form' => drupal_get_form('ms_membership_list_memberships_form'));
  return $build;
}

function ms_membership_list_memberships_form($form, &$form_state) {
    $plans = ms_products_get_plans('ms_membership');
    $plan_options = array();
    foreach ($plans as $plan) {
      $register_arg = FALSE;

      if (!empty($plan->disabled) AND $plan->disabled) {
        continue;
      }

      $allowed_roles = $plan->allow_roles;
      if (is_array($allowed_roles)) {
        $allowed_roles = array_filter($allowed_roles);
      }
      // If the allowed roles setting is on and the authenticated user is not
      // checked, then skip this one.
      if (!empty($allowed_roles) AND (!isset($allowed_roles[DRUPAL_AUTHENTICATED_RID]) OR !$allowed_roles[DRUPAL_AUTHENTICATED_RID])) {
        // Only show plans that are available to authenticated users.
        continue;
      }

      $denied_roles = $plan->deny_roles;
      if (is_array($denied_roles)) {
        $denied_roles = array_filter($denied_roles);
      }
      // If the denied roles setting is on and the authenticated user is
      // checked, then skip this one.
      if (!empty($denied_roles) AND isset($denied_roles[DRUPAL_AUTHENTICATED_RID])) {
        // Only show plans that are not denied to authenticated users.
        continue;
      }

      $plan_options[$plan->sku] = ms_products_get_plan_option_text($plan);
    }
    if (!empty($plan_options)) {
      $options = array();

      foreach ($plan_options as $sku => $name) {
        $options[$sku] = $name;
      }

      $form['ms_membership'] = ms_products_plans_element($form_state, t('Choose your Membership Level'), '', $options, 'ms_membership', variable_get('ms_membership_force_signup', FALSE));
      $form['ms_membership']['#title'] = t('Membership Level');
      $form['ms_membership']['#weight'] = variable_get('ms_membership_registration_form_weight', 0);
	  $form['submit'] = array(
	      '#type' => 'submit',
	      '#value' => 'Submit',
	  );
	  $form['#submit'][] = 'ms_membership_register_form_after_submit';
      // Don't hook it if the settings are set.
      if ((isset($form_state['values']['ms_membership_paid']) AND $form_state['values']['ms_membership_paid'] == TRUE) || !isset($form['ms_membership'])) {
        return;
      }

      // Clean up
      unset($form_state['values']['ms_membership_submit'], $form_state['values']['ms_membership']['sku'], $form_state['values']['ms_membership_paid']);

    }
    return $form;
}
