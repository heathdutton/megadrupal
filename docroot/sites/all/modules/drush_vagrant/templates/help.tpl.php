<?php
/**
 * @file
 * Output for Drush Vagrant's help text.
 *
 * Variables:
 * - $short_help: The default one-line help text.
 * - $vagrant_commands: A table of currently available native Vagrant commands.
 * - $sub_commands: A table of currently available Drush Vagrant sub-commands.
 */
?>
<?php print $short_help; ?>

Usage: drush [<@alias>] vagrant <sub-command> [<arguments>] [<options>]

Available native Vagrant sub-commands:
<?php print $vagrant_commands; ?>

Sub-commands provided by Drush Vagrant:
<?php print $sub_commands; ?>

For additional help on sub-commands try: 'drush vagrant <sub-command> --help'
