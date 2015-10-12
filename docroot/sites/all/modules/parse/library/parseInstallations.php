<?php

class parseInstallations extends parseRestClient{
	
  public function __construct($appid, $masterkey, $restkey, $parseurl, $skip) {
    parent::__construct($appid, $masterkey, $restkey, $parseurl);
		$request = $this->request(
		  array(
			  'method' => 'GET',
				'requestUrl' => 'installations',
			)
		);
		return $request;
	}
	
	public function associate($object_id) {
	  $request = $this->request(
	    array(
	      'method' => 'PUT',
	      'requestUrl' => 'installations/' . $object_id,
	      'data' => array(
	        'user' => array(
	          '__type' => 'Pointer',
	          'className' => '_User',
	          'objectId' => $object_id,
	        ),
	      ),
	    )
	  );
	}
	
}