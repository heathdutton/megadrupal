
Drupal.hideShippingAddress = function(hide) {
  if (hide) {
    $('.customer-addresses').removeClass('shippable');
    $('#customer-address-shipping').hide();
  }
  else {
    $('.customer-addresses').addClass('shippable');
    $('#customer-address-shipping').show();
  }
};

Drupal.behaviors.ecCustomer = function() {
  if ($('#ec-customer-use-for-shipping:checked').size()) {
    Drupal.hideShippingAddress(true);
  }
  else {
    Drupal.hideShippingAddress(false);
  }
  $('#ec-customer-use-for-shipping').click(function() {
    if (this.checked) {
      Drupal.hideShippingAddress(true);
    }
    else {
      Drupal.hideShippingAddress(false);
    }
  });
};
