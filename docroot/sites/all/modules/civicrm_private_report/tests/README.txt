Testing information for
CiviCRM Private Report module in Drupal 7.x

--------------------
Basic setup
--------------------

Drupal tests use an existing Drupal installation to run the tests, though the
tests themselves are run on a temporarary installation that's automatically
created for each test case.

Tests for this module are built on the assumption that CiviCRM is already
installed in the existing Drupal installation, and that CiviCRM is using its
own separate database.  Tests will likely fail horribly in environments that
don't meet this expectation.


--------------------
Patching Drupal 7
--------------------

CAUTION: This involves patching Drupal core, and may have unpredictable results
on your installation. It's NOT recommended to do this on a live Drupal install.

Something about the testing framework of Drupal 7's, combined with the codebase
of CiviCRM, causes tests to crash (that is, die in process, as opposed to
failing with reasonable test failure messages) during the creation of the
temporary testing installation. I've not been able to find the true source of
the problem, but the heart of it is that blocks are created directly in the
database, even though those blocks already exist in the database, resulting in a
fatal key constraint error. The quick solution is to ensure these database rows
are first deleted before attempting to insert them.

The patch at tests/drupal7.patch is meant to make this change, and remedies the
problem.
