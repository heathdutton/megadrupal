Parameters List  {#doc-parameters-list}
===============

@brief A list of parameters which Rush may use.

Parameters List   {#page-parameters-list}
====================

[TOC]

## Introduction  {#sec-parameters-list-intro}

Parameters provide variables for a build, such as site name, repo information,
database connection strings, and really anything needed to complete a build.

Any parameter may be overwritten.

Parameters may be set but not used by every job.

### How and Where to Define Parameters  {#sec-parameters-how-where}

Note: see inheritance.

Tip: How to see your params.

## Parameter Groups  {#sec-parameters-groups}

Drush parameters are organized into the following groups:

- job
- build
- db
- env
- global_locations
- repo
- si
- derived

## job Parameters  {#sec-job-parameters}
## build Parameters  {#sec-build-parameters}

Build parameters should be set in the params file within each Rush
job folder.  However, it may be useful to also set some Build parameters at
the Environment level.  For example, Rush defaults `docroot_name` to
'docroot', but all your builds may use 'www' as their docroot.  You can then
set `docroot_name` at the Environment level to `www` so the default will now
'www'.

@param [directory_list] => Array
(
[0] => patches
[1] => misc/logs
)
<div>An array of directory names to create in the build directory.
Defined like so:
</div>
@code
directory_list[] = patches
directory_list[] = misc/logs
@endcode
@param [file_list] => Array
(
[0] => README.md
[1] => misc/README.md
)
<div>An array of file names to create in the build directory.
Defined like so:
@code
file_list[] = README.md
file_list[] = misc/README.md
@endcode
</div>
@param [gitignore_list] => Array
(
[0] => cache/
[1] => files/
)
<div>An array of .gitignore entries to add to a .gitignore file in the build directory.
Defined like so:
@code
gitignore_list[] = cache/
gitignore_list[] = files/
@endcode
</div>
@param [job] => Test.params
<div>The name of the job.  This parameter is just informational.
A period in the name indicates the job `params` is a sub-job of `Test`.
Derived: From the name of the job directory.
</div>
@param [key] => 08c17
<div>A unique id for the build.  Used in test jobs.  May be also be used for
other purposes such as to append to directory names to keep them unique or
in logging.  Derived: Auto generated.
</div>
@param [domain] => params.dev
<div>The domain to use for the build if the job builds a site.
Derived: From a combination of the job name and the env tld parameter.
</div>
@param [machine_name] => params_dev
<div>A machine name for the build.  The machine name has no whitespace and no
periods in it.  The machine name is typically the basis for other parameters
such as the db name, if none was provided.
Derived: From the domain name with spaces and periods removed.
</div>
@param [site_uri] => http://params.dev
<div>Either a uniform resource locator (URL), or a uniform resource name (URN),
or both.  Usually just the url to the build site.
Derived: From `http://` plus the domain name.  If the host post is not set to
the default of `80`, then the host port will be added.  This is mostly used for
MAMP users with a non-standard port setup.  So for example, if the port is set
to `8888` and the domain is `params.dev` then the URI will become
`http://params.dev:8888`
</div>
@param [root_directory_name] => Sites
<div>
The name of webroot directory to use for the build.
Example settings include: 'www', 'websites', and 'Sites'.
Derived: 'Sites' as a default; which is kind of OSX-centric.
</div>
@param [root_directory_path] => /Users/username/Sites/
<div>
Most often set to the path to the webroot directory on the server. Usually it's
something like `~/webroot` or `~/Sites`.  NOTE: it does not have to be set to
the webroot directory.  A particular Drush Rush job may just download a bunch of
code libraries to a directory somewhere, so the path is often set globally but
may be overwritten at the job level.  Derived: From the Rush Server Home plus
the root directory name plus a forward slash.
@code
$params['build']['root_directory_path'] = $params['global_locations']['RUSH_SERVER_HOME'] . $params['build']['root_directory_name'] . '/';
@endcode
</div>
@param [logs_path] => /Users/username/Sites/logs/
<div>
If Rush detects you have a directory named `logs` within your root_directory_path
then Rush will add an `ErrorLog` entry in your vhost file pointing to the directory.
You may also set `logs_path` to another directory of your choice.
You can set it at the global level in your environment folder or at the job level.
</div>
@param [directory_path] => /Users/username/Sites/params_dev/
<div>
The build_directory_path is the sum of the build_root_directory_path and the
build_machine_name.  It's automatically derived.  However, setting this value in
a job params file will override the path.  In practice, this would be the only
value needed to be overwritten, rather than the build_root_directory_path and
the build_machine_name.  Derived: From the root directory path and the machine name
plus a forward slash.
@code
$params['build']['directory_path'] = $params['build']['root_directory_path'] . $params['build']['machine_name'] . '/';
@endcode
</div>
@param [docroot_name] => docroot
<div>
A Drupal project may contain directories above its html / docroot.  Files meant
to be accessed sub-directory of the overall project. Drush Rush must know where the core
Drupal code is.  Example values include: 'htdocs', 'www', and 'docroot'.
Drush Rush defaults to 'docroot'.
</div>
@param [docroot_path] => /Users/username/Sites/params_dev/docroot/
<div>
Used by Drush and Apache Vhosts.  Path to Drupal core code base.  Derived: From
directory path and docroot name plus a forward slash.
@code
$params['build']['docroot_path'] = $params['build']['directory_path'] . $params['build']['docroot_name'] . '/';
@endcode
</div>
@param [code_directory_path] => /Users/username/Sites/params_dev/docroot/
<div>
Typically an alias for the docroot_path.  However, code_directory_path can be
changed if the codebase is located somewhere else.
</div>
@param [site_folder_name] => default
<div>
The name of the Drupal site folder, inside the sites directory.  Defaults to: `default`.
</div>
@param [site_specification] => /Users/username/Sites/params_dev/docroot/?uri=http://params.dev#default
<div>
A Drush site specification.  Derived: From docroot path, plus uri, plus # sign
and the site folder.
</div>
@param [site_alias_record] => Array
<div>
A Drush site alias record.  Derived: Provided by Drush.
@code
Array = (
  [uri] => http://params.dev
  [root] => /Users/buscay/Sites/params_dev/docroot/
  [path-aliases] => Array
    (
      [%root] => /Users/buscay/Sites/params_dev/docroot/
    )
  [#name] => .Users.buscay.Sites.params_dev.docroot.?uri=params.dev#default
  )
@endcode
</div>
@param [site_profile] => standard
<div>
Site profile used by Drush Site Install.
</div>
@param [make_file_path] => /Users/username/Rush/environment/default.make
<div>
Rush will look for a default.make file in the following order:
1. Within the Job directory
2. Within the Top (Parent) Job directory
3. Within the `environment` directory
You can also define a different make file path. Such as:
`/Documents/makefiles/workbench.make`
</div>
@param [file_list_data] => File added by Drush Rush.
<div>
String containing a text blurb placeholder to insert inside files created by Rush.
</div>

## db Parameters  {#sec-db-parameters}

Database parameters may be set in the params file within the Rush
application folder `~/Rush/environment/`.  The DB parameters may also be set
at the Job and SubJob directory level.

@param [query] => UPDATE users SET mail = 'admin@example2.com' WHERE users.uid = 1
<div>
A string containing a query to run on the database with the `sql_query` operation.
</div>
@param [type] => mysql
<div>
The type of database server.
Options include: `mysql` , `sqlite` , and `postgres` .
Currently only mysql has been tested.
The database type is used to create a db connection url.
Defaults to: `mysql`.
Meant to be set in the `environment` directory and overwritten at the job level when required.
</div>
@param [server] => localhost
<div>
The db host server identifies the server address to connect to.
It identifies either a hostname, IP address, or UNIX domain socket.
Examples include: `localhost` and `127.0.0.1` .
The default is `localhost`.
Meant to be set in the `environment` directory and overwritten at the job level when required.
</div>
@param [port] => 3306
<div>
The database port is the port on which the database server is listening for
connections.  The default mysql port is `3306`. MAMP users may use a different
port, such as `8889` .
Meant to be set in the `environment` directory and overwritten at the job level when required.
</div>
@param [user] => root
<div>
The database user name and password are used to create a db connection url.
All db values may be changed at the Job level for jobs which need to use
different databases and database connection information.
Both default to `root`.  Please note: Putting user names and passwords in text
files has security risks. Rush should only be used on local secure systems
behind a safe firewall.
Meant to be set in the `environment` directory and overwritten at the job level when required.
</div>
@param  [password] => root
<div>
The database user name and password are used to create a db connection url.
All db values may be changed at the Job level for jobs which need to use
different databases and database connection information.
Both default to `root`.  Please note: Putting user names and passwords in text
files has security risks. Rush should only be used on local secure systems
behind a safe firewall.
Meant to be set in the `environment` directory and overwritten at the job level when required.
</div>
@param [name] => params_dev
<div>
The name of the database to use for the job.
The db name will usually be set in the project params file.
If it's not set, then it will default to the job machine name, which in turn
may be derived from the job domain.
</div>
@param [url] => mysql://root:root@localhost:3306/params_dev
<div>
A database connection URL used to connect to the build database.
The db url is built for you based off the following other db params:
  - type
  - user
  - password
  - port
  - server
  - name
  - file_path (for sql lite)
<div>
You can set the db url in your job params to bypass the derived version and to
skip the need to set other db params.  This is helpful if your build should go
to a different db connection than what's default on your system.
@code
MySQL
url = mysql://root:pass@localhost:port/dbname

SQLite
url = sqlite://sites/example.com/files/.ht.sqlite
@endcode
</div>
</div>
@param [file_path] => /Users/buscay/Sites/params_dev/sql/params_dev.sql
<div>
The `directory_name`, `directory_path`, `file_ext`, `file_name`, `file_path`,
db parameters  are used to backup and import an sql file into and out of the build.
Each parameter builds upon the previous parameter settings to complete the
`file_path` parameter.  If you need to point to point to a db file other than
what is automatically derived by Rush, then you can change one of these
parameters to get what you need.
@code
[directory_name] => sql
[directory_path] => /Users/buscay/Sites/params_dev/sql/
[file_ext] => sql
[file_name] => params_dev.sql
@endcode
<div>
Here are some examples:
- The default db `directory_name` for builds is `sql` , but all your builds
  store the db file in a build directory called, `db_snaps`.  Simply change the
  `directory_name` name parameter at the environment level to `db_snaps` to
  have all jobs export db snapshots and import dbs from a directory called,
  `db_snaps`.
- The default db `file_ext` is `sql` , your you have one job which requires
  the extension to be `mysql` .  Change the `file_ext` parameter in your job
  params file to `mysql` .
- For a particular job build, you need to import the staging database rather
  than the default database.  The staging database is called, 'staging.mysql'.
  Change the `file_name` in the job params file to `staging.mysql` .
- All default parameters will point your job to build a site using a db
  snapshot located here: `/Sites/mysite_dev/sql/db.sql` but you want the job
  to build using a db located here: `/Desktop/migrate.mysql`.  Just change the
  `file_path` parameter to `/Desktop/migrate.mysql` and this will supersede
  all other db location and name parameters.
</div>
</div>

## env Parameters  {#sec-env-parameters}
## global_locations Parameters  {#sec-global_locations-parameters}
## repo Parameters  {#sec-repo-parameters}
## si Parameters  {#sec-si-parameters}
