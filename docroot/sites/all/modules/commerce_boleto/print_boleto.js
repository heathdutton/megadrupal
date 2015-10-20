function print_boleto() {
  var print_path = Drupal.settings['commerce_boleto']['print_path'];

  return window.open(print_path, 'commerce-boleto-print', 'width=700,height=500,left=0,top=100,screenX=0,screenY=100').focus();
}
