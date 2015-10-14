[1]Include

   Installs and manages files and packages that need to reside on the
   [2]PHP [3]include_path.

   Any file not found on the default [4]include_path is added to the local
   repository, which is then added to the [5]include_path on every page
   load. If an error occurs, the original [6]include_path is restored and
   an error flag is set to prevent further damage.

   This very simple module contains only one externally useful function:

   include_check_path($path, $source = NULL, $type = 'file')
          Verifies or installs a file or directory into the include
          repository.

        Parameters
                $path The target path to install, relative to the include
                file root. If $path is empty or ends in a trailing slash,
                it is interpreted as a directory name, and both $source
                and $type are ignored.

                $source (optional) The file data, or a uri where it may be
                found. If unset or empty, then $path is a directory name,
                and $type is ignored.

                $type (optional) A string which determines how $source is
                to be interpreted. Must be one of the following:

               o 'dir' : $path is a directory name, and $source is
                 ignored.
               o 'file' : (Default) drupal_realpath($source) is a local
                 file.
               o 'url' : $source is a string containing the file data.

        Return value
                TRUE if the file was found or installed; otherwise FALSE.

References

   1. http://drupal.org/project/include
   2. http://php.net/
   3. http://php.net/manual/ini.core.php#ini.include-path
   4. http://php.net/manual/ini.core.php#ini.include-path
   5. http://php.net/manual/ini.core.php#ini.include-path
   6. http://php.net/manual/ini.core.php#ini.include-path
