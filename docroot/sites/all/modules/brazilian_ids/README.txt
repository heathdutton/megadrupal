This module adds 3 new widgets to the text field type at the Field UI.

They are:
CPF - Accepts only valid tax number of individuals
CNPJ - Accepts only valid tax number of companies
CPF / CNPJ - Accepts either one as long as it is a valid number

Learn more about these Brazilian tax numbers at wikipedia:
CPF and CNPJ

The view format display are done as followed:
CPF - 999.999.999-99
CNPJ - 99.999.999/9999-99

This module also adds 3 new field types for the Form API.
They are:
number_cpf, number_cnpj and number_cnpj_cpf

So, developers can easily build form elements of Brazilian Tax Number types.
For example:

  $form['my_cpf_field'] = array(
    '#type' => 'number_cpf',
    '#title' => t('CPF'),
  );
  $form['my_cnpj_field'] = array(
    '#type' => 'number_cnpj',
    '#title' => t('CNPJ'),
  );
  $form['my_cnpj_cpf_field'] = array(
    '#type' => 'number_cnpj_cpf',
    '#title' => t('CNPJ or CPF'),
  );

And that's it! You are done.
