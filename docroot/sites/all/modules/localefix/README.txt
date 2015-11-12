This module only works if your locale of Apache instance is setup correctly.

Step 1.
Before using this module, you should check your locale of apache start script.

Below is an example of 2 different OS:

A. Centos
Find the script of httpd locate at /etc/init.d/httpd, then edit change below:

HTTPD_LANG=${HTTPD_LANG-"C"}
to
HTTPD_LANG=${HTTPD_LANG-"zh_TW.UTF-8"}

(zh_TW.UTF-8 is the locale you want.)


B. Debian
Find the start script at /etc/init.d/apache2, then edit, change below:

ENV="env -i PATH=/usr/local/bin:/usr/bin:/bin"
to
ENV="env -i LANG=zh_TW.UTF-8 LC_ALL=zh_TW.UTF-8 PATH=/usr/local/bin:/usr/bin:/bin"



Step 2.
After setting that, restart apache, and you can write a test.php to see if the
environment variable is setting correct.

<?php
print $_ENV['LANG'];
print $_ENV['LC_CTYPE'];
?>


Step 3.
Drupal override all the locale setting in function unicode_check (unicode.inc).
So, enable this module, got to admin/settings/localefix to check if the locale 
setup correctly and fix your problem.

In our case, after we enable this module, the locale aware php function basename()
is return correct filename. But without this module, drupal can't recogonize the
filename.
