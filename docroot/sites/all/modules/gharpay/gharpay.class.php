<?php

/**
 * @file
 *   Extends Gharpay PHP Library.
 *   Sets username, password and service url for transactions.
 */

class GpGharpayAPI extends GharpayAPI {

	// Constructor call.
	public function GpGharpayAPI() {
		// Set username.
		$this->setUsername(GP_API);
		// Set password.
		$this->setPassword(GP_PASSWORD);
		// Set service url.
		$this->setURL(GP_SERVICE_URL);
	}
}