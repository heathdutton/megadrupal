(function($, Drupal) {

Drupal.behaviors.templateChooser = {
  attach: function(context, settings) {
    $('.template-chooser-select-input', context).hide();

    $(".template-chooser-modal", context).dialog({
      width: 600,
      height: 400,
      modal: true,
      autoOpen: false
    });

    $('a.template-chooser-select').die('click').live('click', function(e) {
      e.preventDefault();
      var $select = $('select.' + $(this).parents('.template-chooser-modal').attr('id').replace('-modal', ''));
      
      var template_name = $(this).parent('.template-chooser-option').attr('data-template');

      $select.val(template_name).change();

      $(this).parents('.template-chooser-modal').dialog('close');
      return false;
    });

    $('a.template-chooser-open-dialog', context).die('click').live('click', function (e) {
      e.preventDefault();
      $('#' + $(this).attr('data-target')).dialog('open');
      return false;
    });
  },
  detach: function(context) {

  }
}

})(jQuery, Drupal);