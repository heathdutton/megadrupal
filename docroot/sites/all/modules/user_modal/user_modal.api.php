<?php

/**
 * @file
 * Hooks provided by the User modal module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Change the redirection after a successful login/ registration.
 *
 * @param $data
 *   Array passed by reference with the following keys:
 *   - redirect: (optional) The path that should open in the parent window after
 *     the overlay closes. If not set, no redirect will be performed on the
 *     parent window. Defaults to NULL.
 *   - redirect_options: (optional) An associative array of options to use when
 *     generating the redirect URL.
 *   - trigger_refresh: (optional) Determine if the parent should refresh after
 *     the modal closes. Property is checked only if the "redirect" property is
 *     NULL. Defaults to TRUE.
 * @param $context
 *   Array with the $form_state, of the successful form submittion.
 */
function hook_user_modal_alter(&$data, $context1) {
  $form_state = $context['form state'];
  // If the user registered, redirect to "welcome page".
  if ($form_state['selected tab'] == 'register') {
    $data['redirect'] = 'welcome-page';
  }
}

/**
 * @} End of "addtogroup hooks".
 */
