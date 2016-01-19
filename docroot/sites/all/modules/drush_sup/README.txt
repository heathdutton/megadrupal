DESCRIPTION
===========

The Drush site-upgrade command automates the process of upgrading a Drupal site
per the instructions in UPGRADE.txt.  There are two primary benefits of using
Drush site-upgrade over the manual process:

 1. Automation of the tedious process required to upgrade contrib modules
    saves on time and reduces the odds of a failed upgrade due to a mis-step.

 2. Advice is given on replacement modules to use in instances where a module in
    use on the old site has no upgrade path.

The site-upgrade command can be stopped in the middle, resumed, and ran as many
times as needed to perform a successful upgrade.

Note that you will still need to port the CODE for your custom theme and modules,
if any are in use.


REQUIREMENTS
============

* Drush-5.4 or later required.

* Currently, only Drupal-6 to Drupal-7 upgrades are supported.


INSTALLATION
============

If you have not already done so, install Drush as described on the Drush
project page: http://drupal.org/project/drush

To install Drush site-upgrade, simply change to your home directory and run:

  $ drush dl drush_sup

This will put the site-upgrade Drush extension in the correct place for
Drush to recognize it.

        NOTE: It is recommended that download drush_sup from your home
        directory in order to allow Drush to select the correct default
        version for drush_sup. If your current working directory is inside
        of a Drupal 6.x site, Drush will attempt to download drush_sup-6.x,
        which does not exist and will therefore fail.  As an alternative,
        you could specifically request the 7.x branch via:

          $ drush dl drush_sup-7.x-2.x --select

        You may then select the dev version or the most recent stable
        release.


