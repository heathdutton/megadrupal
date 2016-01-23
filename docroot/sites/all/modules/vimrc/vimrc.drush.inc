<?php

/**
 * @file
 *   Drush tools to support the vimrc project.
 */

/**
 * Implements hook_drush_command().
 */
function vimrc_drush_command() {
  $items = array();

  $items['vimrc-install'] = array(
    'description' => 'Install the vim plugin for Drupal (vimrc project).',
    'arguments' => array(
    ),
    'options' => array(
      'vim-command' => 'Command used to invoke vim (defaults to vim).',
      'vimfiles' => 'Path to vimfiles directory (defaults to $HOME/.vim, or $HOME/vimfiles on Windows).',
      'vimrc' => 'Path to vimrc file (defaults to $HOME/.vimrc, or $HOME/_vimrc on Windows).',
    ),
    'examples' => array(
      'drush vimrc-install --vim-command=/home/drupaluser/bin/vim'
      => 'Install the plugin using the executable at /home/drupaluser/bin/vim.',
      'sudo drush vimrc-install --vimfiles=/usr/share/vim/vimcurrent --vimrc=/usr/share/vim/vimrc'
      => 'Install the plugin system-wide. The example paths are suitable for some Linux distributions.',
    ),
    'aliases' => array(),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['vimrc-tag-gen'] = array(
    'description' => 'Generate a tags file for use by vim and other programs.',
    'arguments' => array(
      'path' => 'System path to a file or directory (e.g. your Drupal distribution or a module folder). Defaults to the current directory.',
    ),
    'options' => array(
      'tag-file' => 'Name of the output file (defaults to "tags")',
      'ctags' => 'Path to the exuberant ctags executable (defaults to "ctags")',
      'include-js' => 'Include tags for .js files (defaults to "no")',
      'include-test' => 'Include tags for .test files (defaults to "no")',
      'make-portable' => 'Replace path with \'$DRUPAL_ROOT\' (defaults to "no")',
      'ctags-options' => 'Extra options to pass to ctags (defaults to "")',
    ),
    'examples' => array(
      'drush vimrc-tag-gen $HOME/Sites/drupal-7 --tag-file=drupal7.tags'
      => 'Generate a tags file for your distribution of Drupal 7 and save it as drupal7.tags in the current directory.',
      "drush tags sites/all/modules/ctools --ctags=/usr/local/bin/ctags --ctags-options='--fields=+aimS'"
      => 'Generate a tags file for the ctools module using a specific ctags executable and options and save it as tags in the current directory.',
    ),
    'aliases' => array('tags'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * Implements drush_hook_COMMAND_validate().
 */
function drush_vimrc_install_validate() {
  // Bail out if vim is not executable or is not the right version.
  $vim = drush_get_option('vim-command', 'vim');
  $error = '';
  // This works only if $vim is a full path.
  // if (!is_executable($vim)) {
  //   $error = dt('The command %vim is not executable.', array('%vim' => $vim));
  // }
  // else {
  $success = drush_shell_exec('%s --version', $vim);
  $output = drush_shell_exec_output();
  if (!$success) {
    $error = dt('The command %vim exited with an error.',
      array('%vim' => $vim));
  }
  elseif (!preg_match('/^VIM - Vi IMproved (\d+)/', $output[0], $matches)) {
    $error = dt('The command %vim is not Vi IMproved (vim).',
      array('%vim' => $vim));
  }
  elseif ((int) $matches[1] < 7) {
    $error = dt('The vimrc project requires vim 7.0 or later (7.3+ recommended).',
      array('%vim' => $vim));
  }
  // }
  if ($error) {
    $error .= "\n" . dt('See !url for how to install vim.',
      array('!url' => 'http://www.vim.org/download.php'));
    return drush_set_error('DRUSH_APPLICATION_ERROR', $error);
  }

  // Check that we can write to the vimrc file. If pathogen is not installed,
  // also check the vimfiles directory.
  $vimrc = drush_get_option('vimrc', _vimrc_vimrc_default());
  // @todo Maybe this can be done with file_prepare_directory.
  if (!is_file($vimrc) && !touch($vimrc)) {
    $error = dt('Cannot create the vimrc file %vimrc. Specify another file with --vimrc.',
      array('%vimrc' => $vimrc));
  }
  elseif (!is_writable($vimrc)) {
    $error = dt('Installation requires write access to the vimrc file %vimrc. Specify another file with --vimrc.',
      array('%vimrc' => $vimrc));
  }
  elseif (!_vimrc_check_pathogen($vim)) {
    // Need to install pathogen.
    $vimfiles = drush_get_option('vimfiles', _vimrc_vimfiles_default());
    if (!is_dir($vimfiles) && !mkdir($vimfiles, 0777, TRUE)) {
      $error = dt('Cannot create the vimfiles directory %vimfiles. Specify another directory with --vimfiles.',
	array('%vimfiles' => $vimfiles));
    }
    elseif (!is_writable($vimfiles)) {
      $error = dt('Installation requires write access to the vimfiles directory %vimfiles. Specify another directory with --vimfiles.',
	array('%vimfiles' => $vimfiles));
    }
  }
  if ($error) {
    return drush_set_error('DRUSH_APPLICATION_ERROR', $error);
  }
}

/**
 * Return the default vimfiles directory, depending on the OS.
 */
function _vimrc_vimfiles_default() {
    return drush_server_home() . (drush_is_windows() ?  '/vimfiles' : '/.vim');
}

/**
 * Return the default vimrc file, depending on the OS.
 */
function _vimrc_vimrc_default() {
    return drush_server_home() . (drush_is_windows() ?  '/_vimrc' : '/.vimrc');
}

/**
 * Check whether the pathogen plugin is installed.
 *
 * @return mixed
 * - FALSE if not installed
 * - version string if it can be determined, such as '2.0'
 * - TRUE if installed but cannot determine version string
 */
function _vimrc_check_pathogen($vim) {
  $lines = vimrc_vim_command($vim, '2verbose runtime autoload/pathogen.vim', TRUE);
  if ($lines === FALSE) {
    return drush_set_error('DRUSH_APPLICATION_ERROR',
      dt('Error when calling vim to check for pathogen.'));
  }
  if (preg_match('#^finished sourcing .*autoload/pathogen.vim$#m', $lines)) {
    $commands = array(
      'runtime autoload/pathogen.vim',
      'Vedit autoload/pathogen.vim',
      '0/^" Version',
      '2verbose echo matchstr(getline("."), ' . "'Version.* \zs[0-9.]*$')",
    );
    $version_string = array_pop(vimrc_vim_commands($vim, $commands));
    return $version_string ? $version_string : TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Run a vim (ex) command and return the output.
 *
 * The calling function is reponsible for validating that $vim is executable.
 *
 * Example: $versionstring = vimrc_vim_command('/usr/local/bin/vim', 'version')
 *
 * @param String $vim
 *   The name of the vim executable.
 * @param String $command
 *   The vim command to execute.
 * @param Boolean $implode
 *   If TRUE, then return a single string instead of an array.
 *   (optional, default FALSE)
 *
 * @return Array
 *   The output of $command as an array of strings or FALSE on error.
 */
function vimrc_vim_command($vim, $command, $implode = FALSE) {
  $infile = drush_save_data_to_temp_file("$command\nqa!\n");
  $vimlog = drush_tempnam('vimrc');
  if (!drush_shell_exec('%s -e -s -N -V0%s -S %s', $vim, $vimlog, $infile)) {
    return FALSE;
  }
  $lines = file($vimlog);
  return $implode ? implode("\n", $lines) : $lines;
}

/**
 * Run several vim (ex) commands and return the output.
 *
 * The calling function is reponsible for validating that $vim is executable.
 *
 * @param String $vim
 *   The name of the vim executable.
 * @param Array $commands
 *   The vim commands to execute.
 * @param Boolean $implode
 *   If TRUE, then return a single string instead of an array.
 *   (optional, default FALSE)
 *
 * @return Array
 *   The output of $command as an array of strings or FALSE on error.
 */
function vimrc_vim_commands($vim, $commands, $implode = FALSE) {
  $command = implode("\n", $commands);
  return vimrc_vim_command($vim, $command, $implode);
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_vimrc_install($path = '.') {
  $vim = drush_get_option('vim-command', 'vim');
  $vimrc = drush_get_option('vimrc', _vimrc_vimrc_default());
  $vimfiles = drush_get_option('vimfiles', _vimrc_vimfiles_default());

  if (_vimrc_check_pathogen($vim)) {
    drush_log(dt('Pathogen plugin already installed.'));
  }
  else {
    // Pathogen is not installed, but the validate function already checked
    // that we can write to the vimfiles directory. Start by downloading
    // pathogen.  See https://github.com/tpope/vim-pathogen.
    if (!file_exists("$vimfiles/autoload")) {
      drush_log(dt('Creating directory %autoload.',
        array('%autoload' => "$vimfiles/autoload")));
      drush_op('mkdir', "$vimfiles/autoload");
    }
    $url =
      'https://raw.github.com/tpope/vim-pathogen/master/autoload/pathogen.vim';
    drush_log(dt('Downloading pathogen from %url.', array('%url' => $url)));
    $pathogen = file_get_contents($url);
    drush_log(dt('Saving it to %pathogen.',
      array('%pathogen' => "$vimfiles/autoload/pathogen")));
    drush_op('file_put_contents', "$vimfiles/autoload/pathogen.vim", $pathogen);
  }

  // Now, add lines to the vimrc file. If the vimrc file is empty, start with
  // the standard one distributed with this script.
  // 1. Set 'nocompatible' in case this is a system-wide vimrc file.
  // 2. Tell pathogen about the files in this project.
  // 3. Tell pathogen about the plugins in $vimfiles/bundle.
  // Why do I get a message that __DIR__ is undefined?
  $DIR = dirname(__FILE__);
  $vimrcbundle = realpath("$DIR/bundle");
  $bundle = "$vimfiles/bundle";
  // Avoid E_STRICT and E_WARNING messages from date().
  date_default_timezone_set('UTC');
  $vimrclines = array(
    '" Following lines added by drush vimrc-install on ' . date('r') . '.',
    'set nocompatible',
  );

  // The current version of pathogen requires a trailing '/{}' on the directory,
  // but this silently breaks older versions. It is better to get a noisy error
  // message than to fail silently, so use the older syntax if there is any
  // doubt.
  $pathogen_version = _vimrc_check_pathogen($vim);
  if (is_string($pathogen_version) && preg_match('/(\d+)\.(\d+)/', $pathogen_version, $matches) && (10 * $matches[1] + $matches[2] > 22)) {
    array_push($vimrclines,
      "call pathogen#infect('$vimrcbundle/{}')",
      "call pathogen#infect('$bundle/{}')"
    );
  }
  else {
    array_push($vimrclines,
      "call pathogen#infect('$vimrcbundle')",
      "call pathogen#infect('$bundle')"
    );
  }

  $ft_check = vimrc_vim_commands($vim, array("source $vimrc", 'filetype'), TRUE);
  if (strpos($ft_check, 'detection:ON') === FALSE || strpos($ft_check, 'plugin:ON') === FALSE) {
    $vimrclines[] = 'filetype plugin on';
  }

  array_push($vimrclines, '" End of vimrc-install additions.', '');

  $runcommands = file_get_contents($vimrc);
  if (strpos($runcommands, "call pathogen#infect('$vimrcbundle") === FALSE) {
    if (empty($runcommands)) {
      $runcommands = file_get_contents("$DIR/examples/vimrc_standard.vim");
    }
    drush_log(dt('Rewriting vimrc file %vimrc.', array('%vimrc' => $vimrc)));
    drush_op('file_put_contents', $vimrc, implode("\n", $vimrclines) . $runcommands);
  }

  // Create a bundle directory, if needed, and download plugins.
  if (file_exists($bundle)) {
    if (is_file($bundle)) {
      drush_log(dt('Cannot install plugins: %bundle is a file.', array('%bundle' => $bundle)), 'warning');
    }
    elseif (!is_writable($bundle)) {
      drush_log(dt('Cannot install plugins: %bundle is not writable.', array('%bundle' => $bundle)), 'warning');
    }
  }
  else {
    drush_log(dt('Creating directory %bundle.', array('%bundle' => $bundle)));
    drush_op('mkdir', $bundle);
  }
  if (file_exists($bundle) && is_writable($bundle)) {
    vimrc_download_plugins($vim, $vimrc, $bundle);
  }

  // Update the help tags, using the pathogen variant of :helptags.
  drush_log(dt('Updating help tags.', array()));
  $vimrclines[] =  'call pathogen#helptags()';
  drush_op('vimrc_vim_commands', $vim, $vimrclines);

  return;
}

/**
 * Download vim plugins.
 *
 * Once Pathogen is installed, other plugins can be saved in the bundle
 * directory. The Pathogen plugin is installed by `drush vimrc-install` and
 * cannot be handled by thihs function.
 *
 * @todo For now, the function ignores the 'description' and 'git uri' keys.
 *
 * @param String $vim
 *   The name of the vim executable. The calling function is reponsible for
 *   validating that $vim is executable.
 * @param String $vimrc
 *   Path to vimrc file. It must be writable.
 * @param String $bundle
 *   Path to the bundle directory, such as ~/.vim/bundle. The calling function
 *   is responsible for making sure that this directory is writable.
 * @param Boolean $update
 *   (optional, default FALSE) If TRUE, then skip the check for whether the
 *   plugin is already installed. For now, the function does not check for other
 *   locations, it will just download to $bundle, overwriting the version there.
 * @param Array $plugins
 *   (optional) An array of associative arrays describing the plugins to
 *   download. The keys are the subdirectories of $bundle where the plugins will
 *   be downloaded. Each array should have the keys
 *   - 'name': Display name of the plugin
 *   - 'description': One-line decription of the plugin (optional)
 *   - 'tarball': URI of the tarball to download, any format recognized by
 *     drush_tarball_extract()
 *   - 'file': Name of the plugin file as listed by :scriptnames. Optional: it
 *     defaults to "$key.vim".
 *   - 'git uri': URI to use if downloading with git (optional)
 *   If $plugins is omitted or empty, then use the default list.
 *
 * @return Array
 *   An array of paths to the installed plugins, using the same keys as
 *   $plugins. If there is a problem with a plugin, then the value will be ''.
 */
function vimrc_download_plugins($vim, $vimrc, $bundle, $update = FALSE, $plugins = array()) {
  if (empty($plugins)) {
    // The install script calls this function with $plugins empty. Install the
    // following plugins. See http://drupal.org/node/1389472.
    $plugins['fugitive'] = array(
      'name' => 'Fugitive',
      'description' => 'A Git wrapper so awesome, it should be illegal.',
      'tarball' => 'https://github.com/tpope/vim-fugitive/archive/master.zip',
      'git uri' => 'https://github.com/tpope/vim-fugitive.git',
    );
    $plugins['nerdcommenter'] = array(
      'name' => 'NERDCommenter',
      'description' => 'Vim plugin for intensely orgasmic commenting.',
      'tarball' => 'https://github.com/scrooloose/nerdcommenter/archive/master.zip',
      'file' => 'NERD_commenter.vim',
      'git uri' => 'https://github.com/scrooloose/nerdcommenter.git',
    );
    $plugins['nerdtree'] = array(
      'name' => 'NERDTree',
      'description' => 'A tree explorer plugin for vim.',
      'tarball' => 'https://github.com/scrooloose/nerdtree/archive/master.zip',
      'file' => 'NERD_tree.vim',
      'git uri' => 'https://github.com/scrooloose/nerdtree.git',
    );
    $plugins['snipmate'] = array(
      'name' => 'Snipmate',
      'description' => 'SnipMate.vim aims to be a concise vim script that implements some of TextMate\'s snippets features in Vim.',
      'tarball' => 'https://github.com/msanders/snipmate.vim/archive/master.zip',
      'file' => 'snipMate.vim',
      'git uri' => 'https://github.com/msanders/snipmate.vim.git',
    );
    $plugins['syntastic'] = array(
      'name' => 'Syntastic',
      'description' => 'Syntax checking hacks for vim.',
      'tarball' => 'https://github.com/scrooloose/syntastic/archive/master.zip',
      'git uri' => 'https://github.com/scrooloose/syntastic.git',
    );
    $plugins['tagbar'] = array(
      'name' => 'Tagbar',
      'description' => 'Vim plugin that displays tags in a window, ordered by class etc.',
      'tarball' => 'https://github.com/majutsushi/tagbar/archive/master.zip',
      'git uri' => 'https://github.com/majutsushi/tagbar.git',
    );
    $plugins['twig'] = array(
      'name' => 'Twig',
      'description' => 'Vim plugin that adds syntax highlighting, snippets, and settings for twig files.',
      'tarball' => 'https://github.com/evidens/vim-twig/archive/master.zip',
      'git uri' => 'https://github.com/evidens/vim-twig.git',
    );
    $plugins['vdebug'] = array(
      'name' => 'Vdebug',
      'description' => 'Vim plugin that provides an interface for Xdebug.',
      'tarball' => 'https://github.com/joonty/vdebug/archive/master.zip',
      'git uri' => 'https://github.com/joonty/vdebug.git',
    );
  }

  // Find out which plugins are installed.
  drush_log(dt('Checking installed plugins.'));
  $lines = array(
    'source ' . $vimrc,
    'runtime! plugin/**/*.vim',
    'scriptnames',
  );
  $message = vimrc_vim_commands($vim, $lines, TRUE);

  // For each plugin, check whether it is installed. If not, then download the
  // tarball and extract it to $bundle.
  $paths = array();
  foreach ($plugins as $plugin => $info) {
    $paths[$plugin] = '';
    $info += array('file' => "$plugin.vim");
    if (!$update && preg_match("#$plugin#m", $message)) {
      drush_log(dt('Plugin %plugin already installed.', array('%plugin' => $info['name'])));
      continue;
    }
    $tarball = basename($info['tarball']);
    // Bail out if the archive is in zip format and drush version < 5.5.
    // Note that drush 5.0-dev is probably more recent than 5.5.
    $ext = strtolower(pathinfo($tarball, PATHINFO_EXTENSION));
    if ($ext == 'zip' && (DRUSH_MAJOR_VERSION < 5 || preg_match('/^5.[0-4]$/', DRUSH_VERSION))) {
      drush_log(dt('Drush version 5.5+ required to expand zip archives; %plugin not installed.',
        array('%plugin' => $info['name'])
      ));
      continue;
    }
    $tempdir = drush_tempdir();
    // See http://drupal.org/node/1642444#comment-6742834.
    if (!$path = _vimrc_download_file($info['tarball'], "$tempdir/$tarball")) {
      drush_log(dt('Failed to download %plugin from @uri.', array(
        '%plugin' => $info['name'],
        '@uri' => $info['tarball'],
      )));
      continue;
    }
    if (!$file_list = drush_tarball_extract($path, $tempdir, TRUE)) {
      drush_log(dt('Failed to extract %plugin from @path to @bundle.', array(
        '%plugin' => $info['name'],
        '@path' => $path,
        '@bundle' => $bundle,
      )));
      continue;
    }
    // Cribbed from package_handler_download_project().
    $project_dir = drush_trim_path(array_shift($file_list));
    // We will get an error if "$bundle/$plugin" exists, so hope that our test
    // above gives the right answer.
    drush_op('drush_move_dir', "$tempdir/$project_dir", "$bundle/$plugin");
    $paths[$plugin] = "$bundle/$plugin";
    drush_log(dt('Installed %plugin.', array('%plugin' => $info['name'])));
  }

  return $paths;
}

/**
 * Poor substitute for _drush_download_file(), add --no-check-certificate.
 * See http://drupal.org/node/1642444#comment-6742834
 */
function _vimrc_download_file($url, $destination, $overwrite = TRUE) {
  static $use_wget;
  if ($use_wget === NULL) {
    $use_wget = drush_shell_exec('which wget');
  }

  $destination_tmp = drush_tempnam('download_file');
  if ($use_wget) {
    drush_shell_exec("wget -q --no-check-certificate --timeout=30 -O %s %s", $destination_tmp, $url);
  }
  else {
    drush_shell_exec("curl --insecure --fail -s -L --connect-timeout 30 -o %s %s", $destination_tmp, $url);
  }
  if (!drush_file_not_empty($destination_tmp) && $file = @file_get_contents($url)) {
    @file_put_contents($destination_tmp, $file);
  }
  if (!drush_file_not_empty($destination_tmp)) {
    // Download failed.
    return FALSE;
  }

  drush_move_dir($destination_tmp, $destination, $overwrite);
  return $destination;
}

/**
 * Implements drush_hook_COMMAND_validate().
 */
function drush_vimrc_tag_gen_validate() {
  // Bail out if ctags is not executable or is not the right version.
  $ctags = drush_get_option('ctags', 'ctags');
  $ctags = escapeshellcmd($ctags);
  drush_set_option('ctags', $ctags);
  $success = drush_shell_exec('%s --version', $ctags);
  $output = drush_shell_exec_output();
  if (!$success || !preg_match('/^Exuberant Ctags/', $output[0])) {
    return drush_set_error('DRUSH_APPLICATION_ERROR', dt('Make sure that Exuberant Ctags is installed (http://ctags.sourceforge.net/) and specify the path with (for example) --ctags=/usr/local/bin/ctags'));
  }
  // Check that the tags file is writable.
  $tag_file = drush_get_option('tag-file', 'tags');
  touch($tag_file);
  $tag_file = realpath($tag_file);
  drush_set_option('tag-file', $tag_file);
  if (!is_writable($tag_file)) {
    return drush_set_error('DRUSH_PERM_ERROR', dt('Cannot write the tags file. Specify another path with --tag-file=path/to/tags'));
  }
  // Check that the boolean flags are "yes" or "no".
  $flags = array('include-js', 'include-test', 'make-portable');
  foreach ($flags as $flag) {
    $option = drush_get_option($flag, 'no');
    if ($option != 'yes' && $option != 'no') {
      // TODO: Find a better error code.
      return drush_set_error('DRUSH_UNKNOWN_OPTION', dt('The --include-* options should be specified as "yes" or "no".'));
    }
  }
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_vimrc_tag_gen($path = '.') {
  // Get the option values.  The validate function sets some of them.
  $ctags = drush_get_option('ctags', 'ctags');
  $tag_file = drush_get_option('tag-file', 'tags');
  $include_js = drush_get_option('include-js', 'no');
  $include_test = drush_get_option('include-test', 'no');
  $portable = (drush_get_option('make-portable', 'no') == 'yes');
  if ($portable) {
    $path = realpath($path);
    // Replace the path (but not any trailing '/') with '$DRUPAL_ROOT'.
    $shortpath = rtrim($path, '/');
    $length = strlen($shortpath);
  }

  // Figure out which ctags options to use.
  $excludes = array('.git');
  $langmap = 'php:+.module.install.inc.profile.theme.engine';
  if ($include_test == 'yes') {
    $langmap .= '.test';
  }
  else {
    $excludes += array('simpletest', 'tests', 'Tests', 'Test', 'Test.php');
  }
  if ($include_js != 'yes') {
    $langmap .= ',javascript:';
  }
  $options = drush_get_option('ctags-options', '');
  // Add a "file" language: ctags will add a file tag for each specified
  // extension. See
  // http://knplabs.com/blog/2011/11/30/boost-your-productivity-with-sf2-and-vim/
  $options .= " --langdef=file";
  $langmap .= ",file:+.twig.yml";
  $options .= " -R --totals --extra=+f --langmap=$langmap --php-kinds=-v";
  $options .= ' --exclude=' . implode(' --exclude=', $excludes);

  // Run ctags, generating $tag_file.
  drush_log(dt('Some warning messages from ctags are normal.'));
  // Do not pass $options as an argument to drush_shell_exec() because then it
  // will get wrapped in single quotes by drush_escapeshellarg().
  drush_shell_exec("%s -f %s $options %s", $ctags, $tag_file, $path);

  // Skip lines matching these patterns.
  $patterns = array();
  // Skip anything that looks like a comment.
  $patterns[] = '#/\^\s*(\\\\/\\\\/|/\*|\*|\'\')#';
  // Skip anything that looks like 'key' => 'value'.
  $patterns[] = '/[\'"]#?\w+[\'"]\s*=>/';
  // Skip anything that looks like $output .= 'lotsa text'.
  $patterns[] = '#/\^\s*\$\w+\s*\.?\=#';

  // Read the file into an array.
  $lines = file($tag_file);
  $handle = fopen($tag_file, 'w');
  // Process the lines and rewrite the file.
  foreach ($lines as $line) {
    foreach ($patterns as $pattern) {
      if (preg_match($pattern, $line) == 1) {
        continue;
      }
    }
    if ($portable) {
      $line = str_replace($shortpath, '$DRUPAL_ROOT', $line);
    }
    fwrite($handle, $line);
  }
  fclose($handle);
}
