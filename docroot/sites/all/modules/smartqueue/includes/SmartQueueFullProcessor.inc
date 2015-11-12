<?php

class SmartQueueFullProcessor extends SmartQueueProcessorDefault {

  /**
   * Manipulate the behavior of the queue processing by adjusting variables.
   */
  public $maxProcessTime = 0;
  public $phpTimeout = 0;

}
