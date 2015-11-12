This is a drush extension that can help keeping make files updated, which 
normally is a manual and time consuming process. With this extension you simply
ask drush to do all the work.

# Installation
Simply clone the repository into your .drush folder and with any other drush 
extension.

## Usage
The extension comes with two new commands makefile-check and makefile-update,
where the first only checks the makefile for updates and write version
suggestions on the screen.

The later writes the updates into an output file (update make file).

<code>
~$ drush makefile-check <MAKEFILE>

~$ drush makefile-update <IN MAKEFILE> <OUT MAKEFILE>
</code>

### Options
 * --no-cache: The commands utilize cache this option by-passes the cache.
 * --exclude: This options takes a comma delimited list of projects that should
              not be updated in the make file.</dd>

__Note__: This extension will only work with the 5.x version of drush.