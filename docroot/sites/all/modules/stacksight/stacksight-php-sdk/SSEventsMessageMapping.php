<?php 

require_once('SSUtilities.php');

class SSEventsMessageMapping {

	/**
	 * Compose a ready to use in MEAN network intex a styled message.
	 * Consists of two part: colored Font Awesome icon and text description
	 * @param  array $event array(
	 *                      	'action',			// 'created', 'deleted', 'updated', 'added', 'activated', 'deactivated', 'accessed', 'file_updated', 'logged_in', 'logged_out', 'wrong_password', 'installed', etc...
	 *                      	'object_type',		// 'Attachment', 'Menu', 'Options', 'Plugin', 'Post', 'Taxonomy', 'Theme', 'User', 'Widget', etc...
	 *                      	'object_subtype',	// Page, image. May not be applicable
	 *                      	'object_id',		// id of the object
	 *                      	'object_name'		// name of the object
	 * 						)
	 * @return array        array(
	 *         					'design' => array(
	 *         						'icon' => 'fa-file-text', // Font Awesome icon name
	 *         						'color' => '#8FD5FF'	  // Color
	 *         					),
	 *         					'message' => 'A new user "test (email@domain.com)" has been registered'
	 *         				)
	 */
	static function composeEventData($event, $default_message = '') {
		if (!is_array($event)) {
			SSUtilities::error_log('Expected to get an array', 'error');
			return;
		}
		if (empty($event)) {
			SSUtilities::error_log('Empty an array', 'warn');
			return;
		}

		$design = array();
		$message = '';
		$color_created = '#238a36';
		$color_updated = '#8FD5FF';
		$color_deleted = '#9f253f';

		switch ($event['object_type']) {
			case 'content':
				$design['icon'] = 'fa-file-text';

				switch ($event['action']) {
					case 'created':
						$design['color'] = $color_created;
						break;
					case 'updated':
						$design['color'] = $color_updated;
						break;
					case 'deleted':
						$design['color'] = $color_deleted;
						break;
				}

				$message = SSUtilities::t('{type} "{name}" has been {action}.', array(
					'{type}' => ucfirst($event['object_subtype']), 
					'{name}' => $event['object_name'],
					'{action}' => $event['action']
				));
				break;

			case 'user':
				$design['icon'] = 'fa-user';
				$design['color'] = '#8664aa';
				break;

			case 'system':
				$design['icon'] = 'fa-cubes';
				$design['color'] = '#c79696';
				break;

			case 'actions':
				$design['icon'] = 'fa-certificate';
				$design['color'] = '#fd8e00';
				break;

			case 'cron':
				$design['icon'] = 'fa-cogs';
				$design['color'] = '#de1b16';
				$message = SSUtilities::t('Cron run completed.');
				break;

			default:
				$design['color'] = '#19617a';
				$design['icon'] = 'fa-bars';
				break;
		}

		$message = $message ? $message : $default_message;
		$data = array();
		$data['key'] = $event['object_type'];
		$data['name'] = $message;
		$data['data']['description'] = $message;
		$data['design'] = $design;

		return $data;

	}

}