# Project Dependency Generation Information

## Terms
* *Component*: A module in a package. Many packages contain more than one module.
* *Release*: A node that signifies a release package.

## Processing technique

Before we can do recursive dependency checking, we have to have a complete
database of related components.

1. Set the directory where git repositories should be stored. This is a semi-permanent directory, because it saves us the trouble of checking things out anytime we're processing dependencies. For example:
<code>
drush vset project_dependency_sourcecode_directory /home/rfay/tmp/gitrepos
</code>
or set it at admin/project/project-release-settings
2. Pre-seed a set of git repositories. Although the system can check out each project (and will do so) it's one more level of pain, so it's better to use the techniques in http://drupal.org/node/1057386 to pre-build all the repositories. Note that this takes about 4-5GB of disk space, which should be kept around. You do *not* have to do this step, it just speeds things up if you're going to check dependencies on the entire set of repositories.
3. Create all first-level dependencies. This walks the info files for each release node and finds its explicit dependencies and creates rows in the component and dependency tables. Uses drush command <code>drush pdpp <project></code> or the related drush commands described below.
4. Create all recursive dependencies. Here we have to have #2 already in place, and we check each project for external dependencies, and where they exist, create new dependency rows for the top-level component.

## Key functions

* *project_dependency_get_external_component_dependencies($release_node, $project_node, $depending_components, $dependency_releases)* searches for dependency information at the for a given release.
* *project_dependency_process_release($shortname, $release_node)* is called to process a new release node.
* *project_dependency_get_external_release_dependencies($depending_release_nid, $depending_components = array())* returns an array of release dependency information, given a release nid and optional component names. This will probably be the most common function to be called.


## How we choose dependent packages

* Get all release packages that contain a given component with a given major version tid.
* The first one will be a dev release, if there is a dev release.
* If there is a dev release and there are stable releases, take the first stable, otherwise use the dev.

## Drush commands

* drush project-dependency-process-project (pdpp): Process and store all dependency information for a project or projects, as in <code>drush pdpp views ctools commerce</code>
* drush project-dependency-process-version (pdpv): Process and store all dependency information for a single release node, as in <code>drush pdpv views 7.x-3.0-beta1</code>
* drush project-dependency-show-dependencies (pdsd): Given a project and version, shows the external packages (releases) that depend on this project and version. <code>drush pdsd views 7.x-3.0-beta1</code>.

## Database Tables and Modifications

There are two tables introduced  with this module.

* project_dependency_component: One row for every component of every release.
* project_dependency_dependency: One row for every component which is a dependency of another.

(Note that these obsolete the project_info_* tables that are currently deployed on drupal.org.)

## Changes from earlier approaches

* There is now only one round of dependency generation (where each project's direct dependencies are calculated). Everything else can be determined from that.
* There is (almost) no web UI for this at this time. Since project dependencies need only be calculated on a project node creation, there's really no need. Initial work (and fixups, or paranoid debugging) is available with the drush commands.

## Open Issues

* The info file feature 'test_dependencies' is not currently being respected. We can either test this now, or go with what we have at this point and add that later. It is a very attractive feature.

## TODO
* Tests for the new features.
* Detect creation of a new release node and react to it.
* project_dependency_get_external_component_dependencies() may need to be cached.
