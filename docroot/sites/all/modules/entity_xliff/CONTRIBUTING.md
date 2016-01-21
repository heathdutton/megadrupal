# Contribute

Great to have you here! Here's how you can help make this project better.


## Reporting bugs and proposing new features

* Please report any and all bugs to the [Entity XLIFF issue tracker][]; be sure
  that you label your issue with "bug."
* You can look through the [existing bugs here][]; please be sure to search
  through existing bugs before reporting your own.
* You can help us diagnose and fix existing bugs by asking and providing answers
  for the following:
  * Is the bug reproducible as explained?
  * Is it reproducible in other environments (for instance, on different
    versions of PHP, operating systems, with different packaged libraries, etc)?
  * Are the steps to reproduce the bug clear? If not, can you describe how you
    might reproduce it?
  * Is this bug something you have run into? Would you appreciate it being
    looked into faster?
* You can close fixed bugs by testing old tickets to see if they are still happening.
* You can help bring duplicate bug reports to our attention by cross-linking
  issues.

We'd also love your feedback on features! Please add feature requests in the
same way you add bugs, but use the "enhancement" tag instead. You should also
check [existing feature requests here][] before submitting yours.


## Contributing fixes and features

Pull requests for bug fixes and features are greatly appreciated! Before you
open that pull request up, here are a few tips to make the process smooth:

* Your change should be made in a feature branch based on the default branch. We
  prefer you not commit directly to 7.x-1.x in your fork.
* A good PR probably includes tests. Details on our test suite and how to run it
  can be found below.
* We try our best to follow [Drupal coding standards][].

Don't be discouraged! Maintenance for this project is done on work time as
sprint planning and time allows, and on personal time when necessary. Don't
hesitate to ping us via @mention if we're taking unduly long.


## Running tests locally

Although tests run automatically on Travis CI, you probably want to run them
locally as you're developing. Here's a quick guide:

Before you run any tests, be sure you've

* Ensure you've [installed composer][],
* Ensure you've `composer install`'d or `composer update`'d,

#### PHPUnit

Running unit tests couldn't be simpler:

* From the project root, run `./vendor/bin/phpunit`

New tests should be added under `test/Drupal`.

#### Behat

We use Behat to run behavioral tests against an actual Drupal installation. In
order to simplify local test setup, there are some utility scripts located under
`test/scripts` that you may need to use.

* Run `./test/scripts/setup_behaviors.sh`
  * Note that you will need to have [drush][] and MySQL installed already.
  * The set up process will prompt and warn you to create databases and users
    with credentials.
* Then, run `./vendor/bin/behat`

New Behat features should be added to `test/Features`, and new feature contexts
should be added to `test/Features/Bootstrap/FeatureContext.php`.


[Entity XLIFF issue tracker]: https://github.com/tableau-mkt/entity_xliff/issues
[existing bugs here]: https://github.com/tableau-mkt/entity_xliff/issues?q=is%3Aopen+is%3Aissue+label%3Abug
[existing feature requests here]: https://github.com/tableau-mkt/entity_xliff/issues?q=is%3Aopen+is%3Aissue+label%3Aenhancement
[Drupal coding standards]: https://www.drupal.org/coding-standards
[installed composer]: https://getcomposer.org/doc/00-intro.md#installation-linux-unix-osx
[drush]: https://github.com/drush-ops/drush/blob/master/docs/install.md
