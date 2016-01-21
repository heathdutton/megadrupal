(function ($) {

/**
 * 
 */
Drupal.behaviors.dnd_fields = {
  /**
   * Attaches the behavior.
   *
   */
  attach: function (context, settings) {
    var sources = {
      'str' : 'strength',
      'dex' : 'dexterity',
      'con' : 'constitution',
      'int' : 'intelligence',
      'wis' : 'wisdom',
      'chr' : 'charisma',
    }
    for (key in sources) {
      $('.dnd-fields-ability-score-' + sources[key] + '-field input', context).bind('keyup.displayName change.displayName', function () {
        $('.dnd-fields-ability-mod-' + $(this).attr('rel') + '-field input', context)
          .val(dnd_fields_ability_modifier($(this).val()));
        });
      }
    },
  };
})(jQuery);

function dnd_fields_ability_modifier(score) {
  if (score) {
    var mod = Math.floor((score / 2) - 5);
    if (parseInt(mod)) {
      return mod;
    }
  }
  return '';
}