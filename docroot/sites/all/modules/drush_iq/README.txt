DESCRIPTION
===========

The Drush Issue Queue Commands project (drush_iq) provides a set of
commands to interact with the issue queue on Drupal.org.  These commands
make it easier for beginners and faster for beginning and experienced git
users alike to create and apply patches.

The commands provided by drush_iq includes:

 iq-info                   Show information about an issue.
 iq-apply-patch            Apply a patch from an issue.
 iq-reset                  Remove an applied patch.
 iq-diff                   Create a patch using `git format-patch`.
 iq-submit                 Create a patch and upload it to drupal.org.
 iq-merge                  Merge patch back into the original branch.
 iq-branch                 Manage branches created by iq commands.
 iq-create-commit-comment  Create a commit comment for an issue.
 iq-lint                   Run lint (php -l) on files that have changed.
 iq-open                   Open a file changed by a patch.

For more information on these commands, see the "USAGE" section below.

For more information on creating and applying patches on drupal.org, see
the advanced patch contributor guide (http://drupal.org/node/1054616),
which descripts the process followed by the drush_iq commands.


REQUIREMENTS
============

drush_iq works with Drush version 6 or later.
The iq-submit command also depends on the PHP cURL APIs.


INSTALLATION
============

$ drush dl drush_iq
$ drush dl drush_iq_extras

For instructions on installing the PHP cURL APIs, see:

http://www.php.net/manual/en/curl.installation.php

There are some user comments below the manual text that are
particularly helpful for some environments.


DRUSH IQ-SUBMIT CONFIGURATION
=============================

Drush iq-submit requires that your drupal.org username and password
be specified as options to the command.  This identifies you to
drupal.org, and allows your uploaded patches to be correctly associated
with your account.  Also, drupal.org does not allow anonymous
comments to be posted.

To avoid having to type your username and password every time you
use iq-submit, it is recommended that you configure a command-specific
option record in your drushrc.php file.

For example:

$command_specific['iq-submit']['user'] = 'troubador3';
$command_specific['iq-submit']['pass'] = 'correcthorsebatterystaple';

Note that drushrc.php files are executable PHP files; you can put any
executable PHP code you want inside them.  For example, if you want
to store your drupal.org password in a separate file, you could do
something like this:

$home = drush_server_home();
$pw_file = $home . '/.drupalorgpw';
$command_specific['iq-submit']['pass'] = file_get_contents($pw_file);


USAGE - WORKFLOW FOR PATCH CONTRIBUTOR
======================================

If you have found a patch in the issue queue that you would like to work
on, the following workflow will allow you to contribute an updated patch
on the same issue.

0. Select a local Drupal project to work on

$ cd /path/to/drupal/sites/all/PROJECT

1. Download the project with Drush using git

$ drush pm-download PROJECT --package-handler=git_drupalorg --select

Select the desired development version from the list presented.  The
version downloaded should match the version specified in the drupal.org issue.

2. Apply the latest patch from the issue

$ drush iq-apply-patch 12345-#6

Specify the issue number and the comment number contianing the patch you
would like to apply.  If you specify an issue number without a comment number,
then drush iq-apply-patch will select the most recent patch.  If no patch
number is given, iq-apply-patch will present a list of available patches
taken from a list of open windows containing issues on drupal.org.  [*]

When iq-apply-patch applies the patch, it will create a new working
branch for the patch, and will commit the modified files.  It will also
run the iq-lint command, which will warn you if the patch introduces any
syntax errors into any of the modified php files.

[*] Presently, drush iq-apply-patch can only read the open window list
on Linux.  See: http://drupal.org/node/1864142

3. Open up a file changed by the patch

$ drush iq-open

If you have already set up an IDE to work on your site, you may prefer to
use features of your IDE to find the code you would like to work on.  If
you are just making a quick change to a patched file, though, you might find
the iq-open command to be convenient.  Drush iq-open will present a list of
all modified files, and will open the selected one using the configured
editor, as determined by the EDITOR environment variable.

If you would like to filter the list of presented files, you may use the
--search option to find files where the specified text was either added
or removed by the applied patch.

$ drush iq-open --search='some text added or removed by patch'

If you want to use Drush IQ to post an interdiff of your modified changes,
do not commit your changes to the working branch; leave them unstaged.

4. Test!

Unfortunately, there is no Drush iq-test command -- you still have to do
this step manually!

5. Inspect your changes

$ git diff

  - or -

$ drush iq-diff

Run `git diff` to see your unstaged changes.  If you do not commit
changes to the working branch, then you unstaged changes will correspond
to the interdiff between your latest work, and the patch at the time
it was originally applied.

Run `drush iq-diff` to show the complete, up-to-date changes to the module,
including the changes made in the original patch and your local modifications
combined together.  Drush IQ uses `git format-patch` to create this diff,
so it will contain author information.

6. Submit your patch back to drupal.org

$ drush iq-submit 'This is my modest addition to your awesome patch'

The iq-submit command will use `git diff` to create an interdiff, and
`drush iq-diff` to create a complete patch.  Both will be posted to the
original issue on drupal.org if the interdiff is small compared to the
size of the patch (30% of the size or less); otherwise, only the new,
complete patch will be posted.


USAGE - WORKFLOW FOR CREATING A NEW PATCH
=========================================

0. Select a local Drupal project to work on

$ cd /path/to/drupal/sites/all/PROJECT

1. Download the project with Drush using git

$ drush pm-download PROJECT --package-handler=git_drupalorg --select

Select the desired development version from the list presented.  The
version downloaded should match the version specified in the drupal.org issue.

2. Make your modifications and test

3. Create a new issue on drupal.org

http://drupal.org/node/add/project-issue/PROJECT

Save the new issue, and make note of its issue number for use in the next
step.

4. Submit your patch on your issue

$ drush iq-submit 'Here is a novel approach to the problem described.' 12345

The second parameter to iq-submit should be the issue number of the
issue created in step 3, above.


USAGE - WORKFLOW FOR MODULE MANTAINERS
======================================

If someone has submitted a patch in the drupal.org issue queue for a module
that you maintain, the following workflow will allow you to easily apply,
test and commit the patch back to your modules' repository.

0. Select a local Drupal project to work on

$ cd /path/to/drupal/sites/all

1. Download the project following the git instructions on your project page

$ git clone --branch 7.x-1.x USERNAME@git.drupal.org:project/PROJECT.git
$ cd PROJECT

You probably already have a git checkout of your latest development
branch.  If you don't, follow the git instructions found in the
Version Control tab of your project's project page:

http://drupal.org/project/PROJECT/git-instructions

If you already have cloned the project, make sure it is up to date via:

$ git pull

2. Apply the latest patch from the issue

$ drush iq-apply-patch 12345-#6

Apply the patch as described in the workflow for a patch contributor.
Note that iq-apply-patch will automatically use the --author option
when committing the patch to the working branch, so author credit will
be given, even if the contributor used `git diff` to create the patch.

3. Modify the submitted patch, if necessary, and test

Modification is optional, but be sure to test all submitted patches carefully.

4. Commit your changes as usual, using git:

$ git add .
$ git commit -m "Minor adjustment to contributor's excellent submission"

  - or -

$ git add .
$ git commit --amend

Commit back to the working branch.  Make multiple commits, if desired.
Use --amend if you'd like to merge trivial changes into the contributor's
commit without creating an additional commit of your own.  This should
only be done for truly minor changes, like spelling corrections, etc.

5. Merge and push

$ drush iq-merge
$ git push

The iq-merge command can, if desired, squash all of the commits on the
working branch down into a single commit.  Just specify the --squash
option to do this.  If you always want to squash merged commits, set
up a command-specific option for iq-merge in your drushrc.php file:

$command_specific['iq-merge']['squash'] = TRUE;

Note that if you, the module maintainer, made a commit on the working
branch and then use --squash to compress multiple commits down to one,
you will ERASE the author credit for the other contributors with commits
on the working branch.  To avoid erasing author credit, either use
--amend when committing, as recommended above, or avoid using --squash
when merging.


USAGE - WORKFLOW FOR CONTINUING DEVELOPMENT
===========================================

Not every issue can be resolved with a single patch; often, several
patches must be submitted before everything is done.  Drush IQ provides
several ways to manage multiple and iterative issue development.

Resuming work on the main branch:

$ drush iq-reset

The drush iq-reset command will return you to the original branch.

IMPORTANT NOTE:  If you have unstaged changes, they will remain
when you switch back to the original branch.  If you want to keep
them separate, commit them first.

If you would like to delete all of your changes and your working
branch, you may add the --hard option to get rid of it.  If the
modifications already exist in a patch on a drupal.org (e.g. if
submitted via iq-submit), then you can always re-apply them later
with iq-apply-patch.

If you commit rather than delete your changes, you can easily
switch back to them later:

$ drush iq-branch

The iq-branch command will list all of the branches that were
created by Drush IQ.  You can easily switch back to resume work
on an issue by selecting the one you want from the list presented.

If you commit your changes after running iq-submit, and keep your
work-in-progress unstaged, then you may use iq-submit repeatedly
to submit new patches with interdiffs as work on your issue progresses.


GETTING INVOLVED / FUTURE ENHANCEMENTS
======================================

The workflows described above could be improved with the addition of
the following proposed features being tracked in the drush_iq issue queue:

http://drupal.org/node/1734884 - Add an iq-interdiff command

If Drush IQ had a command that could create interdiffs based on older
patches posted to the issue queue, then iq-submit could still create
an interdiff for a new patch, even if the newest changes had been
committed to the working branch.

http://drupal.org/node/1734888 - Make it easier to follow along...

Drush IQ could do a better job at (a) applying a new patch from the issue
queue on top of an issue branch already created by drush iq-apply-patch,
and (b) updating your issue branch (committing changes, renaming the
branch) after an iq-submit.

For more Drush IQ issues, see the issue queue:

http://drupal.org/project/issues/drush_iq
