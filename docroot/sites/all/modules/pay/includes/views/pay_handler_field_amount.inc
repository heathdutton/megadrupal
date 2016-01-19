<?php
class pay_handler_field_amount extends views_handler_field_numeric {

  function query() {
    $this->ensure_my_table();

    // Construct a new join so we can add the field.
    $join = new views_join();
    $join->construct('pay_transaction', 'pay_form', 'pfid', 'pfid');
    $pay_transaction_table = $this->query->ensure_table('pay_transaction', $this->table_alias, $join);
    $sum = 'SUM(' . $pay_transaction_table . '.' . $this->real_field . ')';
    $field_alias = $this->query->add_field('', $sum, $this->real_field . '_amount');
    $this->field_alias = $field = $field_alias;

    $this->add_additional_fields();
  }
}
