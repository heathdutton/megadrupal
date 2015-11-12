Drush instance test suite
=========================

Summary
--------

This test suite is based on PHPUnit (http://www.phpunit.de/).

Some of this test suite has been reused from Drush's core test suite, including most of the text in this README file.

Usage
--------

- Install PHPUnit [*]
- Attempt to run tests: from the /tests subdirectory, run `phpunit .`
- Review the configuration settings in phpunit.xml. If customization is
  needed, copy and rename to phpunit.xml and edit away.
- Re-run the tests

Advanced usage
---------
- Run only tests matching a regex: phpunit --filter=testVersionString .
- Skip slow tests (usually those with network usage):
    phpunit --exclude-group slow .
- XML results: phpunit --filter=testVersionString --log-junit results.xml .

[*] Install PHPUnit:
---------------

Drush requires PHPUnit 3.5 or later. Install using Composer: the PEAR channel is deprecated.

Here are instructions on how to install using Composer:

    http://phpunit.de/manual/current/en/installation.html#installation.composer

A composer.json file is included which you can use if you like.

Note that if you install using the composer.json file, you should move the
tests/vendor folder up out of tests/ into the top level of drush_instance.
This is to prevent phpunit from trying to run its own tests.

Once phpunit is installed, here's how you run it, from within tests/:

    ~/.composer/vendor/bin/phpunit .     # global installation
    ../vendor/bin/phpunit .              # local installation in vendor/
