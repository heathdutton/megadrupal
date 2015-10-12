Travis Builder
==============

**Trigger [Travis CI][travis] builds within [Drupal][drupal].**

Why?  Perhaps you have a static site/app which uses the Drupal CMS as it's content souce.  You build/deploy the static app automatically, normally after successful tests upon a commit to master via the Travis continuous integration tool to some web host.  Now you can use Drupal events (via Rules) to rebuild and deploy when: content is updated, you clear cache, on cron, or whenever.  Currently this is limited to just one Travis repository per site.

*"Headless Drupal is so hot right now."*

Please file issues on [Drupal.org][tbdrupal]. Please star on [GitHub][tbgithub].

## Install...
This modules has a few, beyond ordinary, installation steps...

### Composer
The module currently uses a forked version of the [l3l0/php-travis-client][phptravis] library for reaching out to Travis' API, which is available via composer.  To add this to your codebase you'll need Drush and [Composer Manager][comp] run `drush composer install` to download the dependecies into your `/sites/all/vendors` folder.  If you run into trouble you may need the `drush composer update` command as well.

### Travis token
Public repos can be triggers via standard account travis token. This can be generated using the Travis [command line tool][traviscli] Ruby gem. Here's the process...

1. `gem install travis`
2. `travis login`
2. `travis token`

Enter the access token into Drupal via `admin/config/system/travisbuilder`.

### Private repos
In order to trigger private repo builds, you'll need to gerneate a "Travis token" using a "Github authorization token."  The command is `travis login --github-token "YOUR GITHUB TOKEN"` but you can read more in the [API docs][travisapiauth].

## Usage...

## Rules

This module uses [Rules][rules] to allow triggering builds as an action.  This is a super flexible way to use whatever event with whatever conditions to restart your builds.  You can trigger different branches with each Rules action.  There is also a minimum lifetime setting, which can be overridden by specific Rules.  It's not a direct dependency, because of probable custom use :)

### Manaully

You can allow admins to manually restart a build (they likely don't have access to your Travis system).  A separate permission handles access to the `admin/config/system/travisbuilder/build` page.  Grant with care.

### Custom

You *could* also just manually fire off the internal function.  If you're using this module chances are you know what you're doing.

```php
function my_reliable_trigger_method() {
  $configs = _travisbuilder_configs();
  _travisbuilder_trigger_build($configs['branch']);
}
```

## Curious about Travis?
* Check out the [Travis eco-system][traviseco]!
* Check out the [Travis docs][travisdocs].

[travis]: https://travis-ci.com
[drupal]: https://www.drupal.org
[comp]: https://www.drupal.org/project/composer_manager
[phptravis]: https://github.com/tableau-mkt/php-travis-client
[traviseco]: http://docs.travis-ci.com/user/apps
[traviscli]: http://blog.travis-ci.com/2013-01-14-new-client
[travisapiauth]: http://docs.travis-ci.com/api/?shell#with-a-github-token
[travisdocs]: http://docs.travis-ci.com/api
[rules]: https://www.drupal.org/project/rules
[tbdrupal]: https://www.drupal.org/project/travisbuilder
[tbgithub]: https://github.com/tableau-mkt/travisbuilder
