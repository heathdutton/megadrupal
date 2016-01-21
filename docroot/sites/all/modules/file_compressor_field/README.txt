This module provides a "File Compressor" field type that allows to archive and
compress attached files to an entity into a single file.

Given that this file is inside a new field on the entity, this module
provides full integration with the entity system out of the box. Entity
revisions and multilingual features work out of the box.

This field is totally hidden during the entity creation process, and makes use
of default File field formatters to display the compressed file.


Installation
---------------

1. Follow the normal Drupal module installation procedures.


Usage
---------------

Usage is straightforward. Add a "File Compressor" field to any entity and select
the fields you want to include in the automatically generated compressed file
from the list.

Configure the field visibility in the "Manage Display" tab.

Create a new entity, upload attachments for the fields selected above and save.

If you made visible the File Compressor field, you will be able to download all
the attachments to the entity in a single compressed file.

Note: By default, File Compressor field module only gives support for File and
Image fields. Besides of that, provides hooks to extend it to other file based
fields.


Requirements
---------------

This module provides by default two Compressor plugins, Zip and GZip. This list
can be extended using hooks. Each zip provider requires a different PHP library:
- Zip: php_zip extension. [PHP Zip Doc](http://php.net/manual/es/book.zip.php)
- GZip: Archive Tar PEAR package
[Archive Tar Doc](http://pear.php.net/package/Archive_Tar)


Authors/Credits
---------------

* Author: [plopesc](http://drupal.org/user/282415)
* Development sponsored by [Bluespark](http://bluespark.com).
