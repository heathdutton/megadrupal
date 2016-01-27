<?php

class pay_handler_field_count extends views_handler_field_numeric {

  function query() {
    $this->ensure_my_table();
    // Construct a new join so we can add the field.
    $join = new views_join();
    $join->construct('pay_transaction', 'pay_form', 'pfid', 'pfid');
    $pay_transaction_table = $this->query->ensure_table('pay_transaction', $this->table_alias, $join);
    $count = 'COUNT(' . $pay_transaction_table . '.' . $this->real_field . ')';
    $count = 'COUNT(' . $pay_transaction_table . '.uid)';
//    $this->query->add_groupby($pay_transaction_table .'.uid');
    $field_alias = $this->query->add_field('', $count, $this->real_field . '_count');
    $this->field_alias = $field = $field_alias;

    $this->add_additional_fields();
  }
}
