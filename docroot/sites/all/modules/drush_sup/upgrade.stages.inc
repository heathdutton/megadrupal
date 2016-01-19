<?php

/**
 * _drush_upgrade_stages
 *
 * This function returns the stages of the finite state machine
 * that controls the major upgrade process.  The key of each item
 * in the stages array is the stage name; the first stage is always
 * START, and the last stage is always STOP.
 *
 * The value of each item in the stages array is the control object.
 * The elements of the control object are as follows:
 *
 *  - description:
 *
 *    Shown to the user before the stage begins.  When running with --prompt,
 *    the user will be asked whether to perform the step, skip it (do it manually)
 *    or to stop.  Perhaps this should be the default, and --no-prompt should be
 *    required to skip prompts.  Note that other confirmations will come up under
 *    --no-prompt; as usual, these may be skiped with --yes.
 *
 *  - omit-stage-header:
 *
 *    Leave off the header with the stage name that is usually displayed
 *    above the stage description.
 *
 *  - no-op:
 *
 *    There is no operation associated with this stage; for example, it
 *    might just be a stage that prompts the user before going on to the
 *    next step.  Without the 'no-op' flag, Drush will advise the user
 *    that they must do the step manually if there is no 'callback' item.
 *
 *  - verify:
 *
 *    The name of a function callback that Drush will call to verify whether
 *    or not the current step, perhaps, may already have been performed.  If
 *    the verify function returns TRUE, then the current step will be skipped
 *    with a $stage_result of TRUE, without prompting the user.  Otherwise, the
 *    stage will run as usual.
 *
 *  - verify-message:
 *
 *    The verify message will be printed if the verify function returns TRUE.
 *
 *  - display:
 *
 *    The display callback is executed immediately after the stage description
 *    is printed.  This is for running code to output special information
 *    (a formatted table, etc.)
 *
 *  - callback:
 *
 *    The name of the function callback that Drush will call to perform the
 *    step.  If not specified, Drush will say: "Drush cannot perform this step;
 *    you must do it manually."  Set the 'no-op' flag (below) to avoid this
 *    message on steps that only confirm or select options to avoid this message.
 *
 *  - backup:
 *
 *    If set, a backup of the target site will be saved if the callback completes
 *    successfully. The backup will be named per the value of this item.
 *
 *  - next:
 *
 *    In its simplest form, 'next' is a string that describes the next
 *    stage to proceed to.  The following are equivalent:
 *
 *      'next' => 'name of next stage',
 *
 *            IS THE SAME AS
 *
 *      'next' => array(
 *        'ok' => 'name of next stage',
 *        'error' => 'STOP',
 *      ),
 *
 *    The return value from the callback function is used to select the
 *    next stage of the FSM to advance to.  TRUE is mapped to 'ok',
 *    FALSE is mapped to 'error'.  Other return values are possible,
 *    provided that they align with an entry in the 'next' array.
 *
 *  - selection-options:
 *
 *    Provides the list of prompts to present to the user.
 *
 *      '_run' => 'Do it for me.'
 *      '#skip' => 'Skip this step.'
 *      '#manual' => 'I will do this step manually.'
 *
 *    n.b. Key values that start with # will cause the callback function
 *    to be skipped with a $stage_result of TRUE, whereas all other
 *    values will call the callback function.  If the callback function
 *    is skipped and the selected option exists in the 'next' array,
 *    then the $stage_result will be set to the selected option key.
 *
 *  - required:
 *
 *    If specified, then the option to skip the step or do it manually is
 *    not provided.  This is used for stages that gather required information
 *    that will be used later, for example.  The value of this variable
 *    is used to replace the prompt for the '_run' item in the selection options.
 *
 *  - optional:
 *
 *    If specified, the user will be advised that this step is optional
 *    (does not always need to be done).
 *
 *  - not-always-needed:
 *
 *    Behaves like 'optional', but is not skipped by --skip-optional.
 *
 *  - unnecessary:
 *
 *    If specified, the user will be advised that this step is not necessary
 *    (does not apply when using the Drush workflow).
 *    Unnecessary steps are automatically considered optional.
 *
 *  - straightforward:
 *
 *    This step does not take much thought or consideration on the part
 *    of the user.  Straightforward steps are by default run automatically
 *    (without prompting).  They will be prompted if --confirm-all is
 *    selected.
 *
 */
