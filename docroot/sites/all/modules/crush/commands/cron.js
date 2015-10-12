(function($) {

Drupal.crush.commands.cron = function(parsed) {
  var lineId = Drupal.crush.lineOut(Drupal.t('Running cron...'));
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
      Drupal.crush.lineOut(Drupal.t(' cron finished successfully.'), lineId);
    }
    else {
      Drupal.crush.lineOut(Drupal.t(' ERROR. Cron may not have finished running.'), lineId);
    }
  });
  addDot();
};

})(jQuery);
