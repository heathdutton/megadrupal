
Allows you to generate a SSH key pair (public/private) for that site using
ssh-keygen and use the phpseclib library. There is a composer.json file included
so that you can easily install the dependencies.

Run "composer.phar install" to download phpseclib before enabling.  If you don't
have composer, see: http://getcomposer.org/

You may optionally use the Composer Manager module to install and manage
phpseclib in a shared library space. To install phpseclib via Composer Manager,
run "drush composer update" if the installer has already been run in the
configured Composer libraries directory or "drush composer install" if it has
not. Refer to the "Installation" and "Usage" sections at
http://drupal.org/project/composer_manager for more details.

Admin path can be found at ?q=admin/config/services/ssh-helper

This module becomes very useful if you deploy the public key to a server and
then do remote execution on those servers from within Drupal

A use-case example to run the beep command as root on the drupal.org servers
(provided that you have access using the right key/pair combination, which you
most probably don't!)


// Get our SSH key and pass
$key = ssh_helper_load_key();
// This will be our connection class, hostname is obligatory
$ssh = ssh_helper_load_ssh('www.drupal.org');
$ssh->login('root', $key);
$ssh->exec('beep');


To execute PHPUnit tests you should execute the following commands. It assumes you have your vendor directory out of
your docroot. The example was also made with composer_manager as it is required to run the PHPUnit tests.
@todo : Make the PHPUnit tests discoverable.

cd PATH/TO/YOUR/DRUPAL/ROOT
./../vendor/phpunit/phpunit/phpunit.php sites/all/modules/contrib/ssh_helper/tests/SshHelperUnitTest.php