function _drush_upgrade_stages($target_version) {
  $stages['START'] = array(
    'omit-stage-header' => TRUE,
    'continue' => TRUE,
    'next' => 'UPGRADE.txt Major Upgrade Step 1',
  );
  // This list of stages is taken from UPGRADE.txt from Drupal 7.10.
  // The intention is for the stages to track changes in UPGRADE.txt,
  // and for the code to track the descriptions in the stages table.
  // Report discrepancies in the drush_sup issue queue.
  $stages['UPGRADE.txt Major Upgrade Step 1'] = array(
    'description' =>
      "
        1. Check on the Drupal 7 status of your contributed and custom modules and
           themes. See http://drupal.org/node/948216 for information on upgrading
           contributed modules and themes. See http://drupal.org/node/895314 for a list
           of modules that have been moved into core for Drupal 7, and instructions on
           how to update them. See http://drupal.org/update/modules for information on
           how to update your custom modules, and http://drupal.org/update/theme for
           custom themes.

           You may decide at this point that you cannot upgrade your site, because
           needed modules or themes are not ready for Drupal 7.
      ",
    'callback' => 'drush_upgrade_check_modules_and_themes',
    'continue' => TRUE,
  );
  $stages['UPGRADE.txt Major Upgrade Prompt'] = array(
    'prompt' => 'Please review the upgread readiness report. You may want to uninstall modules that are not ready to upgrade yet.',
    'omit-stage-header' => TRUE,
    'no-op' => TRUE,
    'selection-options' => array('#skip' => 'Begin upgrade'),
  );
  $stages['UPGRADE.txt Major Upgrade Step 2'] = array(
    // TODO: Eventually we will be picking our finite state machine stages
    // based on the version of Drush being upgraded, so we'll be able to
    // rewrite the text below to presume a 6.x-to-7.x upgrade.
    'description' =>
      "
        2. Update to the latest available version of Drupal 6.x (if your current version
           is Drupal 5.x, you have to upgrade to 6.x first). If you need to update,
           download Drupal 6.x and follow the instructions in its UPGRADE.txt. This
           document only applies for upgrades from 6.x to 7.x.
      ",
    'note' => 'You must also insure that all of your contrib modules are up to date!',
    'verify' => '_drush_upgrade_site_already_latest_version_drupal',
    'selection-options' => array('#skip' => 'Continue upgrade with out-of-date Drupal (Not recommended -- failure probable).'),
  );
  $stages['UPGRADE.txt Major Upgrade Step 3'] = array(
    'description' =>
      "
        3. Log in as user ID 1 (the site maintenance user).
      ",
    'continue' => TRUE,
    'unnecessary' => 'You will not need to log in if Drush is used to perform all upgrade steps for you.',
  );
  $stages['UPGRADE.txt Major Upgrade Step 4'] = array(
    'description' =>
      "
        4. Go to Administer > Site configuration > Site maintenance. Select
           \"Off-line\" and save the configuration.
      ",
    'verify' => '_drush_upgrade_site_already_offline',
    'verify-message' => 'The source site is already offline.',
    'drush-command' => 'variable-set',
    'drush-arguments' => array('maintenance_mode', '1'),
    'drush-options' => array('always-set' => TRUE),
    'optional' => 'When Drush does the steps of your site upgrade, it always modifies the working copy of your site; the original site is left unchanged. You may wish to take your site off-line anyway.',
  );
  $stages['UPGRADE.txt Major Upgrade Step 5'] = array(
    'description' =>
      "
        5. Go to Administer > Site building > Themes. Enable \"Garland\" and select it as
           the default theme.
      ",
    'optional' => 'Drush will set the administrative theme to "Garland" in the target site without affecting the source site.',
  );
  $stages['UPGRADE.txt Major Upgrade Step 6'] = array(
    'description' =>
      "
        6. Go to Administer > Site building > Modules. Disable all modules that are not
           listed under \"Core - required\" or \"Core - optional\". It is possible that some
           modules cannot be disabled, because others depend on them. Repeat this step
           until all non-core modules are disabled.

           If you know that you will not re-enable some modules for Drupal 7.x and you
           no longer need their data, then you can uninstall them under the Uninstall
           tab after disabling them.
      ",
    'unnecessary' => 'Drush will automatically disable all non-core modules in the target site without affecting the source site. You may disable or uninstall contrib modules now if you wish; any that you do not do now will be disabled by Drush after Step 12.',
  );
  $stages['UPGRADE.txt Major Upgrade Step 7'] = array(
    'description' =>
      "
        7. On the command line or in your FTP client, remove the file

             sites/default/default.settings.php
      ",
    'unnecessary' => 'Drush will remove default.settings.php from the target site later, after Step 12.  You may remove it from the source sit now if you wish, but doing so is unnecessary.',
  );
  $stages['UPGRADE.txt Major Upgrade Step 8'] = array(
    'description' =>
      "
        8. Remove all old core files and directories, except for the 'sites' directory
           and any custom files you added elsewhere.

           If you made modifications to files like .htaccess or robots.txt, you will
           need to re-apply them from your backup, after the new files are in place.
      ",
    'unnecessary' => 'Drush will create your upgraded site in a new location; it is not necessary to remove any core files from the source site.',
  );
  $stages['UPGRADE.txt Major Upgrade Step 9'] = array(
    'description' =>
      "
        9. If you uninstalled any modules, remove them from the sites/all/modules and
           other sites/*/modules directories. Leave other modules in place, even though
           they are incompatible with Drupal 7.x.
      ",
    'unnecessary' => 'Drush will create your upgraded site in a new location; it is not necessary to remove any contrib module files from the source site.',
  );
  $stages['UPGRADE.txt Major Upgrade Step 10'] = array(
    'description' =>
      "
        10. Download the latest Drupal 7.x release from http://drupal.org to a
            directory outside of your web root. Extract the archive and copy the files
            into your Drupal directory.

            On a typical Unix/Linux command line, use the following commands to download
            and extract:

              wget http://drupal.org/files/projects/drupal-x.y.tar.gz
              tar -zxvf drupal-x.y.tar.gz

            This creates a new directory drupal-x.y/ containing all Drupal files and
            directories. Copy the files into your Drupal installation directory:

              cp -R drupal-x.y/* drupal-x.y/.htaccess /path/to/your/installation

            If you do not have command line access to your server, download the archive
            from http://drupal.org using your web browser, extract it, and then use an
            FTP client to upload the files to your web root.
      ",
    // TODO: If running in "reuse" mode, notify the user that Drupal is already in place, and skip this step. As it is, this step will seem to run, but will do nothing.
    'callback' => '_drush_upgrade_site_download_next_version_drupal',
    'straightforward' => TRUE,
  );
  $stages['UPGRADE.txt Major Upgrade Step 11'] = array(
    'description' =>
      "
        11. Re-apply any modifications to files such as .htaccess or robots.txt.
      ",
    'options' => array(
      'core-unmodified' => array(
        'description' => 'Drupal core, including .htaccess and robots.txt, is unmodified; continue upgrading without checking.',
        'next' => TRUE,
      ),
    ),
    'not-always-needed' => "If you had no modifications to .htaccess or robots.txt, you don't need to do anything here.",
  );
  $stages['UPGRADE.txt Major Upgrade Step 12'] = array(
    'description' =>
      "
        12. Make your settings.php file writeable, so that the update process can
            convert it to the format of Drupal 7.x. settings.php is usually located in

             sites/default/settings.php
      ",
    'note' => 'Note that Drush will instead *copy* your settings.php file to its new location.  The copy is made writable, but the original may remain write-protected.',
    'callback' => '_drush_upgrade_site_make_settings_writable',
    'straightforward' => TRUE,
  );
  $stages['UPGRADE.txt Major Upgrade Step 12-a'] = array(
    'description' =>
      "
        12-a. Drush-specific step:  Drush will now do steps 5 and 6 of UPGRADE.txt,
              and set the site theme to Garland and disable all contrib modules.
              Before it does this, it makes a copy of your database, and does
              all modifications on the *copy*, leaving your source site unchanged.
              Drush will also uninstall all modules specified via the --uninstall option
              at this time.
      ",
    'callback' => '_drush_upgrade_site_copy_database_and_disable_contrib',
    'straightforward' => TRUE,
  );
  $stages['UPGRADE.txt Major Upgrade Step 12-b'] = array(
    'description' =>
      "
        12-b. Drush-specific step:  Drush will now do step 7 of UPGRADE.txt, and
              remove sites/default/default.settings.php from the target site.
      ",
    'verify' => '_drush_upgrade_site_verify_default_settings_removed',
    'callback' => '_drush_upgrade_site_remove_default_settings',
    'straightforward' => TRUE,
  );
  $stages['UPGRADE.txt Major Upgrade Step 13'] = array(
    'description' =>
      "
        13. Run update.php by visiting http://www.example.com/update.php (replace
            www.example.com with your domain name). This will update the core database
            tables.

            If you are unable to access update.php do the following:

            - Open settings.php with a text editor.

            - Find the line that says:
              \$update_free_access = FALSE;

            - Change it into:
              \$update_free_access = TRUE;

            - Once the upgrade is done, \$update_free_access must be reverted to FALSE.
      ",
    'straightforward' => TRUE,
    'note' => 'Note that Drush will copy your database and run updatedb on the copy.  The source database remains unchanged.',
    'callback' => '_drush_upgrade_site_updatedb',
  );
  $stages['UPGRADE.txt Major Upgrade Step 14'] = array(
    'description' =>
      "
        14. Backup your database after the core upgrade has run.
      ",
    'backup' => 'upgraded-core',
    'backup_message' => "Upgraded Core",
    'straightforward' => TRUE,
    'callback' => '_drush_upgrade_site_backup_after_updatedb',
  );
  $stages['UPGRADE.txt Major Upgrade Step 15'] = array(
    'description' =>
      "
        15. Replace and update your non-core modules and themes, following the
            procedures at http://drupal.org/node/948216
      ",
    'note' => 'Drush will walk through these instructions for each module.  Note that all of the new modules will be downloaded up-front now; they will be moved into place later.',
    'prepare' => '_drush_site_upgrade_reset_module_to_upgrade',
    'verify' => '_drush_site_upgrade_select_contrib_module_to_upgrade',
    'callback' => '_drush_site_upgrade_prepare_module_upgrade',
    'options' => array(
      'module-order' => array(
        'description' => 'A list of modules to upgrade first.',
        'list' => TRUE,
      ),
    ),
    'next' => array(
      '#progress' => 'Upgrade Progress',
      '#done' => 'Upgrading Contrib Modules from Drupal 6 to Drupal 7 Finish Up',
    ),
  );
  $stages['Upgrading Contrib Modules from Drupal 6 to Drupal 7 Step 1'] = array(
    'description' =>
      "
         1. Check your notes, and the UPGRADE.txt files inside each module, to see if any special upgrade instructions apply
      ",
    'prepare' => '_drush_site_upgrade_show_module_upgrade_txt',
    'no-op' => TRUE,
    'required' => TRUE,
    'options' => array(
      // If 'module-order' is set, then autoconfirm this step as well.
      'module-order' => array(
        'next' => TRUE,
      ),
    ),
    'next' => array(
      '#postpone' => 'UPGRADE.txt Major Upgrade Step 15',
    ),
  );
  $stages['Upgrading Contrib Modules from Drupal 6 to Drupal 7 Step 2'] = array(
    'description' =>
      "
         2. Fully remove the old module directory inside sites/all/modules/
      ",
    'unnecessary' => 'Note that Drush builds the target site in a new directory; the modules directory therefore starts off empty, and there is therefore no need to delete anything. If there is for some reason an old module directory in place, Drush will automatically delete it when downloading the new module.',
  );
  $stages['Upgrading Contrib Modules from Drupal 6 to Drupal 7 Step 3'] = array(
    'description' =>
      "
         3. Download, unpack and move the new module directory to sites/all/modules/
      ",
    'straightforward' => TRUE,
    'prepare' => '_drush_site_upgrade_show_module_location',
    'verify' => '_drush_site_upgrade_verify_module_code_exists',
    'verify-message' => 'The correct module already exists at the destination directory.',
    'callback' => '_drush_site_upgrade_get_module_code',
  );
  $stages['Upgrading Contrib Modules from Drupal 6 to Drupal 7 Step 4'] = array(
    'description' =>
      "
         4. Enable the module
      ",
    'straightforward' => TRUE,
    'callback' => '_drush_site_upgrade_enable_module',
    'next' => array(
      'error' => 'Upgrading Contrib Modules from Drupal 6 to Drupal 7 Handle Enable Errors',
    ),
  );
  $stages['Upgrading Contrib Modules from Drupal 6 to Drupal 7 Step 5'] = array(
    'description' =>
      "
         5. Run update.php again (you should have run it once already when upgrading core) and check for any message output.
      ",
    'straightforward' => TRUE,
    // TODO: It would be nifty if we could include a verify function that determined if the module needed any database updates.
    // This should probably be done up-front in step 15.
    'callback' => '_drush_upgrade_site_updatedb',
    'next' => array(
      'error' => 'Upgrading Contrib Modules from Drupal 6 to Drupal 7 Handle Update Errors',
    ),
  );
  $stages['Upgrading Contrib Modules from Drupal 6 to Drupal 7 Step 6'] = array(
    'description' =>
      "
         6. Back up your database and files again, between each module update, so that if there is a problem, you can revert to the most recent update.
      ",
    'straightforward' => TRUE,
    'backup' => 'upgraded-contrib',
    'backup_message' => "Upgraded Contrib Module !module_to_upgrade",
    'next' => 'UPGRADE.txt Major Upgrade Step 15',
  );
  $stages['Upgrade Progress'] = array(
    'description' =>
      "
          Current upgrade progress:
      ",
    'display' => 'drush_upgrade_site_upgrade_progress',
    'no-op' => TRUE,
    'selection-options' => array('_run' => 'Continue upgrade'),
    'next' => 'UPGRADE.txt Major Upgrade Step 15',
  );
  $stages['Upgrading Contrib Modules from Drupal 6 to Drupal 7 Finish Up'] = array(
    'description' =>
      "
         If you have a Drupal 7 version of the theme you were using for Drupal 6, place it in sites/all/themes/ and reenable it.
      ",
    'prepare' => '_drush_site_upgrade_finish_up_contrib',
    'next' => 'Post-upgrade Step A',
  );
  $stages['Upgrading Contrib Modules from Drupal 6 to Drupal 7 Handle Enable Errors'] = array(
    'description' =>
      "
        If you have errors when you attempt to enable a module:

        Note any error messages you see.
        Check to see if perhaps Drush simply could not find the module; this can happen if a module
        is moved to another project.  If this happens, you can manually download the new project and
        select \"Try to enable this module again\".  Drush will first upgrade other modules that had
        no failures before retrying those that had errors the first time.

        If you opt not to upgrade this module, you may upgrade it later by enabling the module
        and then running updatedb.
      ",
    'not-always-needed' => TRUE,
    'selection-options' => array(
      '#skip' => 'Try to enable this module again',
      '_run' => 'Do not upgrade this module',
    ),
    'callback' => '_drush_site_upgrade_remove_broken_module',
    'next' => 'UPGRADE.txt Major Upgrade Step 15',
  );
  $stages['Upgrading Contrib Modules from Drupal 6 to Drupal 7 Handle Update Errors'] = array(
    'description' =>
      "
        If you have errors when you run the update.php script:

        Note any error messages you see.
        Restore your site to its previous state, using the file and database backups you created before you started the upgrade or update process. Do not attempt to do further updates or upgrades on a site that had update problems.
        Consult one of the support options listed on http://drupal.org/support.
      ",
    'next' => 'UPGRADE.txt Major Upgrade Step 15',
  );
  $stages['Post-upgrade Step A'] = array(
    'description' =>
      "
        A. Copy files directory from the source site to the upgrade site.
      ",
    'callback' => '_drush_site_upgrade_copy_files',
  );
  $stages['Post-upgrade Step B'] = array(
    'description' =>
      "
        B. Enable the core module content_migrate.
      ",
    'selection-options' => array(
      '_run' => 'Enable content migrate and show available migrations.',
      '#skip' => 'Do not enable the content migrate module.',
    ),
    'callback' => '_drush_site_upgrade_enable_content_migrate',
    'next' => array(
      '#skip' => 'Post-upgrade Step C',
    ),
  );
  $stages['Post-upgrade Step B.2'] = array(
    'description' =>
      "
        B.2. Migrate fields.
      ",
    'display' => '_drush_site_upgrade_show_migrations',
    'selection-options' => array(
      '_run' => 'Run all available migrations.',
      '#skip' => 'Do not migrate fields.',
    ),
    'callback' => '_drush_site_upgrade_migrate_fields',
  );
  $stages['Post-upgrade Step C'] = array(
    'description' =>
      "
        C. Fix up some file system permissions.

           Note: Drush site upgrade does not know what file system permissions
           it should use; it just makes %files and %temp world-writable for
           development.
      ",
    'callback' => '_drush_site_upgrade_fix_permissions',
  );
  $stages['Post-upgrade Step D'] = array(
    'description' =>
      "
        D. Run cron, for good measure.
      ",
    'straightforward' => TRUE,
    'callback' => '_drush_site_upgrade_run_cron',
  );
  $stages['UPGRADE.txt Major Upgrade Step 16'] = array(
    'description' =>
      "
        16. Go to Administration > Reports > Status report. Verify that everything is
            working as expected.
      ",
    'note' => 'Drush can also show you this information in the console via the `drush status-report` command.',
    'straightforward' => TRUE,
    'drush-command' => 'status-report',
    'drush-site' => '@upgrade-target',
  );
  $stages['UPGRADE.txt Major Upgrade Step 17'] = array(
    'description' =>
      "
        17. Ensure that \$update_free_access is FALSE in settings.php.
      ",
    'straightforward' => TRUE,
    'verify' => '_drush_upgrade_update_free_access_already_false',
    'verify-message' => '$update_free_access is already FALSE in settings.php.',
  );
  $stages['UPGRADE.txt Major Upgrade Step 18'] = array(
    'description' =>
      "
        18. Go to Administration > Configuration > Development > Maintenance mode.
           Disable the \"Put site into maintenance mode\" checkbox and save the
           configuration.
      ",
    'straightforward' => TRUE,
    'verify' => '_drush_upgrade_site_already_online',
    'verify-message' => 'The source site is already online.',
    'drush-command' => 'variable-set',
    'drush-arguments' => array('maintenance_mode', '0'),
    'drush-options' => array('always-set' => TRUE),
  );
  $stages['Upgrade Complete'] = array(
    'description' =>
      "
        Congratulations!

           Your upgrade is now complete.  There are still some things that you
           will need to do before your site is working and ready to go online.

             *  Upgrade your theme, or create a new one, perhaps using
                either zen or omega (both of which contain Drush commands
                to create a new theme template)

             *  Consider using Drupal-to-Drupal migration to keep the content
                on your new site in sync with additions and changes made on
                the live site; see http://drupal.org/sandbox/mikeryan/1234554.
                Since this is still a sandbox project, you may prefer to just
                run drush site-upgrade again to pull in new content.

             To see the status messages output by updatedb during the upgrade
             of your site, run:

                drush @target-site site-upgrade-progress

             Good luck with your upgraded site!
      ",
    'continue' => TRUE,
    'next' => 'STOP',
  );
  return $stages;
}
