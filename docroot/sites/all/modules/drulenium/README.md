Installation instructions for https://www.drupal.org/project/drulenium


Setting up a local selenium server:
* Download Selenium Server from http://docs.seleniumhq.org/download/
** Tested with version 2.47.1
* Download dependencies, java xvfb xauth imagemagick php5-curl and firefox.
** Debian: ```apt-get install default-jre xvfb xauth imagemagick php5-curl iceweasel```
* Run Selenium Server
** Local: `java -jar selenium-server-standalone-<version>.jar`
** On a server: `nohup xvfb-run java -jar selenium-server-standalone-<version>.jar -log selenium.log &`
* Download & install Drupal Libraries API if you don't already have it. https://www.drupal.org/project/libraries
* Download php-webdriver from https://github.com/facebook/php-webdriver (Zip file: https://github.com/facebook/php-webdriver/archive/master.zip)
* Extract into sites/all/libraries and rename the folder to selenium_webdriver so that the file __init__.php exists at sites/all/libraries/selenium_webdriver/lib/__init__.php
* When testing, configure the path to the ImageMagick program on admin/structure/drulenium/vr
