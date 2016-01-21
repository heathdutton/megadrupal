<?php

abstract class DropSharkLinfoCollector extends DropSharkCollectorBase {

  /** @var \Linfo\Linfo */
  protected $linfo;

  protected $parser;

  public function __construct($definition) {
    parent::__construct($definition);

    if ($this->linfo = dropshark_get_linfo()) {
      $this->parser = $this->linfo->getParser();
    }
  }

  protected function hasLinfo() {
    return $this->linfo && $this->parser;
  }

}
