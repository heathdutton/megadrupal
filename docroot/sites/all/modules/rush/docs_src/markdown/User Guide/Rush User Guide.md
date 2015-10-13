Rush User Guide  {#doc-user-guide}
===============

[TOC]

@brief How to use Drush Rush.

Rush User Guide  {#page-user-guide}
====================


## Jobs and Builds {#sec-jobs-builds}

The two main components of Drush Rush are **jobs** and **builds**.

Jobs define what to produce and builds are what is produced.

Jobs define parameters and operations used to execute a build (typically installing a Drupal website).

Conversely, builds are the product of the execution of a job.

Jobs, once written rarely change.  Builds are created and deleted often.

Developers share job folders to give others the ability to create the build defined by the job.

The name of the Rush job folder is also the name of the job.  A Rush job called, 'greatsite' will be in a folder named, 'greatsite'.

To run the Rush job 'greatesite', use the following command:

    drush rush greatsite

To recap:

- Jobs
    - A package of parameters and operations, which when run via the drush rush command will produce a build.

- Builds
    - The product of a job; typically a Drupal website, though a build may be something as simple as adding a domain to a hosts file.


## The Rush Application Folder {#sec-application-folder}

After you [install Rush](@ref doc-install-guide) you will have a Rush application folder on your system.

Your Rush jobs go in this folder.
Your environment files also go in this folder to tell Rush about your local system and your global preferences.

Below is an outline of your typical Rush application directory structure:

- Rush (`~/Rush/`)
    - The Drush Rush application folder in your HOME directory, which contains an `environment` directory and a `jobs` directory.

- environment (`~/Rush/environment`)
    - Contains one or more params files and may also contain a global make file.
    - Put parameters in this directory to use as defaults for most jobs.  Then override these parameters at the job level if needed.

- jobs (`~/Rush/jobs`)
    - Contains job folders. Put your job folders in here.

- a job folder (`~/Rush/jobs/jobName`)
    - Contains a rush file with operations and typically a params file for the specific job.
    - A job folder may also contain a default.make file.
    - A job folder may also contain a sub job folder.

- a sub job folder (`~/Rush/jobs/jobName/subJobName`)


## Drush Rush Commands {#sec-commands}

There are two main commands for Drush Rush:

1. `rush`
    - Run a rush file with rush operations in it.
        - Note: run `drush rush -h` for help documentation of this command.
2. `rush-list`
    - List rush jobs in the jobs directory.
        - Note: run `drush rush-list -h` for help documentation of this command.

### Rush Command Line Options {#sec-command-options}


      'show'   => array(
        'description'   => 'Show mode. Utility to inspect operations or params without actually running the job.',
        'example-value' => 'ops,params, See drush rush-show -h for more examples.',
      ),
      'domain' => array(
        'description'   => 'Domain to use for jobs that use the [build][domain] parameter.',
        'example-value' => 'mydomain.local',
      ),
      'rush'   => array(
        'description'   => 'Rush prefix to select a different rush.ini or rush.php to run.',
        'example-value' => 'un,pre-build,post-build',
      ),
      'branch'   => array(
        'description'   => 'Branch to use for jobs that use the [repo][branch] parameter.  This will affect jobs that run the rush clone operation.  Passing this parameter at runtime is a good way to clone a different git branch to run the rush job against.',
        'example-value' => 'develop,master,sprint1,new-feature',
      ),
      'new-branch'   => array(
        'description'   => 'New branch name to use for jobs that use the [repo][new_branch] parameter.  This will affect jobs that run the rush branch operation.  Passing this parameter at runtime is a good way to create new feature branches that start from the branch defined in the rush job.',
        'example-value' => 'new~feature,experiment',
      ),
      'create-branch'   => array(
        'description'   => 'Create a new branch with the specified name, after all rush operations have run.  This option runs `git branch` after the rush job.  It differs from the `new-branch` option in that this option runs at the end of the job and does not need the rush branch operation to be present in the rush file.',
        'example-value' => 'new~feature,experiment',
      ),

### Rush Commands Examples {#sec-command-examples}

See @ref doc-commands-examples

## Job Templates {#sec-job-templates}

There are a number of example jobs in the  [RushTemplates](https://github.com/seanbuscay/rush/tree/master/RushTemplates) directory.

You can use them from the RushTemplates directory or copy them into your own jobs directory and customize them.

To see a list of the jobs in the job templates folder run:

    drush rush-list --show=jt

To see a see a description of the job with information on how it works run:

    drush rush <job-name> --show=help

For example, to see how tests.filesystem works run:

    drush rush tests.filesystem --show=help
