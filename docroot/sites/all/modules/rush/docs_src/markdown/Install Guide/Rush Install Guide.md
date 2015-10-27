Rush Install Guide  {#doc-install-guide}
===============

[TOC]

@brief How to install the Drush Rush module, how to setup your Rush Application folder, and how to create a parameters file for your environment.

Rush Install Guide  {#page-install-guide}
====================


## Setup {#sec-setup}

**Note:** Setup will discuss two directories:

1.  The `rush` module directory
2.  The `Rush` application folder (directory) within your HOME directory

Note the lowercase and the uppercase titles for the directories.

The `rush` module directory contains the rush module code, like any other Drush module.
The `Rush` application folder will contain your configurations, and Rush jobs.

### 1. Install Drush & the Drush Rush Module {#sec-install}

Drush Rush depends on drush (the DRUpal SHell).

To use Drush Rush, you will need to install Drush.

* See the install instructions on the [Drush Project Page](https://github.com/drush-ops/drush)

The Drush Rush module may by installed in the `.drush` folder within your HOME folder.

Note, if the folder does not already exist, you will have to make the `.drush` folder yourself.

After install the Rush README file would be located here:

    ~/.drush/rush/README.md

The following commands will clone the Drush Rush module into your `.drush` folder.

    cd ~
    git clone git@github.com:palantirnet/rush.git .drush/rush

### 2. Create a Drush Rush Application Directory {#sec-create-application-directory}

You will need a Rush application folder within your HOME directory.  Drush Rush can do this for you.

Open a command line and run:

    drush rush init

After the command runs, you should have a folder named Rush in your HOME directory, with the following directory structure:

    ~/Rush/
    ~/Rush/environment
    ~/Rush/environment/params.ini
    ~/Rush/jobs

### 3.  Set Your Environment Parameters {#sec-set-parameters}

Within your new params file `~/Rush/environment/params.ini`, add parameters which define your personal development environment.

Drush Rush will derive many of your environment parameters for you.

#### Rush Environment Params Examples

See @ref doc-env-params-examples

## Additional Setup for Host Operations {#sec-host-setup}

###  Configure Virtual Hosts {#sec-virtual-hosts}

If you are not using a Debian based operating system such as Ubuntu, then you'll need to configure Apache to use a "sites-available" directory for your vhost entries.

#### 1. Locate where Apache runs on your system and the location of your Apache config file.

- Typically the Apache config file may be located here: `/etc/apache2/httpd.conf`.

- MAMP users should find their Apache config file located here: `/Applications/MAMP/conf/apache/httpd.conf`

#### 2. Now that you have the location to Apache and its config file, create a `sites-available` directory on your system in a place where Apache will have permission to read.

- If you found your Apache location here: `/etc/apache2/` then a good place to create your `sites-available` directory is here: `/etc/apache2/sites-available/`

        sudo mkdir /etc/apache2/sites-available

- For MAMP users, a good place to create your `sites-available` directory is here: `/Applications/MAMP/conf/apache/sites-available/`

        mkdir /Applications/MAMP/conf/apache/sites-available

#### 3. Finally, add the `NameVirtualHost` directive and your `sites-available` directory path to the bottom of your Apache config file.

- The default http port for Apache to listen to is port: 80.

- MAMP uses port 8888 by default but gives you the option to change it to port 80.

Here's an entry example for a typical Apache setup:

    NameVirtualHost *:80
    Include /etc/apache2/sites-available/

If this matches your setup, then add this entry to the bottom of you Apache config.

If you are using MAMP with the port of 8888, then add the following instead:

    NameVirtualHost *:8888
    Include /Applications/MAMP/conf/apache/sites-available/

If you are using MAMP, but have set the default port to 80, then change the `8888` above to `80` .

Restart Apache to load your new configurations.

    sudo apachectl restart

MAMP users run the following:

    /Applications/MAMP/Library/bin/apachectl restart



