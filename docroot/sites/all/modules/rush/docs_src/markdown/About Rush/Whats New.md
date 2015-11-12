What's New  {#doc-whats-new}
===============

[TOC]

@brief Notes to help you find what out what's new with Drush Rush.

What's New  {#page-whats-new}
====================

## Oct 6, 2013 - Environment Based Global Pre and Post Operations {#sec-10-6-2013}

Suppose you want to automatically install the Drupal Coder module on every Drush Rush built site. Simply add a new file called `post.rush.ini` to your `environment` folder. Then add rush commands to it like any other rush file.  In this case add:

    [c] = 'drush dl coder; drush en -y coder' 

From now on, **every** rush job that runs on your local system will have a last command in it to download and enable the coder module. 

##  Sep 23, 2013 - New Branching Options from the Command Line {#sec-09-06-2013}

We have three new options which can be used when calling the `drush rush` command.

Let's start with the difference between the last option and the other two.

### `--create-branch`

The `--create-branch` is the only drush rush option which will pass a parameter 
from the command line **AND** call a rush operation.  

Passing `--create-branch`will call the drush operation `git_create_branch` at the
 **end** of the run of a rush job.  

Here are a few points about using this parameter at runtime:

* `git_create_branch` basically runs `git checkout -b`
*  it does not depend on the `create_branch` operation being present in the job 
rush.ini file
* whether the parameter is present in the params.ini file or not, the `create_branch` 
operation will always run **after** the full rush job has completed

This command parameter is the most useful and truly new option.

### `--branch`

The `--branch` parameter is used with the `git_clone` operation.  Using this 
parameter `--branch=otherbranch` from the command line will tell `git_clone` to 
clone the branch named 'otherbranch' rather than what is specified in the 
params.ini file. Thus `--branch` at the command line is simply an override for 
the `[repo][branch]` parameter set in a rush job's params.ini file.  

The following are a few scenarios which can occur with this param and operation:

1. No `[repo][branch]` parameter has been set in the params.ini file = 
The `git_clone` operation will clone the `master` branch when the operation is 
run in a rush.ini file.
2. No `[repo][branch]` parameter has been set in the params.ini file AND `--branch` 
has been set to 'otherbranch' at runtime = The `git_clone` operation will clone 
the already existing branch named 'otherbranch' when the operation is run in a 
rush.ini file.
3. The parameter `[repo][branch]` has been set in the params.ini file to 'develop' 
= The `git_clone` operation will clone the `develop` branch when the operation 
is run in a rush.ini file.
4. The parameter `[repo][branch]` has been set in the params.ini file to 
'develop' AND `--branch` has been set to 'master' at runtime = The `git_clone` 
operation will clone the `master` branch when the operation is run in a rush.ini file.

### `--new-branch`

The `--new-branch` will likely become less useful now that we can create a new 
branch from the command line call to the job.  Before, this parameter was used 
with the rush operations `git_branch` and `git checkout`.  

Unlike the `--create-branch` parameter, this parameter at runtime only sets or 
overrides a parameter normally set in params.ini. The new parameter will be used, 
when one or both rush operations  `git_branch` and `git checkout` are called in 
the rush.ini file.
