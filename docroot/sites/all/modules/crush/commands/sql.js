(function($) {

Drupal.crush.commands.sql = function(parsed) {
  if (parsed.args.length == 0) {
    Drupal.crush.lineOut('sql: Needs at least one argument.');
    return;
  }
  var lineId = Drupal.crush.lineOut(Drupal.t('Querying...'));
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
    Drupal.crush.lineOut(Drupal.t(' done.'), lineId);
  });
  addDot();
};

})(jQuery);
