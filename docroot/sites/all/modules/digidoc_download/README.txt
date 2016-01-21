INTRODUCTION
------------

DigiDoc files are XML containers of digitally signed files see more:
http://e-estonia.com/components/digidoc. This module strips the container file
and presents the files form within the DigiDoc. If there are more than one files
an zip archive is created and presented. This is useful and sometimes even
required for not disclosing the signees personal information.

REQUIREMENTS
------------

SimpleXML http://ee1.php.net/simplexml, enabled by default since PHP 5.1.2
Zip Archive http://www.php.net/manual/en/class.ziparchive.php PHP >= 5.2.0

INSTALLATION
------------

Standard Drupal module installation.

To prevent direct access to DigiDocs in public files add following lines to
the .htaccess file in the public files folder (usually sites/default/files):

<Files ~ "\.(d|D)(d|D)(o|O)(c|C)$">
  Order allow,deny
  Deny from all
</Files>
