(function($) {

Drupal.crush.commands.reinstall = function(parsed) {
  var lineId = Drupal.crush.lineOut(Drupal.t('Reinstalling modules...'));
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
      Drupal.crush.lineOut(Drupal.t(' reinstalled.'), lineId);
    }
    else {
      Drupal.crush.lineOut(Drupal.t(' ERROR. See details below.'), lineId);
    }
  });
  addDot();
};

})(jQuery);
