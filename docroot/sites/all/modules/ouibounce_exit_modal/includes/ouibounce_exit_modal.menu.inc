<?php
/**
 * @file
 * Menu callbacks.
 */


/**
 * Configuration form for controlling the "ouibounce_exit_modal" behaviour.
 */
function ouibounce_exit_modal_settings_form() {

  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $blocks = array();
  foreach (module_implements('block_info') as $module) {
    $module_blocks = module_invoke($module, 'block_info');
    if (is_array($module_blocks)) {
      foreach ($module_blocks as $module_key => $info) {
        $blocks[drupal_ucfirst($module)][$module . '|' . $module_key] = $info['info'];
      }
    }
  }
  ksort($blocks);
  foreach($blocks as &$subarray) {
    asort($subarray);
  }

  $form['content']['ouibounce_exit_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Modal title'),
    '#default_value' => variable_get('ouibounce_exit_modal_title', ''),
  );

  $form['content']['ouibounce_exit_modal_footer'] = array(
    '#type' => 'textfield',
    '#title' => t('Modal Footer text'),
    '#default_value' => variable_get('ouibounce_exit_modal_footer', 'No, thanks!'),
  );

  $form['content']['ouibounce_exit_modal_body'] = array(
    '#type' => 'select',
    '#title' => t('Select a block/view to display in modal'),
    '#options' => $blocks,
    '#default_value' => variable_get('ouibounce_exit_modal_body', ''),
  );

  $form['ouibounce'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ouibounce JavaScript settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['ouibounce']['ouibounce_exit_modal_sensitivity'] = array(
    '#type' => 'textfield',
    '#title' => t('Sensitivity'),
    '#default_value' => variable_get('ouibounce_exit_modal_sensitivity', 20),
    '#description' => t('Ouibounce fires when the mouse cursor moves close to (or passes) the top of the viewport. You can define how far the mouse has to be before Ouibounce fires. The higher value, the more sensitive, and the more quickly the event will fire. Defaults to 20.'),
    '#element_validate' => array('element_validate_number'),
    '#size' => 5,
  );

  $form['ouibounce']['ouibounce_exit_modal_aggressive_mode'] = array(
    '#type' => 'select',
    '#title' => t('Aggressive mode'),
    '#options' => array(
      1 => t('Enabled'),
      0 => t('Disabled'),
    ),
    '#default_value' => variable_get('ouibounce_exit_modal_aggressive_mode', 1),
    '#description' => t('By default, Ouibounce will only fire once for each visitor. When Ouibounce fires, a cookie is created to ensure a non obtrusive experience. There are cases, however, when you may want to be more aggressive (as in, you want the modal to be elegible to fire anytime the page is loaded/ reloaded). An example use-case might be on your paid landing pages. If you enable aggressive, the modal will fire any time the page is reloaded, for the same user.'),
  );

  $form['ouibounce']['ouibounce_exit_modal_timer'] = array(
    '#type' => 'textfield',
    '#title' => t('Set a min time before Ouibounce fires'),
    '#default_value' => variable_get('ouibounce_exit_modal_timer', 1000),
    '#description' => t("By default, Ouibounce won't fire in the first second to prevent false positives, as it's unlikely the user will be able to exit the page within less than a second. If you want to change the amount of time that firing is surpressed for, you can pass in a number of milliseconds to timer."),
    '#element_validate' => array('element_validate_number'),
    '#size' => 5,
    '#field_suffix' => t('millisecond(s)'),
  );

  $form['ouibounce']['ouibounce_exit_modal_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay'),
    '#default_value' => variable_get('ouibounce_exit_modal_delay', 0),
    '#description' => t("By default, Ouibounce will show the modal immediately. You could instead configure it to wait x milliseconds before showing the modal. If the user's mouse re-enters the body before delay ms have passed, the modal will not appear. This can be used to provide a 'grace period' for visitors instead of immediately presenting the modal window."),
    '#element_validate' => array('element_validate_number'),
    '#size' => 5,
    '#field_suffix' => t('millisecond(s)'),
  );

  $form['ouibounce']['ouibounce_exit_modal_cookie_expiration'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie expiration'),
    '#default_value' => variable_get('ouibounce_exit_modal_cookie_expiration', 0),
    '#description' => t("Ouibounce sets a cookie by default to prevent the modal from appearing more than once per user. You can add a cookie expiration (in days) using cookieExpire to adjust the time period before the modal will appear again for a user. By default, the cookie will expire at the end of the session, which for most browsers is when the browser is closed entirely."),
    '#element_validate' => array('element_validate_number'),
    '#size' => 5,
    '#field_suffix' => t('day(s)'),
  );

  $form['ouibounce']['ouibounce_exit_modal_cookie_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie domain'),
    '#default_value' => variable_get('ouibounce_exit_modal_cookie_domain', ''),
    '#description' => t('Ouibounce sets a cookie by default to prevent the modal from appearing more than once per user. You can add a cookie domain using cookieDomain to specify the domain under which the cookie should work. By default, no extra domain information will be added. If you need a cookie to work also in your subdomain (like blog.example.com and example.com), then set a cookieDomain such as .example.com (notice the dot in front).'),
  );

  $form['ouibounce']['ouibounce_exit_modal_cookie_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie name'),
    '#default_value' => variable_get('ouibounce_exit_modal_cookie_name', 'viewedOuibounceModal'),
    '#description' => t('You can specify custom cookie name.'),
  );

  $form['ouibounce']['ouibounce_exit_modal_sitewide_cookie'] = array(
    '#type' => 'select',
    '#title' => t('Sitewide cookie'),
    '#options' => array(
      1 => t('Enabled'),
      0 => t('Disabled'),
    ),
    '#default_value' => variable_get('ouibounce_exit_modal_sitewide_cookie', 0),
    '#description' => t('You can drop sitewide cookies by enabling this.'),
  );

  return system_settings_form($form);
}
