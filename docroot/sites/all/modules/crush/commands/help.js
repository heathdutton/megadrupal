(function($) {

Drupal.crush.commands.help = function(parsed) {
  if (parsed.args !== undefined && parsed.args.length !== 0) {
    topic = parsed.args.shift();
  } 
  else {
    topic = 'help';
  }
  if (Drupal.settings.crushCommands[topic] !== undefined && Drupal.settings.crushCommands[topic] !== '') {
    Drupal.crush.lineOut(Drupal.settings.crushCommands[topic]);
  }
  else {
    Drupal.crush.lineOut(Drupal.t('No help topics match "!topic".', {'!topic': topic}));
  }
};

})(jQuery);
