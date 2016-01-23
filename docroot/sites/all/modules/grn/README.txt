Git Release Notes
=================

For more information, visit the project's page on drupal.org:

  http://drupal.org/project/grn

If you have any requests or bug reports, please create an issue:

  http://drupal.org/project/issues/grn.

This is a Git port of `cvs-release-notes.php`, which lived in the tricks
repository back in CVS land.  The original script link does not exist after the
Great Git Migration took place.

REQUIREMENTS
------------
Working Drush installation
Working git installation (Tested with 1.7.3.1)

INSTALLATION
------------

The script is a Drush command and it must be installed into `.drush` directory
(for Windows: `%USERPROFILE%/.drush/`, for Linux: `~/.drush`) in a sub-directory
named `grn`.

You may install the module using Drush (`drush dl grn`) or just manualy expand
the archive you download from the module's drupal.org page.

For more information about installing Drush commands, please see the Drush
documentation (http://drush.ws)

FEATURES & USAGE
----------------

This script is a Drush command that generates release notes from commits
between two git reference object.  This is the basic syntax for the command
usage:

    drush release-notes <start> <end>

The `<start>` and `<end>` arguments can be tag names or commit SHA1 hashes.
You can use a branch name for the `<end>` argument (for example, when you have
not had a tag created yet. The `<end>` argument can also be a remote branch.
For example when the branch is not checked out localy you may try something
like this:

    drush rn 6.x-1.0 origin/6.x-1.x

If you only provide the <end> tag, the previous tag before that will be used as
<start> tag.  If both tags are ommitted, the latest tag will be used as the
<end> tag.

The command `release-notes`, has the aliases: `relnotes` and `rn` and there
is support for `--pipe`.  The command options are:

 --baseurl
   Set the base url for all issue-links. Defaults to /node/ for Drupal.org
   usage.  Issue number will be appended to path or replace "%s".
 --changelog
   Display the commits in the format for CHANGELOG.txt as expected by
   drupal.org.
 --commit-count
   If set, output will show the number of commits between the two tags
 --commit-links
   Attach a link to the commit in drupalcode.org repository viewer to the end
   of the commit lines.
 --git=</path/to/git>
   Path to the git binary, defaults to "git"
 --nouser
   Do not try to link to user page using the /u/alias, as used in drupal.org
 --pretty=<%s>
   Pretty format of the message, see the git-log man page (section "PRETTY
   FORMATS")
 --reverse
   Display the commits from old to new instead of the default Git behavior that
   is new to old.

The commit messages undergo some modifications before reaching the output.
 - Issue numbers of the form `#12345` will be transformed into links of the form
 `<a href="/node/12345">#12345</a>`.
 - Commit message prefixes like "Issue ", "Patch ", or "- " are removed.

There is a `hook_release_notes_alter()` that can be used to further process the
output.

CREDITS
-------

* Originally written for CVS by dww.
* Ported to git by [Josh The Geek](http://drupal.org/user/926382)
  for [#1002410](http://drupal.org/node/1002410).
