(function ($) {

Drupal.behaviors.menuBlockSettingsOverrideFieldsetSummaries = {
  attach: function (context) {
    $('fieldset.menu-block-settings-override-form', context).drupalSetSummary(function (context) {
      var id = $(context).attr('id');
      var delta = id.replace(/edit-menu-block-settings-override-/, '');
      var hidden = $('.form-item-menu-block-settings-override-' + delta + '-hidden input', context).attr('checked');
      var status = $('.form-item-menu-block-settings-override-' + delta + '-settings-status-' + delta + ' input', context).attr('checked');

      if (hidden) {
        return Drupal.t('Hidden');
      }
      else if (status) {
        return Drupal.t('Settings overriden');
      }
      else {
        return Drupal.t('No settings overriden');
      }
    });
  }
};

Drupal.behaviors.menu_block_multiple = {
  attach: function (context, settings) {
    $(context).find('fieldset.menu-block-settings-override-form').each(function (context) {

      var id = $(this).attr('id');
      var delta = id.replace(/edit-menu-block-settings-override-/, '');
      // This behavior attaches by ID, so is only valid once on a page.
      if ($('#menu-block-settings-' + delta + '.menu-block-processed').size()) {
        return;
      }
      $('#menu-block-settings-' + delta, context).addClass('menu-block-processed');
      
      // Show the "display options" if javascript is on.
      $('.form-item-menu-block-settings-override-' + delta + '-custom-settings-display-options.form-type-radios>label').addClass('element-invisible');
      // Add original class so Menu Block CSS file will style the Display Options buttons.
      $('.form-item-menu-block-settings-override-' + delta + '-custom-settings-display-options').addClass('form-item-display-options');
      $('.form-item-menu-block-settings-override-' + delta + '-custom-settings-display-options.form-type-radios').show();
      // Make the radio set into a jQuery UI buttonset.
      $('#edit-menu-block-settings-override-' + delta + '-custom-settings-display-options').buttonset();
      // Override the default show/hide animation for Form API states.
      $('#menu-block-settings-' + delta).bind('state:visible', function(e) {
        if (e.trigger) {
          // Stop the handler further up the tree.
          e.stopPropagation() 
          $(e.target).closest('.form-item, .form-wrapper')[e.value ? 'slideDown' : 'slideUp']('fast');
        }
      });
    });
  }
};

})(jQuery);
