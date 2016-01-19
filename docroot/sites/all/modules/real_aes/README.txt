# Real AES

## Introduction

Real AES provides an encryption method plugin for the Encrypt module (https://drupal.org/project/encrypt).
It also serves as a library loader for the Defuse PHP-encryption library.

Partial API compatibility with the insecure AES module (via a submodule) is provided to act as a replacement
for use with other modules. Contrary to AES, this module will not accept keys that are too long or too small.

Defuse PHP-encryption provides authenticated encryption via an Encrypt-then-MAC scheme. AES-128 CBC is the encryption
algorithm, SHA-256 the hash algorithm for the HMAC. IV's are automatically and randomly generated. You do not need
to manage the IV separately, as it is included in the ciphertext.

Ciphertext format is: HMAC || iv || ciphertext

The HMAC verifies both IV and Ciphertext.

Beware that AES compatibility is at API-level only, and then just partial. Existing messages cannot be decrypted, nor
is there an upgrade path.

## Differences to the AES module:

By default:

- Uses AES
- Only one encryption mode
- No IV reuse
- Authenticated encryption (prevents ciphertext tampering attacks eg the Padding Oracle "Vaudenay" attack)
- No silent key replacement
- No database keys
- No generation of weak keys
- PKCS7 padding
- Will not accept "keys" of incorrect length
- No support for AES encryption of user passwords
- Fails hard when there are problems with encryption or decryption

## Requirements

PHP 5.4 with the openssl extension.
The Defuse PHP-Encryption library from https://github.com/defuse/php-encryption. Install it as php-encryption in your
libraries folder (eg sites/all/libraries/php-encryption).

## General configuration

If you need the defuse php-encryption library, or use the Encrypt plugin just enable Real AES and install the library.
If you need aes_encrypt / aes_decrypt using a global key, enable the included AES submodule and follow the steps below
to generate a default key.

### Generate a key

To generate a 128 bits random key, use the following command on the Unix CLI:

dd if=/dev/urandom bs=16 count=1 > /path/to/aes.key

This file MUST be stored outside of the docroot. Copy this file to an off-server, safe backup. If you lose the key,
you will not be able to decrypt encrypted information in the database.

If you do not have access to dd, generate the file using drush on a working Drupal installation:

drush php-eval 'echo drupal_random_bytes(16);' > /path/to/aes.key

### Point Real AES to the key

$conf['real_aes_key_file'] = '/path/to/aes.key';

## Encrypt plugin configuration

Real AES adds the "Authenticated AES" encryption method on the "Encryption method settings" tab for Encrypt
configurations.

It is important to ensure a proper key. We suggest to use the "File" key provider, but generate the key yourself.

dd if=/dev/urandom bs=16 count=1 > /path/to/encrypt_key.key

or

drush php-eval 'echo drupal_random_bytes(16);' > /path/to/encrypt_key.key

Supply the key provider with the path to this file.

## Usage

1. Use the Authenticated AES encryption method with the Encrypt module (https://drupal.org/project/encrypt).

2. If you do not require aes_encrypt and aes_decrypt, use this module as a Defuse PHP Encryption library loader.
   In your own code, include the library with libraries_load('php-encryption'), then call Crypto::encrypt,
   Crypto::decrypt and Crypto::createNewRandomKey directly.

   See
   * https://github.com/defuse/php-encryption for documentation,
   * https://github.com/defuse/php-encryption/blob/master/example.php for an example

3. If necessary, enable the provided AES submodule. This is an API module exposing aes_encrypt and aes_decrypt
for partial API compatibility with modules depending on the insecure AES module.


## Credits

This module was created by LimoenGroen - https://limoengroen.nl - after carefully considering the various encryption
modules and libraries.

TODO: Patch encrypted_files to use Defuse PHP-encryption.
