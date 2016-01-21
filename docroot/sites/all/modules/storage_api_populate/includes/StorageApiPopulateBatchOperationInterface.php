<?php

interface StorageApiPopulateBatchOperationInterface {
  function count();
  function process($current, $total);
  function getProgressMessage();
}

