
Hide Filepath module
-----------------
by Erik Newby (eriknewby)
Contributions by Travis McTighe (tmctighe)

  This module provides a way to hide absolute paths to files from the
  page source, even if your Drupal instance is using the Public download method.
  For example, when an anonymous user clicks on a file path altered
  by Hide Filepah, they're asked to login first and then redirected to the file.
  It especially comes in handy if you want users to be prompted to login first
  before viewing or downloading files.

  It is important to note that this is *not* a security module. It does not
  actually remove accessibility to the real path. If a user hits a real path
  he/she will receive the file as expected. This module is going off a
  "security through obscurity" model, and therefor is by no means bullet
  proof. It does make it more difficult for users to determine file paths.
  That is the module's intent.

  Note: This module currently only works for files attached to Node entities.


 Basic steps to install and configure:
---------------------------------------

  * Drop the hide_filepath module into your preferred module
    directory, and enable.

  * When creating a file field on a content type, enable the "Hide Filepath" 
    checkbox when editing the file field properties.

  * Go to /admin/config/media/hide_filepath/settings to set up what users
    should receive when clicking a file path.

  * An overview and demo can be viewed at https://vimeo.com/63399648



--------------------------------------------------------------------------------
                Using the "Custom Hook" option under configuration
--------------------------------------------------------------------------------

    If you are a module developer, you can hook into this module and run your
    own logic if you choose "custom hook" on the configuration page.

    The available hooks are:

    * hook_hide_filepath_anonymous_user()

      - Do custom stuff if the user attempting to view/download the file is
        anonmymous.

    * hook_hide_filepath_insufficient_access()

      - Do custom stuff if the user attempting to view/download the file does
        not have privileges to do so.
