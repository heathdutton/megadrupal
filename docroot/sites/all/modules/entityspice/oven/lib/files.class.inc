<?php

class __OvenFileHandlerDrush implements __OvenFileHandlerInterface {
  private $file = NULL;
  private $path = NULL;

  /**
   * Closes the file. It's okay for a module like oven to put this in
   * destructor since if it's called very late nothing important happens
   * really.
   */
  public function __destruct() {
    $this->close();
  }

  public function __construct($file=NULL) {
    if($file) {
      $this->open($file);
    }
  }

  public function write($data) {
    $this->checkFileException();
    return fwrite($this->file, $data);
  }

  public function replace($data) {
    $this->truncate();
    $this->write($data);
  }

  public function read() {
    $this->checkFileException();
    return fread($this->file, filesize($this->path));
  }

  public function open($file, $clear=FALSE) {
    $this->path = $file;
    if($clear) {
      $this->file = fopen($file, 'w');
    }
    else {
      $this->file = fopen($file, 'r+');
    }
  }

  public function close() {
    fclose($this->file);
    $this->file = NULL;
  }

  public function truncate() {
    fclose($this->file);
    $this->file = fopen($this->path, 'w');
  }

  private function checkFileException() {
    if(!$this->file) {
      throw new Exception('No file error.');
    }
  }
}

class __OvenFileHandlerDrupal implements __OvenFileHandlerInterface {
  private $file = NULL;

  /**
   * Closes the file. It's okay for a module like oven to put this in
   * destructor since if it's called very late nothing important happens
   * really.
   */
  public function __destruct() {
    $this->close();
  }

  private function __construct() {

  }

  public function write($data) {
    $this->checkFileException();
  }

  public function replace($data) {
    $this->truncate();
    $this->write($data);
  }

  public function read() {
    $this->checkFileException();
  }

  public function open($file) {

  }

  public function truncate() {
    ftruncate($this->file, 0);
  }

  private function checkFileException() {
    if(!$this->file) {
      throw new Exception('No file error.');
    }
  }
}


class __OvenDirectoryHandlerDrupal {
  private static function copyRecursive($source, $destination) {
    return TRUE;
  }
}

function _oven_default_package() {
  return 'Custom Entity Types';
}

function _oven_default_uri() {
  return 'entity/' . __OvenMachineName::get();
}

function _oven_admin_bundle_uri() {
  return 'admin/' . __OvenMachineName::get() . '_type';
}

function _oven_form_redirect_uri() {
  return '/';
}
