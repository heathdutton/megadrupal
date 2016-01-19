Rush Job Components  {#doc-job-components}
===============

[TOC]

@brief An overview of the components of Drush Rush jobs.


Rush Job Components   {#page-job-components}
====================

@todo Add definition and picture from use case

## Jobs and Builds {#sec-builders-jobs-builds}

*Note:* **The following is an extended version of the Jobs and Builds section for Rush users. This version has more information specifically for Rush Job builders. **

The two main components of Drush Rush are **jobs** and **builds**.

Jobs define what to produce and builds are what is produced.

Jobs define parameters and operations used to execute a build (typically installing a Drupal website).

Conversely, builds are the product of the execution of a job.

Jobs, once written rarely change.  Builds are created and deleted often.

Developers share job folders to give others the ability to create the build defined by the job.

To recap:

- Jobs
    - A package of parameters and operations, which when run via the drush rush command will produce a build.

- Builds
    - The product of a job; typically a Drupal website, though a build may be something as simple as adding a domain to a hosts file.

- Parameters
    - Defined in params.ini or params.php files, parameters provide variables for a build, such as site name, repo information, database connection strings, and really anything needed to complete a build.

- Operations
    - Operations run code commands (often in Drush, shell, php, git, and mysql) to act upon the file system, interact with other systems, and to build Drupal websites.

## File Types {#sec-file-types}

- Parameter Files
    - Contain environmental, application, and job based variables to use at run time to build a project.
    - Parameter files come in two flavors: `params.ini` and `params.php`
    - Most jobs use `params.ini` files.  `params.php` files give you the option to use business logic when setting rush params.

- Rush Files
    - Contain a list of operations to complete a build.
    - Rush files come in two flavors: `rush.ini` and `rush.php`
    - Most jobs use `rush.ini` files. `rush.php` files give you the option to use business logic when running drush operations.
    - Drush Rush will default to running rush files with the default names of 'rush.ini' or 'rush.php'.
    - You may specify a different rush file to run at runtime by adding the `--rush=<rush_file>` parameter to your drush rush command.
    
Example files: @todo

## Rush Application Folder {#sec-builders-application-folder}

*Note:* **The following is an extended version of the Rush Application Folder section for Rush users. This version has more information specifically for Rush Job builders. **

- Rush (`~/Rush/`)
    - The Drush Rush application folder in your HOME directory, which contains an `environment` directory and a `jobs` directory.
    - This directory is for user environment settings, default settings for all jobs, and default make files to be used by Drush Make.

- environment (`~/Rush/environment`)
    - Contains one or more params files and may also contain a global make file.
    - Put parameters to use as defaults for most jobs.  Then override these parameters at the job level if needed.

- jobs (`~/Rush/jobs`)
    - Contains job folders.

- a job folder (`~/Rush/jobs/jobName`)
    - Contains a rush file with operations and typically a params file for the specific job.
    - A job folder may also contain a default.make file.
    - A job folder may also contain a sub job folder.

- a sub job folder (`~/Rush/jobs/jobName/subJobName`)
    - A sub job folder will inherit the parameters of its parent job.
    - A sub job may override a parameter from its parent job.
    - A sub job without a rush file will run the rush operations from its parent, while using its own parameters.
    - By design, a sub job with its own rush operations will still inherit parameters from its parent, but not the rush operations from its parent.
    - Instead, the sub job rush operations file may call the parent job operations to run at any point within its own operations, by using the command `[j] = parentJobName`


## Rush Job Commands and Rush Operations {#sec-commands-operations}

There are four types of commands which a Drush Rush job can execute. These are:

1.  Messages
2.  Jobs
3.  Shell Commands
4.  Operations

The following illustrates example syntax for each command type within a `rush.ini` file:

### Messages {#sec-commands-operations-messages}

The command below will echo 'Hello World' to the terminal.

    [m] = 'Hello World'

### Jobs {#sec-commands-operations-jobs}

The command below will run another Rush Job titled, 'beta_site'.

    [j] = beta_site
    
@todo Add detail.

### Shell Commands {#sec-commands-operations-shell}

 The command below will make a directory called Rush within the working build directory.

     [c] = 'mkdir Rush'

 The command below will change to the `~/Sites` directory and then make a directory named 'logs'.

     [c] = 'cd ~/Sites; mkdir logs'

 The command below will be recognized as a Drush command and will be executed within the site webroot with the drush `--uri` parameter correctly added to the Drush command.

     [c] = 'drush cc all'

### Operations {#sec-commands-operations-ops}

Operations carry out actions such as to download files, create databases, commit code, install Drupal sites, and run other Drush commands.

The command below will make a build directory at the location specified in parameter `$params['build']['directory_path']` .
If no location is specified the build directory location will be derived from the job name and the workspace directory.

    [f] = create_build_directory

@see rush_op_create_build_directory

The command below will create a dns record for the build.

    [f] = create_dns

@see rush_op_create_dns

The command below will run `git init` within the build directory.

    [f] = git_init

@see rush_op_git_init

#### More About Operations {#sec-commands-operations-ops-more}

Operations map to functions defined in the `operations` directory of the Drush Rush codebase.

Groups of operation functions include:

- **File** system functions for file and directory management.
- **Host** functions for managing dns and vhost entries as well as Apache.
- **SQL** functions for database management.
- **Site** functions for creating, deleting, and managing Drupal site installs.
- **Repo** functions to manage version control for code.

@see operations
