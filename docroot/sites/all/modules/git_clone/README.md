# Git Clone

### What this module does
Clones, manages and synchronizes a specific git repository reference from a remotely hosted git repository. This allows
the cloned repository references to be consumed in a Drupal environment. Consumers of cloned git repositories are tools
that perform certain functions like parsing or building from several repository references.

An example of a consumer: the <a href="https://www.drupal.org/project/api" target="_blank">API module</a>

### What this module does not do
This module is not a "git repository manager" or "git repository viewer". It will not display the contents of a cloned
git repository reference in the UI. It does not stage/unstage or push changes of files.

All this module does is keep the working tree of a cloned repository reference in sync. This means that when cron runs,
it will pull down a branches the latest commits so it can be consumed.

### Requirements
Git Clone requires the Composer Manager and Entity API modules. This will not change for D7. D8 may lose this
requirement but unsure at the moment. The decision for using these two modules are as follows:
- <a href="https://www.drupal.org/project/composer_manager" target="_blank">Composer Manager</a>  
  Alows PSR-4 autoloading and library installation of <a href="http://gitonomy.com/doc/gitlib/master/" target="_blank">gitlib</a>,
  which is the backbone library that this module uses to interface with the git binary on the server.
- <a href="https://www.drupal.org/project/entity" target="_blank">Entity API</a>  
  Provides the entire admin interface for managing Git Clone. It also allows them to be exportable via Features.
  
### Post-Install
After you have installed the module, you will need to at least set the `file_git_clone_path` variable. It is highly
recommended that this path be somewhere outside your site's DOCROOT. Just ensure that whatever path is set, it is
writable by Drupal.

Depending on the server's include PATH settings, you may also need to explicitly set the `git_binary` variable to the
absolute path to the git binary on the server.

Both of these settings can be set from the UI located at `/admin/reports/status` or, optionally set in settings.php:
```php
$conf['file_git_clone_path'] = '/var/www/git_clone/example.com';
$conf['git_binary'] = '/usr/bin/git';
```

### Integrations
This module was created with the necessity to provide a better UI for the API module. Git Clone comes packaged with a
form alter that replaces the very manual "Directories" section of a branch edit form with a dynamic and easy to
use "Git Clone Repository" select dropdown.

Git Clone ensures that it is the first module ran on cron. This is necessary so the other modules that consume these
repositories are assured they will always have the latest code. Modules that need to frequent manually or independently
iterate over a Git Clone repository may do so using the provided `gitclone://` stream wrapper. This correlates to the
above `file_git_clone_path` variable.
 
Further documentation about integration can be found in this module's `git_clone.api.php` file.
