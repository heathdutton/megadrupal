
ABOUT Quick update
--------------------------------------------------------------------------------
Enhances Drupal core update features

The Quick update module provides a quick way to batch install new projects
and install missing dependency projects.

Drupal core provides a way to install module or theme one by one,
but you can install multiple projects via the Quick update module.
There is an admin UI to search the most installed projects easier.

Additional, Quick update module finds missing dependency projects for you.
And you can just select all missing dependency projects from the admin UI,
then you can install all of them in a batch process.

The Quick update module depends on the Update module and
uses the same workflow as the Update module.
Thus, you can run the updates via the admin update page
at /admin/reports/updates/update.

There are some custom Drush commands to list or install missing dependency
projects that include modules and themes.

- "drush qup-list-md" lists current missing dependency projects.
- "drush qup-dl" downloads projects and their dependency projects.
- "drush qup-dl-md" downloads all missing dependency projects.

REQUIREMENTS
--------------------------------------------------------------------------------
- Update module.
