<?php

/**
 * @file
 * File handling helper functionality.
 */

function files($root, $includes = array(), $excludes = array()) {
  return new DrakeFilesPlaceholder($root, $includes, $excludes);
}

function fileset($name) {
  return new DrakeFilesetPlaceholder($name);
}

function fileset_create($name, $def) {
  $filesets = &drush_get_context('DRAKE_FILESETS');
  $filesets[$name] = new DrakeFileset($name, $def);
}

function fileset_get($name) {
  $filesets = &drush_get_context('DRAKE_FILESETS');
  if (isset($filesets[$name])) {
    return $filesets[$name];
  }
  elseif (substr($name, 0, 3) == 'no-' && ($no_no_name = substr($name, 3)) && isset($filesets[$no_no_name])) {
    // When the name starts with "no-" and we have a fileset named after the
    // rest, take that and invert it.
    $filesets[$name] = $filesets[$no_no_name]->invert($name);
    return $filesets[$name];
  }
  throw new DrakeContextException(dt('Unknown fileset "@name".', array('@name' => $name)));
}

class DrakeFile {
  protected $root;
  protected $file_path;

  function __construct(DrakeFilesPlaceholder $root, $path) {
    $this->root = $root;
    $this->file_path = $path;
  }

  function __toString() {
    return $this->fullPath();
  }

  function fullPath() {
    return $this->root->root() . '/' . $this->file_path;
  }

  function path() {
    return $this->file_path;
  }
}

class DrakeFilesPlaceholder implements DrakePlaceholder {
  protected $root;
  protected $includes = array();
  protected $excludes = array();

  function __construct($root, $includes = array(), $excludes = array()) {
    $this->root = $root;
    $this->includes = $includes;
    $this->excludes = $excludes;
  }

  function root() {
    return $this->root;
  }

  function resolve(DrakeContext $context) {
    if (!empty($this->includes)) {
      $includes = $this->pregify($this->includes);
    }
    else {
      // Default to all files.
      $includes = $this->pregify(array('**'));
    }
    $excludes = $this->pregify($this->excludes);

    if ($this->root instanceOf DrakePlaceholder) {
      $this->root = $this->root->resolve($context);
    }
    $this->root = rtrim($this->root, '/');

    // @todo This is a ineffective way to do it, but this is proof of concept of
    // the file matching functionality. We should do our own scanning.
    // Most arguments below are defaults, first ($dir), second ($mask) and the
    // second to last ($include_dot_files) are non-default.
    $files = array_keys(drush_scan_directory($this->root, '/.*/', array('.', '..', 'CVS'), 0, TRUE, 'filename', 0, TRUE, 0));

    $matched_files = array();
    $length = strlen($this->root) + 1;
    foreach ($files as $file) {
      $file_path = substr($file, $length);
      // $file_path = $file;
      foreach ($includes as $include) {
        if (preg_match($include, $file_path)) {
          // File is included, check if it's excluded.
          foreach ($excludes as $exclude) {
            if (preg_match($exclude, $file_path)) {
              // Break out two loops.
              continue 2;
            }
          }
          $matched_files[] = new DrakeFile($this, $file_path);
        }
      }
    }
    return $matched_files;
  }

  function pregify($patterns) {
    // RegExp escaping: an extreme sport..
    $from = array(
      '/\\\\\\?/',
      '/\\\\\\*\\\\\\*/',
      '/\\\\\\*/',
    );
    $to = array(
      '.',
      '.*',
      '[^/]*',
    );

    $preg_patterns = array();
    if (!is_array($patterns)) {
      $patterns = array($patterns);
    }
    foreach ($patterns as $pattern) {
      $pattern = preg_replace($from, $to, preg_quote($pattern, '{}'));
      // Anchor pattern to start of string, if it starts with a slash.
      if ($pattern[0] == '/') {
        $pattern[0] = '^';
      }
      // Anchor to end.
      $pattern .= '$';
      $preg_patterns[] = '{' . $pattern . '}';
    }

    return $preg_patterns;
  }
}

class DrakeFilesetPlaceholder implements DrakePlaceholder {
  protected $name;

  function __construct($name) {
    $this->name = $name;
  }

  function resolve(DrakeContext $context) {
    return fileset_get($this->name)->resolve($context);
  }
}

class DrakeFileset extends DrakeFilesPlaceholder {
  protected $name;
  protected $extend = array();
  protected $files;
  protected $resolved = FALSE;

  function __construct($name, $def) {
    $this->name = $name;

    if (!empty($def['dir'])) {
      $this->root = $def['dir'];
    }

    if (isset($def['include']) && is_array($def['include'])) {
      $this->includes = $def['include'];
    }

    if (isset($def['exclude']) && is_array($def['exclude'])) {
      $this->excludes = $def['exclude'];
    }

    if (!empty($def['extend'])) {
      $this->extend = is_array($def['extend']) ? $def['extend'] : array($def['extend']);
    }
  }

  function invert($name) {
    $def = array();
    if (!empty($this->root)) {
      $def['dir'] = $this->root;
    }
    if (!empty($this->includes)) {
      $def['exclude'] = $this->includes;
    }
    if (!empty($this->excludes)) {
      $def['include'] = $this->excludes;
    }
    // Invert extends by inverting each one.
    $extend = array();
    foreach ($this->extend as $extend_name) {
      if (substr($extend_name, 0, 3) == 'no-') {
        $extend[] = substr($extend_name, 3);
      }
      else {
        $extend[] = 'no-' . $extend_name;
      }
    }
    if (!empty($extend)) {
      $def['extend'] = $extend;
    }
    return new DrakeFileset($name, $def);
  }

  function resolve(DrakeContext $context) {
    if (!$this->resolved) {
      if (empty($this->root)) {
        throw new DrakeContextException(dt('No directory specified for fileset @name', array('@name' => $this->name)));
      }
      if (!empty($this->extend)) {
        // Merge in extended filesets includes and excludes.
        foreach ($this->extend as $name) {
          $fileset = fileset_get($name);
          $res = $fileset->getCludes();
          $this->includes = array_merge($this->includes, $res['includes']);
          $this->excludes = array_merge($this->excludes, $res['excludes']);
        }
      }

      $this->files = parent::resolve($context);
      $this->resolved = TRUE;
    }
    return $this->files;
  }

  function getCludes() {
    return array(
      'includes' => $this->includes,
      'excludes' => $this->excludes,
    );
  }
}
