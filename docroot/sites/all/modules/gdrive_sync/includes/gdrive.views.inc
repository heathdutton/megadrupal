<?php
	function gdrive_views_data() {
		$data['gdrive_files']['table']['group'] = t('Google');

    $data['gdrive_files']['table']['base'] = array(
  	    'field' => 'fid',
            'title' => t('gdrive file'),
	    'help' => t("gdrive files"),
	    'weight' => -10,
	  );
	
		$data['gdrive_files']['fid'] = array(
			'title' => t('google file id'),
			'help' => t('Google drive file'),
			'group' => t('Google'),
			'field' => array('handler' => 'views_handler_field'),
		);		
		
		$data['gdrive_files']['url'] = array(
			'title' => t('Google drive url'),
			'group' => t('Google'),
			'field' => array('handler' => 'views_handler_field'),
		);

		$data['gdrive_files']['title'] = array(
			'title' => t('Google drive title'),
			'group' => t('Google'),
			'field' => array('handler' => 'views_handler_field'),
		);
		return $data;
	}
?>