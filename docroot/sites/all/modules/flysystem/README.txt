Flysystem
=========

## REQUIREMENTS ##

- PHP 5.4 or greater
- Composer (https://getcomposer.org)
- Composer manager (https://www.drupal.org/project/composer_manager)

## INSTALLATION ##

These are the steps you need to take in order to use this software. Order is
important. (using drush)

 1. drush dl composer_manager && drush en composer_manager -y
 2. drush dl flysystem && drush en flysystem -y
 6. Enjoy.

## CONFIGURATION ##

Stream wrappers are configured in settings.php. The keys (localexample) are the
names of the stream wrappers. Can be used like 'localexample://filename.txt' The
'driver' key, is the type of adapter. Available adapters are:

 - local
 - ftp (Requires the ftp extension)
 - dropbox (https://www.drupal.org/project/flysystem_dropbox)
 - rackspace (https://www.drupal.org/project/flysystem_rackspace)
 - s3v2 (https://www.drupal.org/project/flysystem_s3)
 - sftp (https://www.drupal.org/project/flysystem_sftp)
 - zip (https://www.drupal.org/project/flysystem_zip)

The 'config' key is the settings that will be passed into the Flysystem adapter.

Example configuration:

$schemes = array(
  'localexample' => array(       // The name of the stream wrapper. localexample://

    'driver' => 'local',         // The plugin key.

    'config' => array(
      'root' => '/path/to/dir',  // If 'root' is inside the public directory,
    ),                           // then files will be served directly. Can be
                                 // relative or absolute.

    // Optional settings that apply to all adapters.

    'cache' => TRUE,             // Cache filesystem metadata. Not necessary,
                                 // since this is a local filesystem.

    'replicate' => 'ftpexample', // 'replicate' writes to both filesystems, but
                                 // reads from this one. Functions as a backup.
  ),

  'ftpexample' => array(
    'driver' => 'ftp',
    'config' => array(
      'host' => 'ftp.example.com',
      'username' => 'username',
      'password' => 'password',

      // Optional config settings.
      'port' => 21,
      'root' => '/path/to/root',
      'passive' => true,
      'ssl' => false,
      'timeout' => 90,
      'permPrivate' => 0700,
      'permPublic' => 0700,
      'transferMode' => FTP_BINARY,
    ),
  ),
);

// Don't forget this!
$conf['flysystem'] = $schemes;
