function ucMercadoPago(json) {
  console.log(json);
  if (json.collection_status=='approved'){
    alert ('Pago acreditado');
  } else if(json.collection_status=='pending'){
    alert ('El usuario no completó el pago');
  } else if(json.collection_status=='in_process'){
    alert ('El pago está siendo revisado');
  } else if(json.collection_status=='rejected'){
    alert ('El pago fué rechazado, el usuario puede intentar nuevamente el pago');
  } else if(json.collection_status==null){
    alert ('El usuario no completó el proceso de pago, no se ha generado ningún pago');
  }
}

(function ($) {

  Drupal.behaviors.ucMercadoPago = {
    attach: function (context, settings) {
      if (Drupal.settings.ucMercadoPago && Drupal.settings.ucMercadoPago.country) {
        $.ajax({
          dataType: "jsonp",
          url: 'https://api.mercadolibre.com/sites/' + Drupal.settings.ucMercadoPago.country + '/payment_methods',
          success: function(data, textStatus, jqXHR) {
            var items = [];

            $.each(data[2], function(key, val) {
              items.push('<img title="' + val.name + '" alt="' + val.name + '"src="' + val.secure_thumbnail + '">');
            });

            $('.uc-mercadopago-payment-methods').html(items.join(' '));
          }
        });
      }
    }
  };

})(jQuery);