USAGE
=====

 1. Read the UPGRADE.txt document in Drupal core of the version of Drupal you are
    upgrading to, and understand the process.  Also carefully study the module
    upgrade procedure, available at the URL specified in UPGRADE.txt.  Drush Site
    Upgrade follows the process described in UPGRADE.txt very closely; however,
    one very important difference is that UPGRADE.txt instructs you to upgrade
    your Drupal site in place, whereas `drush site-upgrade` will upgrade to
    a separate copy of your site, leaving the orginal unmodified.

 2. Use `drush pm-update` to do a minor version update of your source site to the
    most recent available version of core and contrib.  All of your code must be
    up to date before you begin the major version upgrade.

 3. Run `drush sup` once on your source site and review the upgrade readiness report.

        $ cd /path/to/drupal
        $ drush site-upgrade

     - OR -

        $ drush @d6 site-upgrade

    The site-upgrade will examine the modules that you have installed,
    and will report on the availability of Drupal 7 versions of the same
    modules.  For modules which have no version available in Drupal 7,
    Drush Site Upgrade will offer to substitute alternate projects that
    may perform similar functions.  Sometimes there is more than one option
    available, in which case Drush will pick the one that it thinks is best,
    but will offer you the chance to select a different one if you wish.
    You may see a message similar to this:

       cck: The module nodereference in this project has no D7 version
       available, but there are multiple alternative projects that may
       be used instead. Drush picked references:node_reference, but
       entityreference also available.  Run again with --preferred to
       select a different preference.

    If you would rather use entityreference than references, then run
    drush site-upgrade again with the --preferred option:

        $ drush @d6 site-upgrade --preferred=entityreference

    If you have multiple alternative preferences to separate, include them
    all in a comma-separated list.  Read the project page of each
    alternative to choose the module that seems to best meet your needs.

    Sometimes, there may be preliminary steps are required before beginning
    the upgrade; if so, you will be advised of what needs to be done.
    For example, if you are using features, you should import all of your
    features into the database using the `drush features-import-all`
    command.  This command is under development in the features issue queue;
    see http://drupal.org/node/1014522#comment-5478110 for details.

    You may decide at this point that you cannot upgrade your site, because
    needed modules or themes are not ready for Drupal 7.  If you would
    like to attempt an upgrade, procede to the next step.  Unavailable modules will
    be skipped.  You can always re-run the upgrade again later if you wish.

 4. Prepare a site alias to describe where drush should put the upgraded
    site:

        $aliases['d7upgrade'] = array(
          'root' => '/path/to/upgradeddrupal',
          'uri' => 'mydrupalsite.org',
        );

    Place this alias in any file where aliases may be stored.  See the
    Drush documentation for more information.

 5. Choose the amount of prompting that you would like, and run `drush
    site-upgrade` again.  The upgrade process involves many steps, and
    produces a lot of output.  How much prompting you would like will
    vary depending on how confident you are about the upgrade process.
    This can be controlled by the various prompting options:

       --prompt-all

       This option is best for your first run through Drush Site Upgrade.
       It will prompt at every stage.  Drush will tell you what it is
       going to do before it does it.  This will give you the opportunity
       to do some investigation in another terminal; you can even opt to
       do the operation manually, and tell Drush to continue with the upgrade.

       --prompt-all --skip-optional

       The --skip-optional flag is only valid in --prompt-all mode, as
       skip optional is otherwise the default.  There are a number of steps
       in UPGRADE.txt that are only necessary when you are doing an upgrade
       in place.  For example, UPGRADE.txt instructs you to take the source
       site offline before beginning the upgrade.  Since the Site Upgrade
       command works on a copy of your site, you may leave the source site
       online while you work if you wish.  The --skip-optional step will
       cause Drush to skip past this step and all similar steps automatically.

       [default]

       If none of the prompting options are specified, then
       Drush Site Upgrade will automatically do all of the stages that
       considered "straightforward", and will prompt you only for stages
       where a decision typically needs to be made.  It is recommended to
       run through an upgrade with more prompting at least once before
       reverting to this mode, although it is safe enough to run with just
       the minimal prompting provided by the default mode.

       --auto

       In fully-automatic mode, Drush Site Upgrade will always choose the
       first (most reasonable) option at all stages, and will prompt you
       only in instances where there is no reasonable default.

       --core-unmodified

       Drush will stop and prompt you to re-apply modifications to .htaccess
       and robots.txt, even in --auto mode, unless you also specify the
       --core-unmodified option to skip this step.


 6. Run the Drush site-upgrade command with your selected target alias
    and prompting level:

       $ drush @d6 site-upgrade --prompt-all @d7upgrade

 7. Step through the stages of the upgrade process, making note of any
    problems or error messages encountered along the way.  If you stop the
    upgrade in the middle, either by selecting the [Cancel] option or
    due to some error, you may re-run the Drush site-upgrade command again
    to resume the upgrade.  Drush will prompt you for your desired action:

     * Resume where you left off:

       The last stage executed will be ran again.  If you have a failure
       that can be resolved manually, you can just pick up the upgrade
       from where you last left off.

     * Begin again from the start, re-using the same code:

       If the target site already exists, the site-upgrade command will
       allow you to run the upgrade again, using the code at the target
       location.  This will allow you to manage code changes required
       during your upgrade; just compose the code you want to use at
       the target site, perhaps via `drush dl` of specific versions, by
       pulling from git, or by using  a Drush Make file, and run site-upgrade
       with this option.

     * Begin again from the start with fresh code

       You may also start over from the beginning, once again pulling
       down all code needed via `drush dl` automatically.

     * Resume from a backup point

       Drush site-upgrade automatically backs up your target site every
       time it runs the updatedb command.  Two backups are retained: the
       one that was taken after Drupal core was upgraded, and the one
       that was taken after the most recent contrib module was successfully
       upgraded.  You may resume from either of these points at any time.

 8. Review the site upgrade report at the end of the upgrade process.
    If all went well, all lines in the report will be labeled 'OK'.  If
    there are any problems reported, repair them manually.

 9. Use the content_migrate module to migrate your cck data to Drupal 7
    format.  The site-upgrade command automatically enables content_migrate,
    so you only need to visit the migration page and step through your data types.

Once the site-upgrade has completed, if there were no problems, then your
site should be nominally functional, and all of your data should be in place.
There will still be work left to do, though; review all of your configuration
settings, and insure that everything is correct. Some settings may be dropped
during upgrade.  Fine-tuning of your theme may also be necessary.


SUPPORT
=======

Issues with Drush site-upgrade itself should be posted in the issue queue:

  http://drupal.org/project/issues/drush_sup

If you are having problems related to error messages displayed when ENABLING
or UPDATING a module (or UPDATING core), please direct your support request
to the issue queue of the particular module that is failing.


ISSUES WITH UPGRADE PATH
========================

This section lists some issues from various issue queues on drupal.org that
relate to potential problems with the upgrade path from Drupal 6 to Drupal 7.
Check the issues themselves for updates or improved recommendations before
following the instructions here.

uuid module:

http://drupal.org/node/1469942:  Before upgrading, it is necessary to update to
the 6.x-1.x-dev release of uuid until the 6.x-1.0-beta3 release goes out.

corrupted menus:

http://drupal.org/node/991778: If menus are corrupted after an upgrade,
try: DELETE FROM drupal_menu_links WHERE module = 'system';

postgres:

http://drupal.org/node/1031122 and http://drupal.org/node/1575790: In step 11,
download and apply the patches in these two issues.


CREDITS
=======

* Based on Moshe Weitzman's original site-upgrade code originally located in Drush core.
* Redesigned and maintained by greg.1.anderson.
