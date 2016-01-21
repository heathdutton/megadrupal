(function($) {

// Brazilian postal code validation regular expression.
var validatePostalCode = /^[0-9]{5}-?[0-9]{3}$/;

Drupal.behaviors.addressfieldViaCEP = {
  attach: function(context, settings) {
    $('.addressfield-viacep-postal-code', context).blur(function() {
      var postalCode = $(this).val();

      if (!postalCode) {
        // Do nothing on empty CEP.
        return;
      }

      if(!validatePostalCode.test(postalCode)) {
        // Invalid postal code.
        return alert('CEP inválido.');
      }

      // Get form element so we alter the correct fields.
      var form = $(this).closest('.addressfield-viacep-wrapper');

      // Query webservice at viacep.com.br.
      $.getJSON('//viacep.com.br/ws/' + postalCode + '/json/?callback=?', function(data) {
        if (!('erro' in data)) {
          // Update address fields.
          $('.thoroughfare', form).val(data.logradouro);
          $('.dependent-locality', form).val(data.bairro);
          $('.locality', form).val(data.localidade);
          $('.state', form).val(data.uf);
        }
        else {
          // Address not found.
          alert('CEP não encontrado. Por favor preencha o endereço manualmente.');
        }
      });
    });
  }
};

})(jQuery);
