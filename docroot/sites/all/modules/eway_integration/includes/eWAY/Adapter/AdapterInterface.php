<?php

namespace eWAY\Adapter;

interface AdapterInterface {
  public function send(TransactionInterface $transaction);
}
