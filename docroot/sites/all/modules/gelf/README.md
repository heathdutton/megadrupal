The GELF module requires the GELF PHP library.

To install, use one of the following methods:

1. Checkout the GELF PHP library from github like so:

    # From your site's docroot.
    git clone https://github.com/Graylog2/gelf-php.git sites/all/libraries/gelf-php

2. Install via Drush

    # From your site's docroot.
    drush gelf-download

3. Install the module via composer, or in your own composer.json:
    {
      "require":{
        "graylog2/gelf-php": "dev-master"
      }
    }

Older versions of the GELF PHP library contained the file 'gelf.php'.
Newer versions have split that out into GELFMessage.php and
GELFMessagePublisher.php. If your instance of gelf-php doesn't have
those two newer files, you probably need to do a 'git pull' in the
gelf-php directory to get the latest version.
