(function($) {

Drupal.crush.commands.admin = function(parsed) {
  if (parsed.args === undefined || parsed.args === null || !parsed.args.length) {
    Drupal.crush.go('admin/index', parsed);
    return;
  }
  var lineId = Drupal.crush.lineOut(Drupal.t('Loading...'));
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
    Drupal.crush.lineOut(Drupal.t(' loaded.'), lineId);
    $.each(data.messages, function(k, v) {
      Drupal.crush.lineOut(v);
    });
    var map = data.map;
    Drupal.crush.prompt(function(val) {
      Drupal.crush.lineOut('>>> ' + val);
      if (typeof map[val] == 'string') {
        Drupal.crush.go(map[val], parsed);
        Drupal.crush.lineOut(Drupal.t('Redirecting...'));
      }
      else {
        Drupal.crush.lineOut(Drupal.t('Unrecognized option - aborting.'));
      }
    });
  });
  addDot();
};

})(jQuery);
