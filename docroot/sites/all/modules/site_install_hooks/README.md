# Site Install Hooks API
This is an API project / library that gives installation profiles (and select modules during install) the ability to
implement `hook_site_pre_install()` and `hook_site_post_install()`.

## Is this the right module for me?
Consider the following questions to determine if this is the right approach for you:

  1. Are you building an installation profile?
  2. Have you ever needed to execute code right before all of the modules the profile depends on get installed?
  3. How about executing code after the entire profile has been installed?

If you answered YES to at least two of the questions above, then this code is for you!

You can technically accomplish what this code does for you simply by implementing your own install tasks via
`hook_install_tasks()` or `hook_install_tasks_alter()`, but why do that when this code abstracts having to deal with
that away from you?

## Sample Use Cases
Here are some scenarios where you'd want to use this approach:

  - You need to configure certain site settings before installing a module that requires those settings
    (`hook_site_pre_install()`).

  - You need to modify user passwords, tweak menu paths, or adjust settings (such as the site name) that would otherwise
    be bashed if set in `hook_install()` in the installation profile (`hook_site_post_install()`).</li>

## How to Use
Do not install this project like a regular module! It serves no purpose after site install.

### Manual install
The project can be placed in any directory belonging to your install profile. If installed as a standard library, the
correct location for it is:

    profiles/[yourprofile]/libraries/site_install_hooks

### Using Drush Make
If using a Drush Make makefile for packaging/distribution, you can place this project in the standard location with the
following lines in the `.make` file:

    libraries[site_install_hooks][type] = "library"
    libraries[site_install_hooks][download][type] = "get"
    libraries[site_install_hooks][download][url] = "http://ftp.drupal.org/files/projects/site_install_hooks-7.x-VERSION.tar.gz"

Where VERSION is the desired version.

### Initializing the API
You also need to initialize the API with the following lines of code in your `.profile`:

    if (!function_exists('site_install_hooks_initialize')) {
      require_once('libraries/site_install_hooks/site_install_hooks.inc');
    }

    site_install_hooks_initialize('YOUR_PROFILE_NAME');

Where YOUR_PROFILE_NAME is the machine name of your profile.

If your profile has its own implementation of `hook_install_tasks_alter()`, you will need to add a call at the top of
your `hook_install_tasks_alter()` implementation that calls `site_install_hooks_install_tasks_alter()`:

    function YOUR_PROFILE_NAME_install_tasks_alter(&$tasks, $install_state) {
      site_install_hooks_install_tasks_alter($tasks, $install_state);
      ... YOUR CODE ...
    }

### Implementing Hooks
#### What Hooks are Available and When Are They Invoked?
- `hook_site_pre_install()` - Invoked right before the modules that the installation profile depends on are installed
  and enabled.
  
- `hook_site_post_install()` - Invoked at the very end of the installation process as the final task, to give modules or
  the profile a chance to initialize anything last minute (update blocks, adjust content, set-up users, etc).

#### Using in the Installation Profile
Either `hook_site_pre_install()`, `hook_site_post_install()`, or both can be implemented in the `.install` file of your
profile.

Normally, the `.install` file of the install profile does not get loaded until after dependent modules have been
installed; this library automatically handles loading of the `.install` file to search for your site post-install hook.

#### Using in Modules
In addition to use in the profile itself, `hook_site_post_install()` can also be implemented in the `.install` file of
modules or features, as a handy way for features to perform one-time installation logic only when the site is being
installed (as opposed to when being installed on an existing site for the first time).

By design, `hook_site_pre_install()` cannot be implemented by modules because it is invoked before the site knows what
modules are going to be available.

## Compatibility with Profiler
This library is not directly compatible with Profiler since both need to define `hook_install_tasks_alter()` on behalf
of the installation profile. [A patch](https://www.drupal.org/node/2418335) has been submitted to Profiler that includes
the same functionality provided by this library, so if you're using Profiler, you should apply that patch instead of
trying to use this library. The interface is the same (the patch was based on the code here).

This project is useful on its own if you're not basing your profile on Profiler.