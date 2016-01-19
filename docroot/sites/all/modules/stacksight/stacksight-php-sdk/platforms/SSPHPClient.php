<?php 

require(dirname(__FILE__).'/../bootstrap.php');
define('APP_SETTINGS_FILE', dirname(__FILE__).'/../../app_settings.json');

class SSPHPClient extends SSClientBase {

	protected function saveSettings($data) {
		return file_put_contents(APP_SETTINGS_FILE, json_encode($data));
	}

	protected function getSettings() {
		if (file_exists(APP_SETTINGS_FILE)) return json_decode(file_get_contents(APP_SETTINGS_FILE), true);
	}

}