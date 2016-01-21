<?php

/**
 * @file
 * The format for brazilian addresses.
 */

$plugin = array(
  'title' => t('Brazilian address'),
  'format callback' => 'br_address_format_address_generate',
  'type' => 'address',
  'weight' => -80,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function br_address_format_address_generate(&$format, $address, $context = array()) {

  $format['#attached']['css'][] = drupal_get_path('module', 'addressfield') . '/addressfield.css';

  $format['street_block'] = array(
    '#type' => 'addressfield_container',
    '#attributes' => array(
      'class' => array(
        'street-block',
        'addressfield-container-inline',
        'br-adress-logradouro-bairro',
      ),
    ),
    '#weight' => 0,
  );
  $format['street_block']['thoroughfare'] = array(
    '#title' => t('Thoroughfare'),
    '#attributes' => array('class' => array('thoroughfare')),
    '#size' => 60,
    '#description' => t('Type and name of thoroughfare and address number. Ex: Av. Beira Mar, 547.'),
    '#required' => TRUE,
    '#weight' => 0,
  );
  // I'm not sure, but according to columns comments at the field's table, the
  // premise column is the nearest for what we call "complemento" in Brazil.
  $format['street_block']['premise'] = array(
    '#title' => t('Complement'),
    '#attributes' => array('class' => array('premise', 'inline')),
    '#size' => 30,
    '#required' => FALSE,
    '#weight' => 10,
  );
  // I'm not sure again, but I think the dependent locality is the best column
  // for store the neighborhood ("bairro").
  $format['street_block']['dependent_locality'] = array(
    '#title' => t('Neighborhood'),
    '#attributes' => array('class' => array('dependent-locality', 'inline')),
    '#size' => 30,
    '#required' => FALSE,
    '#weight' => 20,
  );

  $format['locality_block'] = array(
    '#type' => 'addressfield_container',
    '#attributes' => array(
      'class' => array(
        'addressfield-container-inline',
        'locality-block',
        'br-address-cidade-estado',
      ),
    ),
    '#weight' => 10,
  );
  $format['locality_block']['locality'] = array(
    '#title' => t('City'),
    '#size' => 30,
    '#required' => TRUE,
    '#attributes' => array('class' => array('locality')),
    '#weight' => 0,
  );
  // Enable the autocomplete for cities names.
  if ($context['instance']['widget']['settings']['br_address_options']['autocomplete_city']) {
    $format['locality_block']['locality']['#autocomplete_path'] = 'br-address/autocomplete';
  }
  $format['locality_block']['administrative_area'] = array(
    '#title' => t('State'),
    '#size' => 10,
    '#required' => TRUE,
    '#attributes' => array('class' => array('state')),
    '#weight' => 10,
    '#options' => array(
      ''   => t('--'),
      'AC' => t('Acre'),
      'AL' => t('Alagoas'),
      'AM' => t('Amazonas'),
      'AP' => t('Amapa'),
      'BA' => t('Bahia'),
      'CE' => t('Ceara'),
      'DF' => t('Distrito Federal'),
      'ES' => t('Espirito Santo'),
      'GO' => t('Goias'),
      'MA' => t('Maranhao'),
      'MG' => t('Minas Gerais'),
      'MS' => t('Mato Grosso do Sul'),
      'MT' => t('Mato Grosso'),
      'PA' => t('Para'),
      'PB' => t('Paraiba'),
      'PE' => t('Pernambuco'),
      'PI' => t('Piaui'),
      'PR' => t('Parana'),
      'RJ' => t('Rio de Janeiro'),
      'RN' => t('Rio Grande do Norte'),
      'RO' => t('Rondonia'),
      'RR' => t('Roraima'),
      'RS' => t('Rio Grande do Sul'),
      'SC' => t('Santa Catarina'),
      'SE' => t('Sergipe'),
      'SP' => t('Sao Paulo'),
      'TO' => t('Tocantins'),
    ),
  );

  // Maybe the mandatory of postal code could be a setting option.
  $postal_code = array(
    '#title' => t('Postal Code'),
    '#size' => 10,
    // '#required' => TRUE,
    '#attributes' => array('class' => array('postal-code')),
  );

  // CEP query.
  if ($context['instance']['widget']['settings']['br_address_options']['cep_query']) {
    $postal_code['#ajax'] = array(
      'event' => 'change',
      'callback' => 'br_address_cep_query',
    );
    // Put at the top of form and out of any container.
    $postal_code['#weight'] = -10;
    $format['postal_code'] =& $postal_code;
  }
  else {
    // If CEP query is disabled, append the field to the locality_block.
    $postal_code['#weight'] = 20;
    $format['locality_block']['postal_code'] =& $postal_code;
  }

  // Set Brazil as only available country and hide the field.
  $format['country'] = array(
    '#title' => t('Country'),
    '#options' => array('BR' => t('Brazil')),
    '#required' => TRUE,
    '#attributes' => array('class' => array('country', 'br-address-pais')),
    '#weight' => 20,
    '#access' => FALSE,
  );

  // In render mode, put some separating characters and put postal code back
  // to the end.
  if ($context['mode'] == 'render') {
    $format['street_block']['premise']['#prefix'] = ' - ';
    $format['street_block']['dependent_locality']['#prefix'] = ' - ';
    $format['locality_block']['administrative_area']['#prefix'] = ' / ';
    $format['locality_block']['administrative_area']['#prefix'] = ' / ';
    $postal_code['#tag'] = 'div';
    $postal_code['#weight'] = 20;
  }
}

/**
 * Callback function for the AJAX change event of postal code (CEP).
 *
 * Query the Correios site for the address of that CEP and fill the form with
 * the data returned.
 *
 * @see http://api.drupal.org/api/drupal/developer%21topics%21forms_api_reference.html/7#ajax
 */
function br_address_cep_query($form, $form_state) {

  $commands = array();

  // Build and perform the HTTP Post resquest.
  $cep = $form_state['triggering_element']['#value'];
  $data = http_build_query(array(
    'cepEntrada' => $cep,
    'tipoCep' => '',
    'cepTemp'  => '',
    'metodo' => 'buscarCep',
  ));
  $url = 'http://m.correios.com.br/movel/buscaCepConfirma.do';
  $response = drupal_http_request($url . '?' . $data, array(
    'method' => 'POST',
    'data' => $data,
  ));

  if ($response->code == 200) {
    $parents = $form_state['triggering_element']['#array_parents'];
    $first = array_shift($parents);
    $last = array_pop($parents);
    $base_name = $first . '[' . implode('][', $parents) . ']';

    $html = utf8_encode($response->data);
    if (preg_match_all('#<span class="respostadestaque">([^<]+)</span>#ms', $html, $m)) {

      $rua    = trim($m[1][0]);
      $bairro = trim($m[1][1]);
      list($cidade, $estado) = explode('/', $m[1][2]);
      $cidade = trim($cidade);
      $estado = trim($estado);

      // Ajax commands for fill the form.
      $commands[] = ajax_command_invoke("[name=\"{$base_name}[thoroughfare]\"]", 'val', array($rua));
      $commands[] = ajax_command_invoke("[name=\"{$base_name}[dependent_locality]\"]", 'val', array($bairro));
      $commands[] = ajax_command_invoke("[name=\"{$base_name}[locality]\"]", 'val', array($cidade));
      $commands[] = ajax_command_invoke("[name=\"{$base_name}[administrative_area]\"]", 'val', array($estado));

      // The focus should ends at the thoroughfare, to the user enter the number
      // and complement.
      $commands[] = ajax_command_invoke("[name=\"{$base_name}[thoroughfare]\"]", 'focus');
    }
    else {
      $commands[] = ajax_command_alert(t("Address not found.\n\nPlease, verify if the postal code is correct or fill the address manually."));
      $commands[] = ajax_command_invoke("[name=\"{$base_name}[postal_code]\"]", 'focus');
    }
  }
  else {
    $commands[] = ajax_command_alert(t('Error') . ': ' . $response->status_message);
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}
