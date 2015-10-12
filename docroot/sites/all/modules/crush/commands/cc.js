(function($) {

Drupal.crush.commands.cc = function(parsed) {
  var lineId = Drupal.crush.lineOut(Drupal.t('Clearing cache...'));
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
      Drupal.crush.lineOut(Drupal.t(' cleared.'), lineId);
    }
    else {
      Drupal.crush.lineOut(Drupal.t(' ERROR. Cache may not have cleared successfully.'), lineId);
    }
  });
  addDot();
};

})(jQuery);
