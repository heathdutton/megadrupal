<?php 

class SSHttpRequest {

	const INDEX_ENDPOINT_01 	= 'https://api.stacksight.io/v0.1/index';

	public function publishEvent($data) {
		$opts = array(
			'http' => array(
				'method'  => 'POST',
				'header'  => "Content-type: application/json",
				'content' => json_encode($data)
			)
		);
		$context  = stream_context_create($opts);
		$response = file_get_contents(self::INDEX_ENDPOINT_01.'/'.$data['index'].'/'.$data['eType'], false, $context);

		if ($response !== false) {
			return array('success' => true, 'message' => 'OK');
		} else {
			$error = error_get_last();
			return array('success' => false, 'message' => $error['message']);
		}
	}

	public function sendLog($data) {
		$opts = array(
			'http' => array(
				'method'  => 'POST',
				'header'  => "Content-type: application/json",
				'content' => json_encode($data)
			)
		);
		$context  = stream_context_create($opts);
		$response = file_get_contents(self::INDEX_ENDPOINT_01.'/'.$data['index'].'/'.$data['eType'], false, $context);

		if ($response !== false) {
			return array('success' => true, 'message' => 'OK');
		} else {
			$error = error_get_last();
			return array('success' => false, 'message' => $error['message']);
		}
	}

	public function sendUpdates($data) {
		$opts = array(
			'http' => array(
				'method'  => 'POST',
				'header'  => "Content-type: application/json",
				'content' => json_encode($data)
			)
		);
		$context  = stream_context_create($opts);
		$response = file_get_contents(self::INDEX_ENDPOINT_01.'/updates/update', false, $context);

		// SSUtilities::error_log($response, 'http_update');

		if ($response !== false) {
			return array('success' => true, 'message' => 'OK');
		} else {
			$error = error_get_last();
			return array('success' => false, 'message' => $error['message']);
		}
	}

	public function sendHealth($data) {
		$opts = array(
			'http' => array(
				'method'  => 'POST',
				'header'  => "Content-type: application/json",
				'content' => json_encode($data)
			)
		);
		$context  = stream_context_create($opts);
		$response = file_get_contents(self::INDEX_ENDPOINT_01.'/health/health', false, $context);

		SSUtilities::error_log($response, 'http_health');

		if ($response !== false) {
			return array('success' => true, 'message' => 'OK');
		} else {
			$error = error_get_last();
			return array('success' => false, 'message' => $error['message']);
		}
	}

}