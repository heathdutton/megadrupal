GPG encrypt module
=====

Here you can see pretty simple gpg encryption solution for files.

Use admin/config/system/gpg_acrypt for adding your public key.

There is only one function

```php
function acrypt_gpg_encrypt($src_filename, $dest_filename = NULL, $gpg_pub_key = NULL);
```

that can be used as

```php
$source_file = 'private://webform_export_471.csv';
$encrypted_file = 'public://webform_export_471.csv.gpg';

$status = acrypt_gpg_encrypt($source_file, $encrypted_file);

if ($status) {
  unlink($source_file);
}
```

You can check encrypted file at ```$encrypted_file``` path

For decryption you can use console gpg app

```sh
gpg --output webform_export_471.csv --decrypt webform_export_471.csv.gpg
```

with decrypted data saved to ```webform_export_471.csv``` file.

PHP settings
=====

```sh
sudo apt-get install php-pear php5-dev libgpgme11-dev
sudo pecl install gnupg
```

Also there is a requirement point.
You have to create .gnupg directory within apache's user home directory writable for apache
```sh
mkdir /var/www/.gnupg
chown -R apache:apache /var/www/.gnupg
```
where
- /var/www - apache's user home dir
- apache:apache - user and group apache's process belongs to.

You should add 
```ini
extension=gnupg.so
```

to your ```php.ini``` config file afterwards
and restart ```apache``` or ```php5-fpm``` service, depending from your system configuration.

```sh
sudo service apache2 restart
sudo service php5-fpm restart
```

UI tools
=====

Here is a program for windows for ability to create keys, encrypt and decrypt data.

http://ppgp.sourceforge.net

For mac
https://gpgtools.org
