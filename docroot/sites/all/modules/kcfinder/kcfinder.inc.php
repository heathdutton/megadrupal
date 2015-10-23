<?php
// Common utility functions


/**
* Tries it's best to find and return the KCFinder library path, relative to drupal root.
* For common use, please call variable_get('kcfinder_library_path') instead.
*/
function kcfinder_find_library_path() {
  static $library_path = NULL;

  // Try to locate the library path in any possible setup.
  if ($library_path == NULL) {
    
	// First check the default location.
    $path = variable_get('kcfinder_library_path', 'sites/all/libraries/kcfinder');
    if (is_dir($path)) {
      $library_path = $path;
    }
	
    // Ask the libraries module as a fallback.
    if ($library_path == NULL && module_exists('libraries')) {
      $path = libraries_get_path('kcfinder');
	  if ($path != '') {
        $library_path = $path;
      }
    }
  }
  
  // If no path is found suggest the default one.
  if ($library_path == NULL) {
    $library_path = 'sites/all/libraries/kcfinder';
  }
  
  // Throw an error if we weren't able to find a suitable path
  if (!kcfinder_library_exists($library_path)) {
    drupal_set_message(
	  t(KCFINDER_LIB_NOT_FOUND, array('!kcfinder_config' => l('configuration page', 'admin/config/content/kcfinder')) ),
	  'error'
	);
  }
  
  return $library_path;
}


/**
* Returns the kcfinder library absolute url path, based on variable_get('kcfinder_library_path')
*/
function kcfinder_library_path_url() {
	return url(variable_get('kcfinder_library_path'), array('absolute' => TRUE, 'language' => FALSE));
}


/**
* Returns TRUE if the kcfinder library exists in variable_get('kcfinder_library_path'), or in the parameter if specified
*/
function kcfinder_library_exists($library_path = "") {
	if($library_path == "")
		$library_path = variable_get('kcfinder_library_path');
	return file_exists($library_path . '/browse.php');
}