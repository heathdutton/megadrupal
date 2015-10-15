Usage
---

- Create a date field with date_popup widget.
- In the field/widget settings go to «Choose how to render the datepicker»
and select the option you want.

API
---

To create a date_popup form element with datepicket inline, use #flavour parameter:

<?php
  $form['date'] = array(
    '#type' => 'date_popup',
    '#flavour' => 'inline',
    ...
  );
?>

