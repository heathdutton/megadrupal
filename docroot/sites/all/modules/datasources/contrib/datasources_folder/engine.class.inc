<?php

/**
 * @file
 * Aggregator Datasource Engine classes.
 */

class DatasourceEngineFolderException extends DatasourceEngineException {
  const FILE_NOT_FOUND = 302;
  const NODE_NOT_FOUND = 302;
}

/**
 * Aggregator feed datasource engine.
 */
abstract class DatasourceEngineFolderAbstract extends DatasourceEngineAbstract {
  
  protected $items = array();
  
  /**
   * datasources_folder object
   * 
   * @var object
   */
  protected $folder = NULL;
  
  /**
   * dir handlers stack
   * 
   * @var array
   */
  protected $stack = array();
  
  public function getFolderOption($field) {
    if (isset($this->folder->options[$field])) {
      return $this->folder->options[$field];
    }
    return null;
  } 
  
  public function initImport() {
    $ret=parent::initImport();
    if (!$ret) return $ret;
    
    $this->folder = datasources_folder_load($this->input['settings']['dfid']);
    
    if (!is_dir($this->folder->options['root'])) {
      watchdog('datasources_folder', 'root dir !root does not exist (!folder).', array('!root' => $this->folder->options['root'], '!folder' => $this->folder->name), WATCHDOG_ERROR);
      return FALSE;
    }
    
    // first open dir
    $this->addStack($this->folder->options['root']);
    return TRUE;
  }
  
  public function completeImport() {
    parent::completeImport();
  }
  
  protected function getNextDir() {
    $top = $this->getStackTop();
    
    // nothing left to do
    if (!$top) return FALSE;
    
    // special case for root
    if($this->initDir($top)) {
      return $top;
    }
    list(, $subdir) = each($top->_subdirs);
    if ($subdir) {
      $this->addStack($subdir);
      return $this->getNextDir();
    }
    // if no more subdir shift the stack
    $this->shiftStack();
    
    // try again
    return $this->getNextDir();
  }
  
  protected function shiftStack() {
    return array_shift($this->stack);
  }
  
  protected function addStack($dir) {
    array_unshift($this->stack, (object)array('dir' => $dir));
  }
  
  protected function &getStackTop() {
    static $null = null;
    if (empty($this->stack)) return $null;
    return $this->stack[0];
  }
  
  /**
   * Init the given stack level.
   * 
   * @param object $level
   * @return boolean TRUE if level was just init
   */
  protected function initDir(&$level) {
    if (empty($level->read)) {
      if (!$dh = opendir($level->dir)) {
        watchdog('datasources_folder', 'Cannot open dir !dir (!folder).', array('!dir' => $level->dir, '!folder' => $this->folder->name), WATCHDOG_ERROR);
        return FALSE;
      }
      $level->subdirs = array();
      $level->texts = array();
      $level->images = array();
      while (($file = readdir($dh)) !== FALSE) {
        if ($file == '.' || $file == '..') continue;
        
        $parts = array();
        if (preg_match('/^(.*)\.([^.]+)$/', $file, $matches)) {
          $parts['basename'] = $matches[1];
          $parts['extension'] = $matches[2];
        }
        else {
          $parts['basename'] = $file;
        }
        if (is_dir($level->dir . '/' . $file)) {
          $level->subdirs[] = $level->dir . '/' . $file;
          continue;
        }
        if (isset($parts['extension'])) {
          if (preg_match('/^jpe?g$/i', $parts['extension'])) {
            $level->images[$parts['basename']] = (object)array('image' => $level->dir . '/' . $file);
            continue;
          }
          if (preg_match('/^(html?|txt)$/i', $parts['extension'])) {
            $level->texts[$parts['basename']] = $level->dir . '/' . $file;
          }
        }
      }
      closedir($dh);
      
      if ($this->getFolderOption('detect_caption')) {
        // detect image caption file
        foreach ($level->images as $basename => &$image) {
          if (isset($level->texts[$basename])) {
            $image->caption = $level->texts[$basename];
            unset($level->texts[$basename]);
          }
        }
      }
            
      $level->read = TRUE;
      
      $level->_subdirs = $level->subdirs;
      $level->_images = $level->images;
      
      // this cursors are important
      reset($level->_subdirs);
      reset($level->_images);
      return TRUE;
    }
    return FALSE;
  }
  
