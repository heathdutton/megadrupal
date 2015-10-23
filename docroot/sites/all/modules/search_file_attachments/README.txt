Search File Attachments

This modules allows the indexing and searching of file attachments. It will
extraxt the content of attached files using the Apache Tika library and
index it. Search File Attachments will index many file formats, including
MS Office documents, PDFs, EPUB, JPEG metadata, ...

More information about the Apache Tika library:
http://tika.apache.org/download.html

REQUIREMENTS
============
Requires the ability to run java on your server (for the Apache Tika library)
and an installation of the Apache Tika library.

In your PHP settings (php.ini) the Safe Mode must be disabled and the exec()
function should not be disabled.

INSTALLATION
============
1. Install java on your machine, if not yet done.
2. Download the Apache Tika library: http://tika.apache.org/download.html
     > wget http://www.apache.org/dyn/closer.cgi/tika/tika-app-1.1.jar
3. Copy the search_file_attachments module direcotry into your modules folder.
4. Install the search_file_attachments module on your Drupal site.
5. Go to the configuaration page: admin/config/search/file_attachments
   5.1. Set the full path to the tika-app-x.x.jar file from step 2.
        e.g. /var/apache-tika/
   5.2. Enter the name of the jar file: e.g. tika-app-1.2.jar
6. Go to the search configuration page: /admin/config/search/settings
   and activate 'Search File Attachments' under the 'Active search modules'
   section.

Note: If you run the drupal cron via drush you need to set the $base_url
      variable in your settings.php configuration file.

MAINTAINER
==========
- cbeier (Christian Beier)
