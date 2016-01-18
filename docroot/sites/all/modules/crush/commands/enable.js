(function($) {

Drupal.crush.commands.enable = function(parsed) {
  var lineId = Drupal.crush.lineOut(Drupal.t('Enabling modules...'));
  var stillWorking = true;
  var timeout = 200;
  var addDot = function() {
    if (stillWorking) {
      Drupal.crush.lineOut('.', lineId);
      setTimeout(addDot, timeout);
    }
  };
  Drupal.crush.processAjax(parsed, function(data) {
    stillWorking = false;
    if (data == 'success') {
      Drupal.crush.lineOut(Drupal.t(' enabled.'), lineId);
    }
    else {
      Drupal.crush.lineOut(Drupal.t(' ERROR. See details below.'), lineId);
    }
  });
  addDot();
};

})(jQuery);
