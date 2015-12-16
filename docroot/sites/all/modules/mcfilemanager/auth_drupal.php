<?php

/**
 * Root directory of Drupal installation.
 */
define('DRUPAL_ROOT', getcwd());

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

// Strip the referref url so we can find out what type of manager is called (image- or filemanager).
$queries = explode('?', array_pop(explode('?', urldecode($_GET['return_url']))));
$_get = array();
foreach ($queries as $q) {
  $q = explode('=', $q);
  $_get[$q[0]] = $q[1];
}

// Check if user is valid or not
if (!user_access('administer files'))
	die("Sorry, you don't have access to the imagemanager/filemanager.");

// Override any config values here
$config = array();

// Uncomment this one to create user specific file areas. Change the path if needed
// Paste here all your default settings
$config = variable_get("mcfilemanager_config_options", mcfilemanager_config_defaults());
$config['filesystem.rootpath'] = realpath($config['filesystem.rootpath']);

if ($_get['type'] == 'fm' && variable_get('mcfilemanager_imagemanager_status_ok', FALSE)) {
	$config['general.tools'] = "createdir,createdoc,refresh,zip,upload,edit,rename,cut,copy,paste,delete,selectall,unselectall,view,download,insert,addfavorite,removefavorite,imagemanager";
}
elseif (variable_get('mcfilemanager_filemanager_status_ok', FALSE)) {
  $config['general.tools'] = "createdir,upload,refresh,addfavorite,removefavorite,insert,delete,edit,preview,filemanager";
}

// Generates a unique key of the config values with the secret key
$key = md5(implode('', array_values($config)) . $config['ExternalAuthenticator.secret_key']);
?>

<html>
<body onload="document.forms[0].submit();">
<form method="post" action="<?php echo htmlentities($_GET['return_url']); ?>">
<input type="hidden" name="key" value="<?php echo htmlentities($key); ?>" />
<?php
	foreach ($config as $key => $value) {
		echo '<input type="hidden" name="' . htmlentities(str_replace('.', '__', $key)) . '" value="' . htmlentities($value) . '" />';
	}
?>
</form>
</body>
</html>