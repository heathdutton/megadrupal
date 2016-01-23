<?php

/**
 * @file
 * Provide all the administration pages
 */

/**
 * Form builder.
 */
function ajax_screen_lock_settings_form($form, $form_state) {

	$form['ajax_screen_lock_popup_title'] = array(
		'#title' => t('Title of screen lock popup'),
		'#type' => 'textfield',
		'#default_value' => variable_get('ajax_screen_lock_popup_title', t('Loading... wait please')),
	);

	$form['ajax_screen_lock_popup_timeout'] = array(
		'#title' => t('Timeout of displaying screen lock popup in ms'),
		'#type' => 'textfield',
		'#default_value' => variable_get('ajax_screen_lock_popup_timeout', 10000),
	);

	$form['ajax_screen_lock_throbber_hide'] = array(
		'#title' => t('Hide ajax progress throbber'),
		'#type' => 'checkbox',
		'#default_value' => variable_get('ajax_screen_lock_throbber_hide', FALSE),
	);

	// Per-page-path visibility.
	$form['visibility']['pages_path'] = array(
		'#type' => 'fieldset',
		'#title' => t('Pages condition'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
		'#group' => 'visibility',
		'#weight' => 0,
	);

	// Per-request-path visibility.
	$form['visibility']['request_path'] = array(
		'#type' => 'fieldset',
		'#title' => t('Ajax request paths condition'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
		'#group' => 'visibility',
		'#weight' => 1,
	);

	$form['visibility']['pages_path']['ajax_screen_lock_disable_in_admin'] = array(
		'#title' => t('Disable for admin pages'),
		'#type' => 'checkbox',
		'#default_value' => variable_get('ajax_screen_lock_disable_in_admin', 1),
	);

	$form['visibility']['pages_path']['ajax_screen_lock_pages_path'] = array(
		'#type' => 'textarea',
		'#title' => t('Pages'),
		'#description' => t("Allowed only relative urls. On pages listed here the AjaxScreenLock will be enabled. Enter one page per line as Drupal paths.
		The '*' character is a wildcard. Example paths are '<em>blog</em>' for the blog page and '<em>blog/*</em>' for every personal blog.
		'<em>&lt;front&gt;</em>' is the front page."),
		'#cols' => 40,
		'#rows' => 5,
		'#default_value' => variable_get('ajax_screen_lock_pages_path', ''),
	);

	$form['visibility']['pages_path']['ajax_screen_lock_pages_path_ignore'] = array(
		'#type' => 'textarea',
		'#title' => t('Ignore pages'),
		'#description' => t("Allowed only relative urls. On pages listed here the AjaxScreenLock will be disabled. Enter one page per line as Drupal paths.
		The '*' character is a wildcard. Example paths are '<em>blog</em>' for the blog page and '<em>blog/*</em>' for every personal blog.
		'<em>&lt;front&gt;</em>' is the front page."),
		'#cols' => 40,
		'#rows' => 5,
		'#default_value' => variable_get('ajax_screen_lock_pages_path_ignore', ''),
	);

	$options = array(
		AJAX_SCREEN_LOCK_VISIBILITY_NOTLISTED => t('All pages except those listed'),
		AJAX_SCREEN_LOCK_VISIBILITY_LISTED => t('Only the listed pages'),
	);

	$form['visibility']['request_path']['ajax_screen_lock_request_visibility'] = array(
		'#type' => 'radios',
		'#title' => t('Add AjaxScreenLock for specific paths of the ajax requests.'),
		'#options' => $options,
		'#default_value' => variable_get('ajax_screen_lock_request_visibility', AJAX_SCREEN_LOCK_VISIBILITY_NOTLISTED),
	);
	$form['visibility']['request_path']['ajax_screen_lock_request_path'] = array(
		'#type' => 'textarea',
		'#description' => t("Allowed only relative urls. Enter one path per line. <b>Notice: no wildcard supported</b>
     <p>You may use only beginning of the URL. For example, <i>example/path</i> will block/allow all URLs like <i>example/path/1, example/path/2 and etc</i>"),
		'#default_value' => variable_get('ajax_screen_lock_request_path', ''),
	);

	return system_settings_form($form);
}

/**
 * Validation function for ajax_screen_lock_settings_form().
 */
function ajax_screen_lock_settings_form_validate(&$form, &$form_state) {
	$timeout = $form_state['values']['ajax_screen_lock_popup_timeout'];
	if (!is_numeric($timeout) && $timeout >= 0) {
		form_set_error('ajax_screen_lock_popup_timeout', t('The field Timeout must contain positive numeric value'));
	}
}
