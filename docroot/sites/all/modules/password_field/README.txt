Password Field
--------------

This module allows you to create fields that store passwords. It will store the
passwords in encrypted format and (by default) will not display them on the
website. So this module is really only useful in conjunction with other modules
that can access the data through the API it provides.

A note on security
==================

The most important thing to know about this module is that YOU SHOULD NOT USE
THIS MODULE IF YOU CAN POSSIBLY AVOID IT.

The reason for this rather strong statement is that, in general, storing
passwords in a database is generally a bad idea. If you can store a salted hash
of a password (like Drupal does), or an authorisation token (the way oAuth
works), then you should do that instead. Sometimes though, there is a genuine
need to store a password in a database, and you want to make sure that the data
is at least encrypted. If you are in that situation, then this module is for
you.

Please note, even though the passwords are stored in an encrypted format in the
database, anyone who has access to your `settings.php` file should be able
to figure out how to decrypt them with very little trouble - especially since
this module is publicly available on drupal.org.

Accessing Passwords
===================

If you are writing a module that relies on this one, you can fetch the encrypted
value of the password using the standard [Drupal Field API
functions](http://tinyurl.com/cb9oxj6).

Once you have the encrypted value, you can decrypt it using the
`password_field_decrypt()` function. For example:

    global $node, $language;
    module_load_include('module', 'password_field', 'password_field');

    $langcode = $language['language'];
    $lang  = (array_key_exists($langcode, $field_data)) ? $langcode : 'und';

    $field_data = $node->field_password_field;
    $parts = array_values($field_data[$lang][0]);

    $password_value = password_field_decrypt($parts[0]);


Author
======

James Sinclair
http://drupal.org/user/873966
