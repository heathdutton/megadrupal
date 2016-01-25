(function($) {

Drupal.crush.commands.ls = function(parsed) {
  var lineId = Drupal.crush.lineOut(Drupal.t('Loading...'));
  var stillWorking = true;
  var timeout = 200;
  var addDot = function() {
    if (stillWorking) {
      Drupal.crush.lineOut('.', lineId);
      setTimeout(addDot, timeout);
    }
  };
  parsed.current_path = $('iframe').attr('src').replace(Drupal.settings.basePath, '').replace('?q=', '');
  Drupal.crush.processAjax(parsed, function(data) {
    stillWorking = false;
    Drupal.crush.lineOut(Drupal.t(' loaded.'), lineId);
  });
  addDot();
};

})(jQuery);
