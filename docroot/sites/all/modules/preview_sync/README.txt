Aims to solve the holy grail of Drupal problems - site preview. It does this in a different manner to all other preview systems that currently exist in Drupal.

Features
========
* Automate the synchronization of your environment to a separate 'preview' environment, and then performs arbitrary actions on the preview environment.
* Supports the publishing of all content in 'Needs Review' in workbench_moderation by default
* You can plug your own drush commands into the task queue, as it is alterable. For instance you may wish to support state_machine review content being published.
* There is a single administration UI to see what is happening with the preview sync, and all tasks run in the background - this means there is no blocking the administrative UI.
* AJAX auto updates the administration UI when a preview is active
* Preview sync records the history of each task, and can estimate based on the last 10 times the task ran, how long the current task will run for
* Queue is processed synchronously (each tasks waits for the next) in the background
* Preview Sync drush worker can run on all web servers in a cluster, there is locking in place to ensure only one web server will 'win'. This also ensures preview sync will continue to run if you lose a web server.

Requirements
============
* Drush 6.4.0 or later installed globally, the web server user (e.g. www-data) will require execute access
* Drush alias file, correctly configured and placed at /etc/drush/preview.aliases.drush.php. An example template file can be found in the module. The alias file should be readable by the webserver.
* For every environment you want to be able to preview, you will need a separate (logically not physically) Drupal environment. This Drupal environment will require the exact same code deployed to it as the site you are syncing from. It is best practise to automate this.

Integrations
============
* drush and drush alias file [required]
* environment support [required]
* views support [required]
* workbench_moderation support [optional]
* apachesolr [optional]
* PHP process control for stopping long running background tasks [optional]

Gotchas
=======
* The group name for the drush alias is currently locked at 'preview', do not change the file name preview.aliases.drush.php
* Do not use the site alias 'preview' inside the preview group. This seems to be a drush limitation (you can't have an alias @preview.preview)
* Ensure when you do a production code release, you also release to preview. The 2 environments require the same code. There is no need to run database updates on the preview server, just execute a preview sync after the production release to update the preview site's database.
* If you are running a cluster of web servers, ensure the database dump file is shared amongst them (e.g. using NFS), also ensure you don't place this in a publicly accessible area.

Find out more
=============
* Read the blog post about preview sync http://www.pixelite.co.nz/article/site-preview-the-holy-grail-of-drupal/
* Watch the 4 minute video demonstration https://www.youtube.com/watch?v=wJCyI4wqG9I
* See the DrupalSouth presentation http://bit.ly/drupalsouthsitepreview