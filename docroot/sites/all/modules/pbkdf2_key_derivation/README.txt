# Introduction

Offers a PBKDF2 key derivation function that works on PHP 5 below PHP 5.5. On PHP 5.5, the builtin hash_pbkdf2 will be used.

# Installation

* Install https://github.com/limoengroen/hash-pbkdf2 into your libraries folder (eg sites/all/libraries) as hash-pbkdf2.
* Enable the module.

# Usage

This is an API module, there is no interface.

## API usage

pbkdf2($algorithm, $password, $salt, $count, $key_length, $raw_output = false)

* $algorithm - The hash algorithm to use. Recommended: sha256
* $password - The password.
* $salt - A salt that is unique to the password.
* $count - Iteration count. Higher is better, but slower. Recommended: At least 1000.
* $key_length - The length of the derived key in bytes.
* $raw_output - If true, the key is returned in raw binary format. Hex encoded otherwise.
* Returns: A $key_length-byte key derived from the password and salt.

# Credits

Taylor Hornby (defuse.ca) for the PBKDF2 PHP implementation.
