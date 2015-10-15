# Install & Setup
* run "composer install" to get all the dependencies 
* edit the Behat.yml to fit your project urls and paths
* make sure the Drush alias "local" is configured correctly

# Selenium
* Get latest version of Selenium Server from http://docs.seleniumhq.org/download/
* You may place it in the root directory of this repo
Note: Selenium Server 2.44.0 does not work with Firefox 35!!! (You may get Firefox 34 form here: https://ftp.mozilla.org/pub/mozilla.org/firefox/releases/34.0/)

## Phantomjs
npm -g install phantomjs

## Chrome
* Get it from https://sites.google.com/a/chromium.org/chromedriver/
* You may place it in the root directory of this repo

## Firefox
* Supported without additional software

# Test

You need to run the web driver first and then start the behat tests.

## Start the web driver
### Selenium 2 default (Firefox)
    java -jar selenium-server-standalone-2.44.0.jar
### Chrome
    java -jar selenium-server-standalone-2.44.0.jar -Dwebdriver.chrome.driver="chromedriver"
### Phantomjs
    phantomjs --webdriver=8643

## Run the test
    bin/behat

# Troubleshooting
## Tests don’t run and break at the first step.

    Given I am logged in as a user with the "administrator" role                            # Drupal\DrupalExtension\Context\DrupalContext::assertAuthenticatedByRole()
    (RuntimeException)

Your drush alias might not be configured correctly. Try to run "drush @local status" in your test directory to debug.

## "bash: drush: command not found"

Behat directly executes the drush command on the environment via ssh. Files like .profile or .bashrc will not be executed. Symlink your drush command into /usr/local/bin or try http://stackoverflow.com/a/10564729 (did not work for me on my puphpet vagrant vm)