  /**
   * Return the next dir with valid texts
   * 
   * @return Ambigous stdClass|boolean
   */
  protected function getNextText() {
    while($dir = $this->getNextDir()) {
      if (empty($dir->texts)) continue;
      return $dir;
    }
    return FALSE;
  }
  
  /**
   * Return the next image
   *
   * @return stdClass|boolean
   */
  protected function getNextImage() {
    static $dir = null;
    if (!$dir) {
      $dir = $this->getNextDir();
      if (!$dir) return $dir;
    }
    
    list(, $image) = each($dir->_images);
    if ($image) {
      $image = clone $image;
      $image->parent = $dir;
      return $image;
    }
    $dir = null;
    return $this->getNextImage();
  }
  
  protected function getFileText($file, $options = array()) {
    if (preg_match('/\.txt$/i', $file)) {
      return file_get_contents($file);
    }
    return FALSE;
  }
  
  protected function getRelPath($path) {
    return substr($path, strlen($this->folder->options['root'])+1);
  }
}

class DatasourceEngineFolderTextContents extends DatasourceEngineFolderAbstract {
  protected function getRecord() {
    $text = $this->getNextText();
    if (!$text) return FALSE;
    $item = array();
    $item['title'] = array();
    $item['text'] = array();
    $item['text_files'] = array();
    $first = true;
    foreach ($text->texts as $file) {
      $item['text_files'][] = $this->getRelPath($file);
      $item = array_merge_recursive($this->getFileText($file, $first), $item);
      if ($this->getFolderOption('multiple_text') == 'first') {
        break;
      }
      $first = false;
      
    }
    $item['text'] = array_values($item['text']);
    $item['title'] = reset($item['title']);
    $item['path'] = $this->getRelPath($text->dir);
    
    $stats = stat($text->dir);
    $item['modification'] = (int)$stats['mtime'];
    $item['creation'] = (int)$stats['ctime'];
    $item['image_files'] = array();
    foreach ($text->images as $image) {
      $item['image_files'][] = $this->getRelPath($image->image);
    }
    $item['children'] = array();
    foreach ($text->subdirs as $subdir) {
      $item['children'][] = $this->getRelPath($subdir);
    }
    return $item;
  }
  
  protected function getFileText($file, $options = array()) {
    if (!is_array($options)) {
      $options = array(
        'first' => $options,
      );
    }
    if (!isset($options['first'])) $options['first'] = FALSE;
    if (preg_match('/\.txt$/i', $file)) {
      $lines = array_values(array_filter(file($file), function($line) {
        if (!trim($line)) return FALSE;
        return TRUE;
      }));
      $ret = array();
      if ($options['first']) {
        // for first text file first line is the title
        $ret['title'] = $lines[0];
        unset($lines[0]);
      }
      $ret['text'] = $lines;
      return $ret;
    }
    return array();
  }
}

class DatasourceEngineFolderImageContents extends DatasourceEngineFolderAbstract {
  protected function getRecord() {
    $image = $this->getNextImage();
    if (!$image) return FALSE;
    $item = array();
    $item['caption'] = '';
    if (isset($image->caption)) {
      $text = $this->getFileText($image->caption);
      if (!is_array($text)) {
        $text = array('text' => $text);
      }
      if (isset($text['text'])) {
        $text['caption'] = $text['text'];
        unset($text['text']);
      }
      $item = array_merge($item, $text);
    }

    $item['path'] = $this->getRelPath($image->image);
    
    $stats = stat($image->image);
    $item['modification'] = (int)$stats['mtime'];
    $item['creation'] = (int)$stats['ctime'];
    return $item;
  }
}
