(function($) {

Drupal.crush.commands.eval = function(parsed) {
  var lineId = Drupal.crush.lineOut(Drupal.t('Evaluating...'));
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
    Drupal.crush.lineOut(Drupal.t(' evaluated.'), lineId);
  });
  addDot();
};

})(jQuery);
