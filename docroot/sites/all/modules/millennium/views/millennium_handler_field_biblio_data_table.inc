<?php

class millennium_handler_field_biblio_data_table extends views_handler_field {
  function render($values) {
    $biblio_data = unserialize($values->millennium_node_bib_biblio_data);
    $table = theme('millennium_biblio_data', array('biblio_data' => $biblio_data));
    return $table;
  }
}
